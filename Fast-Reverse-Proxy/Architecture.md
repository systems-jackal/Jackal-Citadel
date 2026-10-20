# Architecture

## Components

```
┌──────────────┐         ┌───────────────┐         ┌──────────────┐
│ jackal@overnode       │     frps      │         │ spectre@kali │
│ (visitor)    │         │  (public VPS) │         │ (proxy/target)│
│              │         │ 162.35.122.112│         │              │
│  frpc        │────────▶│   :7000       │◀────────│   frpc       │
│ (visitor     │  TLS,   │ control plane │  TLS,   │ (proxy role, │
│  role)       │  token  │ + STCP broker │  token  │  "secret-ssh")│
│              │  auth   │               │  auth   │              │
│ binds        │         │ no public port│         │ connects to  │
│ 127.0.0.1    │         │ for SSH, ever │         │ 127.0.0.1:22 │
│ :6022        │         │               │         │ (sshd)       │
└──────────────┘         └───────────────┘         └──────────────┘
```

Three roles, three machines, one shared secret pair:

1. **frps** (relay) — the only component with a public IP. Listens on a single
   TCP port (`bindPort`, here `7000`). Authenticates any connecting `frpc` with a
   shared `auth.token`. Brokers STCP pairings between a registered proxy and a
   visitor that presents the matching `secretKey`. Never opens a port on behalf
   of an STCP proxy — that's the entire point of the proxy type.

2. **frpc — proxy role** (on `kali`) — registers itself with frps as a named STCP
   proxy (`secret-ssh`), pointing at a *local* address and port (`127.0.0.1:22`).
   It does not request a public port. It just tells frps "I exist, here's my
   name, here's the secret that gates access to me."

3. **frpc — visitor role** (on `overnode`) — connects to frps, declares it wants
   to reach the proxy named `secret-ssh`, and presents the same `secretKey`. On
   success, frps mediates a connection between the two `frpc` processes, and the
   visitor's `frpc` opens a **local** listener (`bindAddr`/`bindPort`,
   `127.0.0.1:6022`) that tunnels anything sent to it through to the proxy side.

## Connection lifecycle

1. `frpc` (proxy, kali) dials out to `frps:7000`, authenticates with the token,
   registers the `secret-ssh` STCP proxy. This connection stays open
   (heartbeats) for as long as the service runs.
2. `frpc` (visitor, overnode) dials out to the same `frps:7000`, authenticates
   with the same token, declares intent to visit `secret-ssh`, and presents the
   `secretKey`.
3. frps verifies the secret matches what the proxy registered. If it matches,
   frps instructs both `frpc` processes to set up a dedicated work connection
   for this pairing.
4. The visitor's `frpc` opens `127.0.0.1:6022` locally and keeps it listening.
5. A user runs `ssh -p 6022 spectre@127.0.0.1` on overnode. That connection is
   accepted by the visitor's `frpc`, tunneled over the existing frps-mediated
   channel, delivered to the proxy's `frpc` on kali, which connects it to
   `127.0.0.1:22` — kali's real sshd, which is listening *only* on loopback.
6. sshd handles authentication exactly as it always would (key or password) —
   frp is fully transparent to the SSH protocol itself; it has no visibility
   into or influence over SSH's own auth.

## Why two separate secrets exist, and why that matters

| Secret | Scope | Compromise impact |
|---|---|---|
| `auth.token` | Shared by frps + every frpc instance | Lets an attacker *register as a client* with frps — they could register their own bogus proxy/visitor, or exhaust `allowPorts`-governed resources, but they still cannot reach `secret-ssh` without `secretKey` |
| `secretKey` (STCP) | Shared only between this specific proxy and its intended visitor(s) | Lets an attacker reach the `secret-ssh` proxy specifically — i.e., reach kali's loopback SSH — but only if they also independently satisfy frps's token auth first |

Both secrets must be compromised together for an attacker to reach SSH through
this path. This is a meaningful improvement over a single shared secret gating
everything, and it's a design property specific to STCP/XTCP — plain TCP proxies
only have the token layer, since the "proxy" itself is just a public port once
registered.

## Config anatomy

**frps.toml** (relay) — top-level keys must precede any `[table]` header, a real
gotcha in frp's TOML parser (see note below):

```toml
bindPort = 7000
allowPorts = [{ start = 6000, end = 6100 }]
transport.tls.force = true
log.to = "console"
log.level = "info"

[auth]
method = "token"
token = "<32-byte-random, generated via `openssl rand -base64 32`>"
```

**frpc.toml — proxy role** (on kali):

```toml
serverAddr = "<Server-Hosting the FRPS IP Address >"
serverPort = 7000
transport.tls.enable = true
auth.method = "token"
auth.token = "<same token as frps>"

[[proxies]]
name = "secret-ssh"
type = "stcp"
secretKey = "<separate 32-byte-random secret>"
localIP = "127.0.0.1"
localPort = 22
```

**frpc.toml — visitor role** (on overnode):

```toml
serverAddr = "xxx.x.xxx.xxx"
serverPort = 7000
transport.tls.enable = true
auth.method = "token"
auth.token = "<same token as frps>"

[[visitors]]
name = "secret-ssh-visitor"
type = "stcp"
serverName = "secret-ssh"      # must match the proxy's `name`
secretKey = "<same secret as the proxy>"
bindAddr = "127.0.0.1"
bindPort = 6022
```

### Known gotcha: TOML key ordering

frp's config loader treats any bare key written *after* a `[table]` header as a
sub-key of that table. Writing `allowPorts` after `[auth]` silently turns it into
`auth.allowPorts`, which doesn't exist — and the resulting error,
`json: unknown field "allowPorts"`, misleadingly suggests the key name is wrong
rather than its position. Fix: keep every top-level key above the first
`[table]`/`[[array-of-tables]]` block in the file.

## Process hardening (both frpc instances)

Both `frpc` processes run as a dedicated, unprivileged, login-disabled system
user (`useradd --system --no-create-home --shell /usr/sbin/nologin frpc`), under
a systemd unit with:

- `NoNewPrivileges=true`
- `ProtectSystem=strict`, `ProtectHome=true`, `PrivateTmp=true`
- `CapabilityBoundingSet=` / `AmbientCapabilities=` (empty — frpc needs zero
  Linux capabilities; it only makes outbound TCP connections)
- `ReadOnlyPaths=/etc/frpc`
- Config file and directory at `0600`/`0700`, owned by `frpc:frpc` — unreadable
  by any other account on the box, including the human operator

This means a compromise of the `frpc` process itself — a bug in frp, a supply
chain issue in the binary, etc. — yields an attacker a process with no
capabilities, no write access outside its own narrow config path, and no
plausible privilege escalation route via the service itself.
