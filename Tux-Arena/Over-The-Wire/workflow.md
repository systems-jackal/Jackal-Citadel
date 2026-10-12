# Work Flow
```WorkFlow
How to actually use these (workflow)
Don't look up the answer first. Spend at least 15–20 minutes actually trying — re-read relevant man/tldr pages, think about what the level is hinting at. This struggle is the entire point.
If stuck after genuine effort, it's fine to search (e.g. "bandit level 7 walkthrough") — but read why the solution works, don't just paste the command.
Document every level, even ones that felt trivial — see format below. Skipping documentation on "easy" ones is exactly how you lose the value of having done them.
Don't binge levels. A few a day, with notes, beats speed-running the whole wargame in one sitting with nothing retained.
Documentation format (one entry per level)
```
Keep this consistent so your notes are scannable later:

## Bandit Level 4 → 5
**Goal**: find the password hidden among several files, only one of which is human-readable.
**Commands used**: `file *`, `cat`
**What I learned**: `file` identifies actual file type regardless of extension —
several files were named misleadingly but only one was plain ASCII text.
**Solution**: had to exclude non-printable files first; `cat`-ing a binary file
filled the terminal with garbage.
