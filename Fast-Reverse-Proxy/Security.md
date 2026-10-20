# Security Analysis

## Threat model this design addresses

The baseline it's compared against is "SSH exposed directly to the internet on
the target's own IP" or "SSH reachable via a plain frp TCP proxy with a public
remote port." Against that baseline, this design removes:

- **Opportunistic scanning/brute-forcing of an SSH port** — there is no port to
  scan on the target at all (loopback-only sshd), and no port to scan on the
  relay that leads to SSH (STCP registers no public listener).
- **Credential-stuffing against SSH directly from the internet** — same reason;
  an attacker would first need to pass frps's token auth, then the STCP
  `secretKey`, before ever reaching sshd's own auth prompt.
- **Exposure surviving a single leaked secret** — token and `secretKey` are
  independent; leaking one doesn't grant access to SSH, only leaking both does.

## What this design does *not* address

Being clear-eyed about this matters as much as the parts that work:

- **It does not harden SSH itself.** sshd's own auth (password vs. key,
  `PermitRootLogin`, fail2ban, etc.) is completely unaffected by any of this.
  If `spectre`'s account has a weak password, this tunnel changes nothing about
  that risk once someone's past the frp layer.
- **It does not protect frps.** The relay is still a single point of failure
  with a permanently open control port. A sufficiently severe frp vulnerability
  on that port (see CVE history below) bypasses everything built on top of it,
  because it's the one thing that can never be hidden.
- **It does not hide that frp is in use.** Port scanning will still reveal
  `162.35.122.112:7000` as open, and frp's handshake has recognizable
  characteristics. A determined adversary can fingerprint "this IP is running
  frp" even if they can't do anything with that information without both
  secrets. Obscurity was never the goal here — the token and secretKey are.
- **It does not provide anonymity.** Every connection to this system is
  traceable: overnode's IP connects to frps's IP, in the clear at the network
  layer (TLS protects payload content, not metadata/endpoints). This is a
  private-access tool, not an anonymity tool — see `FUTURE-APPLICATIONS.md` for
  why that distinction matters.
- **It does not eliminate frps as a trust anchor.** Whoever controls the relay
  VPS controls the only rendezvous point. If that VPS is compromised, an
  attacker sees connection metadata (timing, volume, which named proxies exist)
  even without the secrets, and could potentially disrupt or deny service to
  the tunnel even without breaking into it.

## Known relevant CVEs (as of this writing, frp 0.70.1 in use)

| CVE | Affected | Impact | Status here |
|---|---|---|---|
| CVE-2026-73564 | frp's SSH Tunnel Gateway feature, pre-0.70.1 | Unauthenticated integer overflow crashing frps, taking down all tunnels | Not applicable — SSH Tunnel Gateway feature is not enabled in this config; also fixed in the version in use (0.70.1) |
| Auth bypass via `routeByHTTPUser` | 0.43.0–0.68.0 | HTTP vhost routing access-control bypass | Not applicable — no HTTP/HTTPS proxies used here, TCP/STCP only |
| CVE-2026-110619 | pprof debug endpoints, 0.41.0–0.71.1 | Unauthenticated info leak / profiling trigger via dashboard debug routes | Not applicable — dashboard (`webServer`) is disabled entirely in `frps.toml` |

**Takeaway:** this design's resistance to known CVEs comes largely from feature
minimalism — not enabling the dashboard, not enabling SSH Tunnel Gateway, not
using HTTP vhost routing — rather than any property of STCP itself. Enabling
those features later would reopen this analysis.

## Residual risk surface, ranked

1. **frps availability/compromise** — the one component that must stay public.
   Mitigated by: keeping it patched, no dashboard, `allowPorts` whitelist,
   TLS-forced transport, token auth. Not eliminated: it's still one machine.
2. **Token or secretKey disclosure via the host, not the wire** — both secrets
   live in files on disk, locked at `0600` under a dedicated non-login user.
   The realistic leak vectors are operator error (committing a config to a
   public repo, pasting a real secret into a chat/ticket/doc — note this very
   document deliberately uses placeholders), not network interception.
3. **frpc process compromise on either end** — mitigated substantially by the
   systemd hardening (empty capability set, `ProtectSystem=strict`, dedicated
   unprivileged user), which limits blast radius even if frp itself had a
   remote-code-execution bug.
