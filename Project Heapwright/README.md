# Project Heapwright

A hands-on memory-corruption research track: from C fundamentals, through stack and heap
exploitation basics, up to understanding **AnyPwn** — a real, disclosed pre-authentication
root RCE in AnyDesk Linux 8.0.2 — as a worked case study.

## Scope & disclaimer

This repository is for learning and defensive research only. It:

- Contains **original, self-contained toy programs** written to illustrate bug classes —
  never code targeting any real, currently-exploitable product.
- Discusses the AnyPwn vulnerability **conceptually**, based on public disclosures, as a
  case study for understanding the *pattern* behind it. It does not include, reproduce, or
  adapt any working exploit against AnyDesk or any other real software.
- Assumes everything here is run inside an isolated VM or container you control. Several
  examples deliberately disable compiler hardening (stack protectors, `_FORTIFY_SOURCE`) to
  make a vulnerability's mechanics visible — never disable these in anything you ship.

If you're looking for a working AnyPwn exploit: it isn't here, and won't be added. See
[Further reading](#further-reading) for the public disclosure instead.

## Prerequisites

- Linux (a VM is fine — Ubuntu 22.04/24.04 works well)
- `gcc`, `gdb`, `valgrind`
- Some prior exposure to C syntax is assumed; prior comfort with **pointers and memory** is
  not — that's exactly what Module 0 rebuilds.

```bash
sudo apt install build-essential gdb valgrind
```

---

## Module 0 — C fundamentals refresher

It's been a while, so this module exists purely to shake the rust off before anything
"security" shows up.

**Warm-up**
- Write a program that declares an `int`, prints its value *and* its address with `&`.
- Write a function that takes a pointer parameter, modifies the value through it, and
  confirms the caller sees the change.
- Do basic pointer arithmetic: declare an `int arr[5]`, walk it with a pointer instead of
  an index, and print each address to see the stride (should be 4 bytes apart on a typical
  system).
- Compile with `gcc -g -O0 -Wall` and get used to reading compiler warnings — most memory
  bugs throw one.

---

## Module 1 — C memory model

### Stack vs heap

A running program's memory splits into regions with different lifetimes and management
rules: code, global/static data, the **heap** (manual, via `malloc`/`free`, grows upward),
and the **stack** (automatic, grows downward, torn down when a function returns).

**Warm-up**
- Declare a local variable and a `malloc`'d variable in the same function. Print both
  addresses. Note which is numerically higher — on most Linux x86_64 setups the stack
  address will be far higher than the heap address.
- Return a pointer to a local stack variable from a function and try to use it afterward.
  Compile with `-Wall` and read the warning carefully — this is a real, common bug class
  (dangling pointer to freed stack memory) in its own right.

### Pointers and addresses

The minimum you need going forward: what `&x` and `*p` actually do, the difference between
a pointer's own address and the address it stores, and why a wrong pointer value is just a
number the CPU will trust completely.

**Warm-up**
- Write a tiny "pointer swap" function (`void swap(int *a, int *b)`) and verify it actually
  swaps the caller's variables — classic pass-by-address practice.
- Deliberately write a NULL pointer dereference (`int *p = NULL; *p = 5;`), run it, and
  read the segfault in `dmesg` or under `gdb` to see what a hardware memory-protection fault
  actually looks like.

### Function calls

Every call pushes a stack frame: the return address (where to resume after the call),
sometimes a saved base pointer, then the callee's locals.

**Warm-up**
- Compile a small program with two nested function calls, then step through it in `gdb`
  with `disas` and `info frame` at a breakpoint inside the inner function. Identify the
  return address on the stack with `x/8xg $rsp`.

---

## Module 2 — Stack exploitation

### Stack buffer overflow

Writing past the end of a stack-allocated buffer overwrites *whatever sits next to it* —
other locals, saved registers, eventually the saved return address.

**Warm-up**
- Write a function with two adjacent local variables: a small `char buf[8]` and an
  `int flag = 0`. Use `strcpy` to copy an oversized attacker-style string into `buf` and
  observe `flag` change value afterward, purely from the overflow. (This mirrors the heap
  version of the same idea in `labs/` — see Module 3.)

### Overwriting saved return addresses

The classic next step: if the overflow reaches far enough, it overwrites the return
address itself, redirecting execution when the function returns.

**Warm-up (the standard "ret2win" pattern)**
- Write a program with two functions: `vulnerable()`, containing an oversized `strcpy`
  into a small stack buffer, and `win()`, an unreachable function that just prints a
  success message. Compile with protections off
  (`-fno-stack-protector -z execstack -no-pie`).
- Find the offset from the start of the buffer to the saved return address using a
  cyclic/de Bruijn pattern (`gdb`'s `pwndbg` plugin has `cyclic` built in, or use
  `pwntools`'s `cyclic()` from Python).
- Craft an input of exactly that many junk bytes followed by `win()`'s address (read with
  `objdump -d` or `gdb`'s `print win`), and confirm the program jumps into `win()` instead
  of returning normally.
- This is the whole exercise — no shellcode, no real target, just proving to yourself that
  overwriting a return address changes where execution resumes.

### Stack canaries

A canary is a random value placed between locals and the saved return address, checked
before the function returns; a smashed canary aborts the program instead of letting a
corrupted return address run.

**Warm-up**
- Compile the same `vulnerable()` program twice: once with `-fno-stack-protector`, once
  with the default (canary enabled). Run your Module 2 overflow against both and compare —
  the canary build should abort with a "stack smashing detected" message instead of
  redirecting execution.

---

## Module 3 — Heap exploitation

### How malloc/free work internally

Heap allocators track chunks via metadata stored alongside each allocation, and reuse
freed chunks via free lists. Nothing here prevents you from writing where you shouldn't —
the allocator does less safety-checking than people assume.

**Warm-up**
- Allocate several small objects with `malloc`, print their addresses, `free` them in a
  different order than you allocated them, then allocate new objects and see which freed
  slots get reused. This is the first taste of why allocation order matters later.

### Heap buffer overflow

Same idea as the stack version, different neighborhood: writing past a `malloc`'d buffer
corrupts whatever object (or allocator metadata) happens to sit next to it.

**Warm-up**
- See `labs/heap_overflow_demo.c` — a self-contained program demonstrating exactly this: a
  tiny heap buffer, a neighboring "trusted" struct, and a deliberate overflow that flips a
  value in that struct purely by writing past the buffer's real size. Compile per the
  comment at the top of the file, run it, then re-run it under `gdb` and `valgrind` to see
  the corruption two different ways.

### Heap grooming

Adjacency on the heap isn't guaranteed — attackers often have to allocate and free objects
in a specific sequence first, to force a useful target object to land right next to the
buffer they're about to overflow.

**Warm-up**
- Extend the heap overflow demo: allocate a few extra filler objects of varying sizes
  between the vulnerable buffer and the target struct, `free` some of them, and observe how
  the layout — and therefore what your overflow actually reaches — shifts. This is the
  entire intuition behind "heap grooming," without anything resembling a real exploit.

---

## Module 4 — AnyPwn case study

**AnyPwn** is a real, publicly disclosed vulnerability (V12 Security / Rick de Jager,
October 2026): a pre-authentication, zero-click root RCE in AnyDesk Linux 8.0.2, fixed in
8.0.3. It's used here purely as a motivating, real-world example of the bug classes above
chaining together. No exploit code for it lives in this repo.

### Integer overflow

The vulnerable handler added a 16-byte header to an attacker-declared length using 32-bit
arithmetic. A declared length near `0xFFFFFFFF` plus that header wraps around past the
integer's maximum value, back down to a tiny number.

**Warm-up**
- Write a one-line demonstration: `uint32_t x = 0xfffffff0; printf("%u\n", x + 16);` and
  watch it print `0`, not the ~4.3 billion you might expect. This single line *is* the root
  cause of AnyPwn's memory corruption.

### Size-vs-copy mismatch

The bug isn't the overflow arithmetic alone — it's that the **allocation** used the
wrapped (tiny) size while the **copy** later used the original (huge) attacker-supplied
length. Two different numbers, derived from the same tainted input, computed at two
different points in the code, never reconciled.

**Warm-up**
- This is exactly what `labs/heap_overflow_demo.c` models: `vulnerable_compute_alloc_size()`
  does the wrapping arithmetic, `malloc` uses that wrapped result, and the `memset` later
  uses a separate, larger figure to simulate the mismatch. Read through the file with this
  framing in mind, rather than as a standalone heap bug.

### Unauthenticated vs. pre-authenticated attack surface

The handler that contained this bug ran during session *setup*, before any login or
user-approval step — meaning the vulnerable code was reachable by anyone who could reach
the service on the network, with zero credentials and zero clicks from the victim. This is
why the impact was so severe despite being "just" a memory corruption bug: there's no
authentication gate standing between an attacker and the vulnerable code path.

**Warm-up (no code — a reading/analysis exercise)**
- Pick a network service you have legitimate access to test (something you run yourself).
  Using Wireshark, identify which parts of its protocol are exchanged *before* any
  authentication step. That pre-auth surface is, structurally, the same category of
  exposure that made AnyPwn so severe — the lesson transfers even without touching AnyDesk
  at all.

---

## Repository structure

```
.
├── README.md
├── 00-c-fundamentals/
├── 01-memory-model/
├── 02-stack-exploitation/
│   └── ret2win_demo.c
├── 03-heap-exploitation/
│   └── heap_overflow_demo.c
├── 04-anypwn-case-study/
│   └── notes.md
└── labs/
```

## Further reading

- [AnyDesk Linux Flaw Lets Remote Attackers Execute Code as Root Without Authentication](https://cybersecuritynews.com/anydesk-linux-vulnerability/)
- [AnyDesk Vulnerability Allowing Remote Code Execution With Root Privileges](https://gbhackers.com/anydesk-vulnerability/)
- V12 Security's public writeup and PoC repository (referenced for awareness only — not
  reproduced here)

## License / use

Educational use only. Nothing in this repository targets, or is intended to be adapted to
target, any real production system.
