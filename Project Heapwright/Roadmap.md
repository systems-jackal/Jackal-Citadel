# Low-Level Systems & Security Foundations

A self-paced roadmap for understanding how computers really work (CPU, kernel, memory, buffers, the heap) with security as the lens. Built for a backend / DevSecOps developer starting from the fundamentals.

> **Core loop for every project:**
> **Build it simply → Break it on purpose (under ASan) → Understand why → Fix it → Compare (with and without mitigations)**

---

## How this works

- **No weeks, no deadlines.** Each stage ends with a "Ready to move on when..." checklist. Advance when you can honestly tick the boxes.
- **Small sessions beat big ones.** Three 45-minute sessions a week outperform one 5-hour binge.
- **End every session with one line in your notes** ("next: implement free-list coalescing"). Coming back after a gap becomes painless.
- **After a long break, redo the last exercise** before moving forward. It rebuilds context in 15 minutes.
- **Rotate three session modes:** read, build, break.
- **Rough balance:** ~50% fundamentals, ~30% hands-on, ~20% security. The security share grows as you progress.
- **Stay legal.** Practice only on your own code, your own VMs, and intentionally vulnerable platforms (wargames, CTFs).

---

## The mental model

```
Your code (C / Go / Python / Java)
   ↓ compiled / interpreted
Machine instructions (x86-64 / ARM)
   ↓ runs on
CPU (registers, caches, privilege rings)
   ↓ managed by
Kernel (processes, memory, files, network, scheduling)
   ↓ talks to
Hardware (RAM, disk, NIC)
```

Security bugs live at the seams between these layers, where one layer's assumptions don't match the next one's behavior.

---

## Stage 0: Set up your lab

**Do**
- Install Linux (VM, WSL2, or a spare machine).
- Install `gcc`, `gdb`, `make`, `valgrind`, `strace`, `ltrace`.
- Later add `pwndbg` or `GEF` (gdb plugins) and `checksec`.
- Start a git repo with a notes file. Write in your own words.

**Always compile with**

```bash
gcc -Wall -Wextra -g -fsanitize=address,undefined file.c -o file
```

**Ready to move on when**
- [ ] You can compile, run, and step through "hello world" in gdb (`break main`, `run`, `next`, `print`, `quit`).

---

## Stage 1: Relearn C, with a security lens

Rebuild C through small security-flavored tools. Do the basics quickly (you know the logic from backend work), then spend your time on strings, pointers, and dynamic memory.

**Topics, in order:** types and control flow → arrays and strings (null terminator!) → pointers and pointer arithmetic → structs → `malloc`/`free` → input handling (`fgets` vs `gets`, `strcpy` vs `snprintf`).

| # | Project | C concepts | Break it |
|---|---|---|---|
| 1 | **Password strength checker**: scores length, character classes, common-word list | strings, loops, `ctype.h` | 500-char password into a fixed buffer; forget the `\0` |
| 2 | **Hex dumper** (mini `xxd`) | pointers, bytes, file I/O | Read past the end of your buffer and see what leaks |
| 3 | **Login simulator**: `struct user {char name[16]; char pass[16]; int is_admin;}` | structs, `strcmp`, arrays | Overflow `name` into `is_admin`; fix with bounded copies |
| 4 | **Log analyzer**: count failed logins per IP, flag brute-force patterns | `fgets`, dynamic memory, linked list | Huge line with no newline; leak memory; double free |
| 5 | **Session token store**: create, look up, delete tokens on the heap | `malloc`/`free`, pointers | Use a token after deletion (use-after-free) and read the ASan report |
| 6 | **XOR/Caesar encoder** (explicitly *not* real crypto) | byte ops, signed vs. unsigned | Signed `char` bug; wrong output-buffer size; why it's trivially breakable |

*Optional:* an RPN calculator, broken with integer overflow (`INT_MAX + 1`) and stack underflow.

**Run every "break it" under ASan** and read the report line by line: bug type, address, line, who allocated it.

**Ready to move on when you can explain, without notes**
- [ ] What a pointer is, and why `arr[i]` equals `*(arr + i)`.
- [ ] Why a C string is more dangerous than a Python/Java string.
- [ ] What stack vs. heap means in practice.
- [ ] Why `strcpy` is considered unsafe.

**Resources:** Beej's Guide to C (refresher), *Modern C* by Jens Gustedt (deeper), K&R (reference).

---

## Stage 2: How the CPU and machine code work

**Learn**
- Binary, hex, two's complement, endianness.
- Registers: `RIP` (next instruction), `RSP` (stack pointer), `RBP` (base pointer), general purpose.
- The stack, frames, calling convention, where the return address lives.
- Reading basic x86-64 assembly (read, not write).
- Privilege rings: user mode vs. kernel mode.