4. **sshd's own posture** — unaffected by any of the above; still the operator's
   responsibility (key-only auth, fail2ban, monitoring auth logs).

## Documented real-world abuse of this exact tool

The dual-use point above isn't theoretical — frp has its own entry in the MITRE
ATT&CK framework (**S1144**), which tracks it as software actively abused by
intrusion sets, not merely a hypothetical risk. Worth recording what that abuse
looks like mechanically, since it's structurally identical to this project's
own legitimate design.

### The attacker playbook, mechanically

1. Initial compromise of an internal host via a vulnerable internet-facing
   service (commonly IIS or Exchange servers in documented cases).
2. The attacker drops an `frpc` binary on the compromised host and points it at
   an `frps` instance they control.
3. `frpc` dials outbound to that relay — the same "always dials out, never
   listens" property that let this project's `kali` box avoid opening any
   inbound firewall rule is exactly what lets an attacker's tunnel bypass
   egress-focused firewall policy, since most corporate firewalls block
   inbound traffic by default but permit outbound.
4. The attacker now has an encrypted (TLS/STCP) pivot path into the internal
   network, from a relay they fully control, that looks like ordinary outbound
   TLS traffic unless specifically inspected for frp's handshake signature.

### MITRE ATT&CK mapping (S1144)

| Technique | ID | Relevance |
|---|---|---|
| Protocol Tunneling | T1572 | frp tunnels SSH/Unix-socket traffic over TCP between external nodes and internal resources behind NAT/firewalls |
| Proxy / Multi-hop Proxy | T1090 / T1090.003 | frp relays traffic through a public-IP server to internal targets; the client can additionally chain through another proxy for extra hops |
| Symmetric Encryption | T1573.001 | STCP's pre-shared key secures the tunneled traffic — the same feature used throughout this project |
| Asymmetric Encryption | T1573.002 | TLS-only transport mode — also used throughout this project |

### Documented campaigns

- **Chinese state-sponsored intrusion sets** have used frp to maintain
  persistent access and obfuscate C2 traffic across TCP/UDP/HTTP(S), per
  threat-intel tracking of the tool.
- **A campaign against South Korean companies** used initial access via
  vulnerable IIS/Exchange servers, followed by installing frp (or the similar
  LCX/HTran tool) for sustained internal access — frp reported as the more
  commonly used of the two by the actors observed.
- **Indian critical-infrastructure intrusions** have specifically involved frp
  being used by intruders to establish remote access, per MITRE's S1144 record.

### Why this is hard to detect, and what to do about it

Guidance from incident-response practice on this exact tool is explicit:
detection has to be built around the *behavior* — unauthorized, persistent
reverse-proxy or tunneling activity — rather than the string "frp," because
attackers routinely rename the binary or stage it in non-default paths where a
name-based detection rule would miss it entirely. The recommended baseline is
an allowlist/approval process for any reverse-proxy, tunneling, or remote-
access utility on servers and workstations, with alerting on any host that
starts a persistent external connection without a documented business reason
for doing so.

### Direct implication for this project

The `frpc` process now running persistently on `kali`, dialing out to
`162.35.122.112:7000`, is — purely as a network traffic pattern —
indistinguishable from the first stage of the attacker playbook described
above. The entire legitimacy of this setup rests on it being documented,
intentional, and monitored (see `journalctl -u frpc` recommendations earlier
in this file), not on the traffic itself looking any different from malicious
use. Anyone doing network monitoring on this environment should treat this
document as the authoritative record of why this specific outbound connection
exists.

## Operational recommendations going forward

- Rotate `auth.token` and `secretKey` periodically, and immediately if either
  is ever suspected to have touched an insecure channel (chat, email, a repo).
- Monitor `journalctl -u frpc` on both ends for unexpected reconnects or
  authentication failures — a flood of failed STCP visitor attempts would be
  visible there before it's visible anywhere else.
- Keep `frp` version current; this entire analysis is only valid for the
  specific versions and feature set in use today.
- Consider disabling password auth on sshd entirely (key-only) now that the
  network-layer exposure is minimized — this is the next-highest-value step
  that this document doesn't cover in depth, since it's independent of frp.
