# Generalizing the Pattern

The interesting property of this setup isn't "SSH over frp" specifically — it's
that **a rendezvous server can broker a connection between two parties without
ever exposing a publicly connectable service port for the thing being brokered.**
That property generalizes well beyond SSH. Worth writing down while it's fresh,
along with honest limits on how far the generalization actually goes.

## Generalizing beyond SSH

Anything that's normally "a service on a port" can sit behind the same pattern:
an internal admin dashboard, a database, a Jupyter notebook, a Git server, a
second hop to another internal machine. The template is identical — proxy side
registers an STCP (or XTCP) proxy, visitor side presents the matching secret,
nothing is ever publicly listening for the actual service. This is a reasonable
default posture for *any* internal tool a small team needs occasional remote
access to, not just SSH.

## Multiple proxies, multiple visitors — toward a private mesh

A single frps can broker many independent STCP pairings simultaneously, each
with its own `name`/`secretKey`. Nothing stops a design like:

- Several machines, each registering their own named STCP proxy against one
  relay (a "hub")
- Several authorized visitors, each holding only the secrets for the proxies
  they're meant to reach — not a master key to everything

This starts to resemble a lightweight private mesh overlay: a hub-and-spoke
network where the hub never has to know anything about the services it's
brokering access to beyond their names, and no spoke is ever directly reachable
by anything except through the hub, with per-pairing secrets. It's a much
simpler, more centralized structure than real mesh VPN tooling (Tailscale,
Nebula, WireGuard mesh configs), which do peer discovery, key rotation, and
routing in far more sophisticated ways — but the STCP primitive is a legitimate,
minimal building block for the same underlying goal.

**Where this breaks down as a "real" mesh:** frps remains a single, centralized
rendezvous point and a single point of failure — true mesh designs are
specifically trying to avoid exactly that bottleneck. XTCP's P2P hole-punching
helps once a connection is established (traffic stops routing through the
relay), but *discovery and authorization* still flow through one hub. A
genuinely decentralized mesh would need multiple independent rendezvous points
or a distributed discovery mechanism — frp alone doesn't provide that.

## The "anonymous protocol" framing — and why it mostly doesn't hold up

It's tempting to look at "no public listener, secret-gated access" and think
this is adjacent to anonymity-network design (Tor, I2P, mixnets). It's worth
being precise about why it isn't, because conflating *access control* with
*anonymity* is a common and consequential mistake:

- **Every connection here is fully attributable at the network layer.** frps
  sees the real IP of every frpc that connects to it. TLS protects the content
  of the control/data channel, not the fact that `overnode`'s IP talked to
  `162.35.122.112` at a specific time. There is no onion-style multi-hop
  relaying, no traffic mixing, no attempt to decorrelate sender from receiver.
- **Anonymity networks are built around *not trusting* any single relay** —
  Tor's core design assumption is that any one relay might be hostile, so no
  single node ever sees both the client's identity and the destination. This
  design's relay (frps) by contrast sees everything: which proxies exist, which
  visitors connect, timing and volume for all of it. It is a *trusted*
  rendezvous point, not a trust-minimized one.
- **What this design actually buys is access control, not unlinkability.** The
  secret key answers "is this visitor allowed to reach this proxy" — it says
  nothing about hiding *that* a connection happened, or from where.

If genuine traffic anonymity were the goal, the correct tools are the
purpose-built ones (Tor, I2P) — layering frp underneath or alongside them is
plausible as a *transport convenience* (e.g., reaching an internal service
without a public port, while separately routing through Tor for anonymity at
the network layer), but frp itself contributes nothing to the anonymity
property. Worth being precise about this distinction in any write-up that uses
the word "anonymous" — it's doing more rhetorical work than the architecture
actually supports.

## Dual-use honesty

It's worth stating plainly, since it came up earlier in this project's own
security review: this exact pattern — a client behind NAT dialing out to a
relay it controls, registering a secret-gated tunnel back to an internal
service — is structurally identical to how real intrusions use frp for
post-exploitation pivoting. The *mechanism* that makes this a clean, minimal-
attack-surface access tool for a legitimate operator is the same mechanism that
makes frp attractive to someone tunneling out of a network they've compromised.

This isn't a reason not to use the pattern — NAT traversal itself is dual-use in
exactly this way (so is SSH, so is any remote access tool). It is a reason to:

- Keep an inventory of exactly which machines are running frpc and why
- Treat an *unexpected* frpc process or outbound connection to an unfamiliar
  relay IP as a serious incident-response signal, not noise
- Not assume "it's just our internal tool" is a reason to skip monitoring it

## Reasonable near-term extensions of this specific setup

Grounded, incremental ideas that stay within what's already built, rather than
speculative architecture:

- Add a second STCP proxy (e.g., an internal web dashboard) using the same
  relay and a distinct `secretKey`, to validate the "multiple independent
  proxies on one hub" pattern in practice.
- Move from password-based SSH to key-only auth now that network exposure is
  minimized, closing the one layer this project deliberately left untouched.
- Add connection logging/alerting on the relay (not the dashboard — a log
  pipeline) so an unexpected proxy registration or visitor attempt is visible
  without needing to enable frps's web UI, which was intentionally left off.