**Do**
- View compiled code with `objdump -d` or `gcc -S`.
- In gdb: `disassemble`, `info registers`, `x/20gx $rsp`, and `stepi` across a function call.
- Compare `-O0` vs. `-O2` output.

| # | Project | What you learn | Break it |
|---|---|---|---|
| 7 | **Stack visualizer**: print addresses of locals, args, and frame pointer across nested calls | stack layout | Compare your output with gdb |
| 8 | **Your own crackme**: password-check program; find the comparison in assembly | reading assembly | Patch a jump in gdb (your own binary) to see why client-side checks are weak |
| 9 | **Auth flag overflow**: `char buf[16]; int authenticated = 0;` | local variable ordering | Overflow with protections off, then with canaries on; compare |

**Ready to move on when you can**
- [ ] Draw a stack frame from memory (locals, saved base pointer, return address).
- [ ] Explain what happens to `RSP` and `RIP` on `call` and `ret`.
- [ ] Read a 15-line assembly function and say roughly what it does.

**Resource:** CS:APP chapters 2 and 3.

---

## Stage 3: Operating systems and memory

**Learn**
- Processes vs. threads, scheduling basics.
- User vs. kernel mode and **system calls** (every syscall is an attack surface).
- **Virtual memory**: pages, page tables, per-page R/W/X permissions, page faults.
- Process address space: text, data/bss, heap, mmap region, stack.
- File descriptors, permissions, users and groups, `setuid`.
- **Heap allocators**: chunks, metadata, free lists/bins, reuse behavior.

**Do**
- Read `/proc/<pid>/maps` for a running program and label each region.
- `strace` `ls`, `cat`, and your own programs; match each syscall to what the program did.
- Print stack/heap/libc addresses across runs with ASLR on and off.
- Trigger a segfault three ways (NULL deref, write to read-only memory, stack exhaustion) and explain each in terms of pages and permissions.

| # | Project | What you learn | Break it |
|---|---|---|---|
| 10 | **Memory map printer + ASLR tester** (reads `/proc/self/maps`) | address space, ASLR | Compare runs with ASLR on and off |
| 11 | **Mini shell**: `fork` + `execve` + a few built-ins | processes, syscalls | Use `system()` with user input (command injection); fix with `execve` and argument arrays |
| 12 | **File permission auditor**: find world-writable files and setuid binaries | `stat`, permissions | Build a symlink case that fools a naive check |
| 13 | **TOCTOU demo**: `access()` then `open()`, with a race script in your own temp dir | race conditions | Swap the file between check and use; fix by opening first, then checking the descriptor |
| 14 | **Toy heap allocator**: free-list `my_malloc`/`my_free` on a static array, metadata beside each chunk | how allocators work | Overflow into the next chunk's metadata; double free; use-after-free. **The single best project for understanding heap bugs.** |

**Ready to move on when you can explain**
- [ ] What happens, step by step, when a program calls `read()`.
- [ ] Why two processes can use the same virtual address without clashing.
- [ ] What a page fault is, and when one is normal vs. fatal.
- [ ] How freed memory gets reused by later allocations (and why that enables heap grooming).

**Resources:** *Operating Systems: Three Easy Pieces* (free), CS:APP virtual memory chapters, *The Linux Programming Interface* (reference).

---

## Stage 4: Vulnerability classes and mitigations

Learn each class as **concept → minimal vulnerable program → fix → which mitigation affects it**.

**Bug classes:** stack overflow · heap overflow · use-after-free / double free · integer overflow into wrong allocation size · format string · off-by-one · uninitialized memory / info leaks · race conditions (TOCTOU) · type confusion (more in C++).

**Mitigations:** NX/DEP, ASLR, PIE, stack canaries, RELRO, CFI / shadow stacks, allocator hardening (safe-unlinking, tcache checks), seccomp / sandboxing. For each, know what it stops and what it doesn't.

| # | Project | Bug class |
|---|---|---|
| 15 | **Array-of-records allocator**: `malloc(count * size)` with attacker-controlled values | integer overflow |
| 16 | **Naive logger**: `printf(user_input)` | format string, info leak |
| 17 | **Length-prefixed echo server on localhost** that trusts the length | over-read (the idea behind Heartbleed) |
| 18 | **Off-by-one string builder** | off-by-one, null terminator |
| 19 | **Config parser** reading `key=value` lines | realistic fuzzing target for Stage 5 |

For each: run `checksec`, compile with mitigations on and off, and write 5 sentences on what changed.

**Practice platforms:** OverTheWire (Bandit → Narnia), pwn.college, how2heap (after Stage 3), Nightmare. Read 1-2 CVE write-ups every couple of weeks, even if you only half understand them at first.

