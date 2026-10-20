# Secret SSH Tunnel over frp (STCP)

A reverse-proxy-based SSH access setup using [frp](https://github.com/fatedier/frp)'s
Secret TCP (STCP) proxy type, built to eliminate public SSH exposure entirely while
retaining full remote access from a single trusted laptop.

## Scope

This project covers three machines:

| Role | Hostname | Function |
|---|---|---|
| Relay | `xxx.xx.xxx.xxx` (public VPS) | Runs `frps` — the only publicly reachable component |
| Target | `spectre@kali` | Runs `sshd` (loopback-only) + `frpc` in **proxy** role |
| Visitor | `jackal@overnode` | Runs `frpc` in **visitor** role; initiates all connections |

The relay never sees SSH traffic in the clear and never exposes a port that leads
anywhere near SSH. The target never has SSH reachable from its own network
interfaces. The visitor is the only place a human-usable SSH session is ever
reachable, and only on `127.0.0.1`.

## Purpose

Standard SSH port-forwarding (NAT traversal via `ssh -R`, plain frp TCP proxies,
ngrok, etc.) solves "how do I reach a machine behind NAT" by opening a public port
somewhere that forwards to the private service. That public port is then a
standing, scannable, brute-forceable target for as long as the tunnel exists.

This setup instead asks a different question: **can the tunnel exist without a
public port existing at all?** STCP's answer is yes — the relay (`frps`) agrees to
broker a connection between two parties who each independently prove knowledge of
a shared secret, but never opens a listener that an unauthenticated party could
even attempt to connect to. The only public attack surface, permanently, is the
one control port frps needs to register clients in the first place.

## Why this is worth documenting

This is a reasonably niche use of frp — most tutorials stop at the plain TCP proxy
case ("expose port 6000, forward to local port 22"), which is the weaker design.
The STCP pattern deserves to be written down as the default recommendation for
anything sensitive (SSH, admin panels, databases), not an advanced footnote.

## Files in this set

- `ARCHITECTURE.md` — full data-flow walkthrough, component roles, config anatomy
- `SECURITY.md` — threat model, security value, and known weaknesses
- `FUTURE-APPLICATIONS.md` — where this pattern generalizes beyond SSH: mesh
  overlays, anonymity-adjacent designs, and the honest limits of that framing

## Status

Working, verified end-to-end as of 2026-10-20. Both `frpc` instances run as
hardened, unprivileged systemd services (dedicated `frpc` system user, no login
shell, `ProtectSystem=strict`, empty capability set). `sshd` on the target is
bound to loopback only.