**Ready to move on when you can**
- [ ] Look at a short vulnerable C snippet and name the bug class.
- [ ] Explain why a bug is or isn't exploitable under specific mitigations.
- [ ] Describe, conceptually, how an info leak plus ROP defeats ASLR and NX.

---

## Stage 5: The defender's toolkit (your DevSecOps edge)

This overlaps with Stage 4 and is where your career value is.

- **Sanitizers:** ASan, UBSan, MSan, TSan.
- **Fuzzing:** AFL++, libFuzzer.
- **Static analysis:** Clang static analyzer, cppcheck, CodeQL. Triage critically; learn to spot false positives.
- **Hardening flags:** `-fstack-protector-strong`, `-D_FORTIFY_SOURCE=2`, `-fPIE -pie`, `-Wl,-z,relro,-z,now`.
- **Supply chain:** SBOMs, patching cadence, reading advisories.
- **Memory-safe languages:** why Rust and Go change the risk profile, and where `unsafe` brings it back.

| # | Project | Goal |
|---|---|---|
| 20 | **Fuzz your config parser (#19)** | find your own crash, minimize it, fix the root cause |
| 21 | **Secure rewrite** of 2 earlier projects | bounds checks, safe APIs, hardened flags; show before/after under ASan and fuzzing |
| 22 | **CI pipeline** (GitHub Actions) | sanitizer build + short fuzz run + static analysis as a security gate |
| 23 | **Rust port** of the login simulator or log analyzer | see which of your bugs can't be written |

**Ready to move on when you can**
- [ ] Set up CI that builds a C project with sanitizers, fuzzes it for a few minutes, and fails on findings.

---

## Stage 6: Widening out (pick by interest)

- **C++ for security:** object lifetime, copy/move, RAII, vtables, iterator invalidation, type confusion (where browser and database CVEs live).
- **Rust:** ownership and borrowing as compile-time memory safety.
- **CPU internals:** pipelines, caches, speculative execution (Spectre/Meltdown), and hardware fault injection (voltage/clock glitching, linked to overclocking).
- **Kernel basics:** a tiny Linux kernel module in a VM, how syscalls enter the kernel, a simple kernel CVE.
- **Reverse engineering:** Ghidra, reading unknown binaries.

---

## Project conventions

- **Keep projects small.** Past ~150 lines, you're over-building. The learning is in the break step.
- **Each project gets a `NOTES.md`:** what the bug was, why it happened, how you fixed it, which mitigation would catch it.
- **Suggested repo layout**

```
lowlevel-sec/
├── stage1/
│   ├── 01-password-checker/
│   │   ├── vuln.c
│   │   ├── fixed.c
│   │   └── NOTES.md
│   └── ...
├── stage2/
├── stage3/
├── stage4/
└── stage5/
```

- **Your repo becomes a portfolio** that shows real low-level security understanding.

---

## Habits that keep it sustainable

- **Teach it back.** After each topic, write a 5-sentence explanation as if for a colleague. If you can't, you haven't got it yet.
- **One "break it" per program.**
- **Don't skip stages** to reach the exciting parts. Heap exploitation without Stage 3 is memorized recipes.
- **Keep a "things that confused me" list** and revisit it monthly.
- **Understand, don't memorize.** If you can't explain *why* ASLR helps in one sentence, revisit it.

---

## Progress tracker

- [ ] **Stage 0:** Lab works, gdb basics
- [ ] **Stage 1:** C relearned (projects 1-6)
- [ ] **Stage 2:** Stack and assembly reading (projects 7-9)
- [ ] **Stage 3:** Virtual memory, syscalls, allocator (projects 10-14)
- [ ] **Stage 4:** Vulnerability classes and mitigations (projects 15-19), first wargames
- [ ] **Stage 5:** Sanitizers, fuzzing, CI (projects 20-23)
- [ ] **Stage 6:** Chosen specialization

---

## Resource index

| Topic | Resource |
|---|---|
| C refresher | Beej's Guide to C; *Modern C* (Gustedt); K&R |
| Systems foundation | *Computer Systems: A Programmer's Perspective* (CS:APP) |
| Operating systems | *Operating Systems: Three Easy Pieces* (free) |
| Syscalls reference | *The Linux Programming Interface* (Kerrisk) |
| Exploitation concepts | *Hacking: The Art of Exploitation* (Erickson) |
| Hands-on practice | OverTheWire, pwn.college, how2heap, Nightmare |
| Tooling | gdb + pwndbg/GEF, objdump, readelf, strace, valgrind, ASan/UBSan, AFL++, libFuzzer, Ghidra |

---

## Your very first session

Write a C program that reads a name into a 16-byte buffer using `strcpy`. Compile with `-fsanitize=address -g`, feed it 100 characters, and read the ASan report fully. Then fix it with `fgets` and `snprintf`. That one exercise touches arrays, strings, buffers, and tooling, and it's the seed of everything else.
