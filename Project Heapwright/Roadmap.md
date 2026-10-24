<div align="center">

```text
  ╔══════════════════════════════════════════════════════════════╗
  ║                                                              ║
  ║    ██╗      ██████╗ ██╗    ██╗    ██╗     ██╗   ██╗██╗       ║
  ║    ██║     ██╔═══██╗██║    ██║    ██║     ██║   ██║██║       ║
  ║    ██║     ██║   ██║██║ █╗ ██║    ██║     ██║   ██║██║       ║
  ║    ██║     ██║   ██║██║███╗██║    ██║     ╚██╗ ██╔╝██║       ║
  ║    ███████╗╚██████╔╝╚███╔███╔╝    ███████╗ ╚████╔╝ ███████╗  ║
  ║    ╚══════╝ ╚═════╝  ╚══╝╚══╝     ╚══════╝  ╚═══╝  ╚══════╝  ║
  ║                                                              ║
  ║                      PROJECT HEAPWRIGHT                      ║
  ╚══════════════════════════════════════════════════════════════╝
```

# ⚡ The 10-Week Low-Level Systems & C Roadmap

**One concept per week. One lab per concept. One morning review to make it stick.**

![weeks](https://img.shields.io/badge/duration-10_weeks-89b4fa?style=for-the-badge)
![pace](https://img.shields.io/badge/pace-self--paced-a6e3a1?style=for-the-badge)
![lang](https://img.shields.io/badge/language-C-f38ba8?style=for-the-badge)
![os](https://img.shields.io/badge/os-Linux-fab387?style=for-the-badge)
![track](https://img.shields.io/badge/track-Secure_Development-cba6f7?style=for-the-badge)

*From `hello.c` all the way down to CPU registers. 🐧*

</div>

---

## 📖 Table of Contents

- [🧭 How This Roadmap Works](#-how-this-roadmap-works)
- [🌅 The Morning Review Ritual](#-the-morning-review-ritual)
- [🧰 Setup: Your Lab Rig](#-setup-your-lab-rig)
- [🗺️ The Map](#️-the-map)
- [📍 Weeks 1–10](#-week-1--the-compilation-lifecycle)
- [📋 Master Cheat Sheet](#-master-cheat-sheet)
- [📚 Glossary](#-glossary)
- [🏁 After Week 10](#-after-week-10)

---

## 🧭 How This Roadmap Works

You don't need to rush. Each week is **one idea**, small enough to finish in a week and deep enough to change how you see computers.

```text
   ┌────────────┐   ┌────────────┐   ┌────────────┐   ┌────────────┐
   │  🧠 LEARN  │ → │  🔬 BUILD  │ → │  🎯 BREAK  │ → │  🌅 REVIEW │
   │  the idea  │   │  the lab   │   │  the limits│   │  next week │
   └────────────┘   └────────────┘   └────────────┘   └────────────┘
```

### The Weekly Rhythm (suggested, bend it freely)

| Day | Focus | Time | What you do |
|-----|-------|------|-------------|
| 🟦 **Day 1** | **Read** | 30–45 min | Read the *Big Picture* and *Mental Model*. Don't code yet. Just absorb. |
| 🟩 **Day 2–3** | **Build** | 1 hr/day | Do the *Practical Lab*. Type every command yourself. No copy-paste. |
| 🟨 **Day 4–5** | **Explore** | 1 hr/day | Tackle the *Self-Discovery Challenge*. Get stuck. Google. Get unstuck. |
| 🟧 **Day 6** | **Write** | 20 min | Write 5 sentences in your notes: *"This week I learned…"* |
| 🟥 **Day 7** | **Rest** | 0 min | Seriously. Your brain consolidates while you're away. |

> 💡 **Fell behind?** Stay on the same week. Nothing here has a deadline. Mastery beats speed.

### 🏆 Progress Tracker

Tick these off as you go (works in GitHub / Obsidian / most Markdown editors):

- [ ] Week 1 · Compilation Lifecycle
- [ ] Week 2 · Pointers & Memory Addresses
- [ ] Week 3 · Process Memory Segments
- [ ] Week 4 · File Descriptors & Syscalls
- [ ] Week 5 · Identity & Privileges
- [ ] Week 6 · Dynamic Linking
- [ ] Week 7 · Network Sockets
- [ ] Week 8 · Packet Parsing
- [ ] Week 9 · Memory Boundaries
- [ ] Week 10 · Debugging with GDB

---

## 🌅 The Morning Review Ritual

This is the secret weapon. **At the start of each new week, spend 10–15 minutes (coffee in hand ☕) reviewing the previous week before touching anything new.**

```text
  ☕ 06:30  Open terminal
  📜 06:32  Read last week's "Morning Review" flashcards, answer OUT LOUD first
  ⌨️ 06:38  Re-run last week's lab from memory (no notes!)
  ✅ 06:45  Peek at the answers, fix gaps, then start the new week
```

Why it works: recalling something *after a gap* is what moves it from short-term to long-term memory. Every week below ends with a **🌅 Morning Review** block. Cover the answer, say it aloud, *then* expand it.

---

## 🧰 Setup: Your Lab Rig

> ⚠️ **Use a virtual machine or a throwaway container for these labs.** You'll be setting SUID bits, preloading libraries, and deliberately crashing programs. Treat your lab like a lab.

**Recommended:** Ubuntu or Debian in a VM (VirtualBox, QEMU, UTM) or WSL2.

```bash
sudo apt update
sudo apt install -y build-essential gdb strace ltrace xxd \
                    netcat-openbsd tcpdump net-tools iproute2 \
                    binutils file man-db manpages-dev
```

**Make a workspace:**

```bash
mkdir -p ~/lowlevel/{week01,week02,week03,week04,week05,week06,week07,week08,week09,week10}
cd ~/lowlevel
```

**Your best friend: `man`.** Every function in this roadmap has a manual page.

```bash
man 2 open      # section 2 = system calls
man 3 printf    # section 3 = library functions
man 7 socket    # section 7 = overviews
```

---

## 🗺️ The Map

```text
   HIGH LEVEL  ┌──────────────────────────────────────────┐
      ▲        │  Week 1   C source → ELF binary          │
      │        │  Week 2   Pointers & addresses           │
      │        │  Week 3   Stack / Heap / Data / Code     │
      │        │  Week 4   File descriptors & syscalls    │
      │        │  Week 5   Users, UIDs & SUID             │
      │        │  Week 6   Shared libraries & LD_PRELOAD  │
      │        │  Week 7   Sockets                        │
      │        │  Week 8   Raw bytes & packet headers     │
      │        │  Week 9   Buffer boundaries              │
      ▼        │  Week 10  GDB, registers, assembly       │
   LOW LEVEL   └──────────────────────────────────────────┘
```

---

# 🛠️ Week 1 · The Compilation Lifecycle

> **🧩 Big Picture:** Code is just text until a compiler turns it into CPU instructions. Today you watch that transformation happen, one stage at a time.

### 🧠 Mental Model

```text
 hello.c ──► [Preprocessor] ──► hello.i ──► [Compiler] ──► hello.s
 (text)       expands #include   (still C)   C → assembly   (assembly text)

 hello.s ──► [Assembler] ──► hello.o ──► [Linker] ──► hello
 (text)       asm → machine   (object)    joins libs   (ELF executable)
```

**ELF** = *Executable and Linkable Format*, the file format Linux uses for programs.

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `main()`, functions, `#include`, `printf` |
| **Linux** | `ls`, `cd`, `mkdir`, `file`, `size`, `strip` |

### 🔬 Practical Lab

**1. Write `hello.c`:**

```c
#include <stdio.h>

int main(void) {
    printf("Hello, low-level world!\n");
    return 0;
}
```

**2. Run each compiler stage by hand:**

```bash
gcc -E hello.c -o hello.i    # 1. Preprocess  → open it, it's HUGE. Why?
gcc -S hello.i -o hello.s    # 2. Compile     → human-readable assembly
gcc -c hello.s -o hello.o    # 3. Assemble    → machine code (not runnable yet)
gcc hello.o   -o hello       # 4. Link        → final ELF binary
```

**3. Inspect your creation:**

```bash
file hello         # What kind of file is it?
size hello         # Sections: text, data, bss
ls -lh hello.c hello
head -c 4 hello | xxd   # Spot the magic bytes: 7f 45 4c 46 = ".ELF"
```

### 🎯 Self-Discovery Challenge

1. Compare the size of `hello.c` and `hello`. Why is the binary so much bigger?
2. Run `strip hello`, then `ls -lh` again. **What did `strip` remove, and why does the program still run?**
3. 🌶️ *Bonus:* Run `readelf -h hello` and find the "Entry point address". Is it `main`?

### 🌅 Morning Review

<details><summary><b>Q1.</b> Name the four stages of GCC compilation in order.</summary>

Preprocessing → Compiling → Assembling → Linking.
</details>

<details><summary><b>Q2.</b> What does the preprocessor actually do to <code>#include</code>?</summary>

It pastes the contents of the header file into your source, as plain text, before compilation begins.
</details>

<details><summary><b>Q3.</b> What does <code>strip</code> remove?</summary>

The symbol table and debug info (function/variable names). The CPU never needed them, so the program still runs. Tools like debuggers do.
</details>

---

# 📍 Week 2 · Pointers & Memory Addresses

> **🧩 Big Picture:** A variable is a human nickname for a spot in RAM. A **pointer** is a variable that stores that spot's address.

### 🧠 Mental Model

```text
   Address        Value        Name
  ┌──────────┬───────────┐
  │ 0x1000   │    10     │   arr[0]   ◄── p
  │ 0x1004   │    20     │   arr[1]   ◄── p + 1   (moved 4 bytes!)
  │ 0x1008   │    30     │   arr[2]   ◄── p + 2
  │ 0x100C   │    40     │   arr[3]   ◄── p + 3
  └──────────┴───────────┘
   `p + 1` moves by sizeof(int), not by 1 byte.
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `&` (address-of), `*` (dereference), pointer arithmetic, type casting |
| **Linux** | `xxd`, `hexdump` |

### 🔬 Practical Lab

```c
#include <stdio.h>

int main(void) {
    int arr[4] = {10, 20, 30, 40};
    int *p = arr;

    for (int i = 0; i < 4; i++) {
        printf("addr=%p  value=%d\n", (void *)(p + i), *(p + i));
    }
    return 0;
}
```

Run it and **look at the addresses**: they should differ by `4`. That's `sizeof(int)`.

### 🎯 Self-Discovery Challenge

1. Cast to `unsigned char *` and print the array **byte by byte** in hex:
   ```c
   unsigned char *b = (unsigned char *)arr;
   for (int i = 0; i < 16; i++) printf("%02x ", b[i]);
   ```
2. The number `10` is `0x0000000A`. Do you see `0a 00 00 00` or `00 00 00 0a`? **That tells you your system's endianness.** (Little-Endian = least significant byte first.)
3. Confirm with `lscpu | grep "Byte Order"`.
4. 🌶️ *Bonus:* Dump the same bytes from a file with `xxd`.

### 🌅 Morning Review

<details><summary><b>Q1.</b> What's the difference between <code>&x</code> and <code>*p</code>?</summary>

`&x` gives the **address** of `x`. `*p` follows pointer `p` to get the **value** stored at that address.
</details>

<details><summary><b>Q2.</b> If <code>p</code> is an <code>int *</code> at <code>0x1000</code>, what is <code>p + 2</code>?</summary>

`0x1008`. Pointer arithmetic scales by the size of the pointed-to type (4 bytes × 2).
</details>

<details><summary><b>Q3.</b> What does Little-Endian mean?</summary>

The least significant byte is stored at the lowest address. `0x0000000A` is stored as `0a 00 00 00`.
</details>

---

# 🏗️ Week 3 · Process Memory Segments

> **🧩 Big Picture:** Every running program gets its own private, virtual map of memory, divided into regions with different jobs.

### 🧠 Mental Model

```text
  High addresses
  ┌─────────────────────┐
  │       STACK         │  ← local variables, function calls (grows ↓)
  │          ↓          │
  │                     │
  │          ↑          │
  │        HEAP         │  ← malloc() memory (grows ↑)
  ├─────────────────────┤
  │  .bss  (uninit gv)  │  ← global vars with no initial value
  │  .data (init gv)    │  ← global vars with initial value
  ├─────────────────────┤
  │  .text (CODE)       │  ← your machine instructions (read-only)
  └─────────────────────┘
  Low addresses
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | Stack variables, `malloc()` / `free()`, global & `static` variables |
| **Linux** | `/proc/[PID]/maps`, `pgrep`, `top`, `free -m` |

### 🔬 Practical Lab

```c
#include <stdio.h>
#include <stdlib.h>

int global_var = 42;                       // .data

int main(void) {
    int local_var = 7;                     // stack
    char *heap_buf = malloc(64);           // heap

    printf("global (data):  %p\n", (void *)&global_var);
    printf("local  (stack): %p\n", (void *)&local_var);
    printf("heap   (heap):  %p\n", (void *)heap_buf);
    printf("code   (.text): %p\n", (void *)main);

    printf("Press Enter to exit...\n");
    getchar();                             // pause here!

    free(heap_buf);
    return 0;
}
```

In a **second terminal** while it's paused:

```bash
cat /proc/$(pgrep -n memdemo)/maps
```

Match each printed address to a range labelled `[stack]`, `[heap]`, or your binary's name. 🎯

### 🎯 Self-Discovery Challenge

1. Write a loop that calls `malloc(1024*1024)` forever with no `free()`. Watch memory in another terminal with `top` or `watch free -m`.
   > ⚠️ Put a cap on it (e.g., stop after 500 iterations) or be ready with `Ctrl+C`.
2. Run the program twice. Do the **stack addresses change** between runs? (Hint: ASLR. Look up `/proc/sys/kernel/randomize_va_space`.)

### 🌅 Morning Review

<details><summary><b>Q1.</b> Which segment holds local variables? Which holds <code>malloc()</code> memory?</summary>

Locals → **stack**. `malloc()` → **heap**.
</details>

<details><summary><b>Q2.</b> What's a memory leak?</summary>

Memory allocated on the heap that's never freed, so the process keeps consuming RAM it can no longer use.
</details>

<details><summary><b>Q3.</b> What does <code>/proc/PID/maps</code> show?</summary>

The virtual memory regions of a process: address ranges, permissions (`r-x`, `rw-`), and what's mapped there.
</details>

---

# 📁 Week 4 · File Descriptors & System Calls

> **🧩 Big Picture:** In Linux, "everything is a file", and every open file is just a small integer the kernel hands you. Programs ask the kernel to do real work through **system calls**.

### 🧠 Mental Model

```text
   Your program
        │   write(1, "hi\n", 3)
        ▼
  ════ system call boundary ════
        │
   Linux kernel ──► terminal / file / socket / device

   FD 0 = stdin     FD 1 = stdout     FD 2 = stderr
   Next open()  →  FD 3, 4, 5 ...
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `fopen`/`printf` (library) vs. `open`/`read`/`write`/`close` (raw syscalls) |
| **Linux** | FDs 0/1/2, redirection (`>`, `2>`, `|`), `strace` |

### 🔬 Practical Lab

**1. Create a file using raw syscalls:**

```c
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    const char *msg = "written by a raw syscall\n";
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, msg, strlen(msg));
    close(fd);
    return 0;
}
```

**2. Now change `fd` to `1`** (skip `open`). Where does your text go? 👀

**3. Play with redirection:**

```bash
./prog > out.txt          # stdout → file
./prog 2> errors.txt      # stderr → file
./prog > /dev/null        # stdout → the void
```

### 🎯 Self-Discovery Challenge

1. Run `strace ./prog` and **find your `write` call** in the output.
2. Count how many syscalls happen **before** `main()` runs. What are they doing? (Spoiler: `execve`, `mmap`, `openat` for libc…)
3. Try `strace -c ./prog` for a summary table.
4. 🌶️ *Bonus:* Compare `strace` output of a `printf` program vs. a raw `write` program.

### 🌅 Morning Review

<details><summary><b>Q1.</b> What are file descriptors 0, 1, and 2?</summary>

`stdin`, `stdout`, `stderr`.
</details>

<details><summary><b>Q2.</b> Why is <code>write(1, ...)</code> the same as printing to the terminal?</summary>

FD 1 *is* standard output, which is connected to your terminal by default.
</details>

<details><summary><b>Q3.</b> What does <code>strace</code> show you?</summary>

Every system call a program makes to the kernel, with arguments and return values.
</details>

---

# 🔐 Week 5 · Identity, Privileges & SUID

> **🧩 Big Picture:** Linux decides what a process may do by comparing *who it is running as* against file permission bits. SUID lets a program temporarily act as its **file's owner**.

### 🧠 Mental Model

```text
  Real UID (ruid)       = who actually launched the program
  Effective UID (euid)  = whose permissions the kernel checks

  Normal program:   ruid = 1000   euid = 1000
  SUID-root program: ruid = 1000   euid = 0   ← runs with root's powers

  Permission bits:   rwx r-x r-x          (owner / group / others)
  With SUID:         rws r-x r-x          (the 's' replaces the owner's 'x')
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `getuid()`, `geteuid()` |
| **Linux** | `chmod`, `chown`, `ls -l`, SUID bit, octal modes |

### 🔬 Practical Lab

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Real UID:      %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());
    return 0;
}
```

```bash
gcc privilege_demo.c -o privilege_demo
./privilege_demo                        # both = your UID

sudo chown root:root privilege_demo
sudo chmod u+s privilege_demo
ls -l privilege_demo                    # look for the 's'
./privilege_demo                        # Real = you, Effective = 0
```

> 🛡️ **Security note:** A SUID-root binary is a high-value target. Every bug in it becomes a path to root. Delete your demo when done: `sudo rm privilege_demo`.

### 🎯 Self-Discovery Challenge

1. List SUID binaries on your system:
   ```bash
   find / -perm -4000 -type f 2>/dev/null
   ```
2. **Why does `/usr/bin/passwd` need SUID?** (Hint: where are password hashes stored, and who can write there?)
3. 🌶️ *Bonus:* Read `man 2 setuid` and learn why SUID programs often *drop* privileges as soon as they can.

### 🌅 Morning Review

<details><summary><b>Q1.</b> Real UID vs. Effective UID?</summary>

Real = who started the process. Effective = whose permissions the kernel enforces.
</details>

<details><summary><b>Q2.</b> What does the SUID bit do?</summary>

When the file runs, the process's effective UID becomes the **file owner's** UID instead of the caller's.
</details>

<details><summary><b>Q3.</b> Why is <code>passwd</code> SUID root?</summary>

Regular users must update their own entry in root-owned files like `/etc/shadow`. SUID lets the tool do that safely and narrowly.
</details>

---

# 🔗 Week 6 · Dynamic Linking & `LD_PRELOAD`

> **🧩 Big Picture:** Most programs don't carry their own copy of `printf` or `puts`. They borrow from shared libraries (`.so`) at runtime. Whoever the loader finds *first* wins.

### 🧠 Mental Model

```text
  ./prog starts
     │
     ▼
  ld.so (dynamic linker) loads libraries
     │    1st: anything in LD_PRELOAD   ◄── your override jumps the queue
     │    2nd: libc.so.6 and friends
     ▼
  puts("hi") → resolves to the FIRST "puts" found
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | Function pointers, building `.so` files (`-shared -fPIC`) |
| **Linux** | `ld.so`, `ldd`, `LD_PRELOAD`, `LD_LIBRARY_PATH` |

### 🔬 Practical Lab

**1. The target (`original.c`):**

```c
#include <stdio.h>
int main(void) { puts("I am the real puts()"); return 0; }
```

**2. The override (`custom_puts.c`):**

```c
#include <stdio.h>

int puts(const char *s) {
    // We can't call puts() here (infinite recursion!), so use printf.
    printf("[intercepted] original message was: %s\n", s);
    return 0;
}
```

**3. Build & run:**

```bash
gcc original.c -o original
gcc -shared -fPIC custom_puts.c -o custom_puts.so

./original
LD_PRELOAD=./custom_puts.so ./original
```

> 🛡️ **Why defenders care:** This exact mechanism is used for debugging, profiling, *and* user-space rootkits. Learn it to detect it.

### 🎯 Self-Discovery Challenge

1. Run `ldd /bin/ls`. Identify `libc` and the dynamic loader.
2. **Why is `LD_PRELOAD` ignored for SUID binaries run by regular users?** (Search: *"glibc secure-execution mode"* / `AT_SECURE`.)
3. 🌶️ *Bonus:* Use `dlsym(RTLD_NEXT, "puts")` to call the *real* `puts` from your wrapper.
4. 🌶️ *Bonus:* Check for suspicious preloads on a system: `cat /etc/ld.so.preload` and `env | grep LD_`.

### 🌅 Morning Review

<details><summary><b>Q1.</b> What's a shared object (<code>.so</code>)?</summary>

A library loaded at runtime and shared among many programs, instead of being copied into each binary.
</details>

<details><summary><b>Q2.</b> What does <code>LD_PRELOAD</code> do?</summary>

Tells the dynamic linker to load the listed libraries **first**, so their functions take precedence.
</details>

<details><summary><b>Q3.</b> Why is it blocked for SUID programs?</summary>

Otherwise an unprivileged user could inject their own code into a process running with elevated privileges, an instant privilege escalation.
</details>

---

# 🌐 Week 7 · Network Sockets

> **🧩 Big Picture:** A socket is a file descriptor connected to the network. Once connected, you `read()` and `write()` just like a file.

### 🧠 Mental Model

```text
  SERVER                               CLIENT
  socket()   create endpoint           socket()
  bind()     claim port 4444
  listen()   open for business
  accept()   ◄──────── connect() ──────── connect()
  write()    ─────────  bytes  ─────────► read()
  close()                              close()
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `struct sockaddr_in`, `socket()`, `bind()`, `listen()`, `accept()` |
| **Linux** | `htons()`/`htonl()`, `127.0.0.1`, `nc`, `ss` |

### 🔬 Practical Lab

Build a tiny TCP server skeleton and **fill in the blanks**:

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void) {
    int srv = socket(AF_INET, SOCK_STREAM, 0);

    int yes = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(4444);          // network byte order!
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback only = safe

    // TODO: bind(srv, (struct sockaddr *)&addr, sizeof(addr));
    // TODO: listen(srv, 5);
    // TODO: int client = accept(srv, NULL, NULL);
    // TODO: write(client, "Welcome!\n", 9);
    // TODO: close(client); close(srv);
    return 0;
}
```

Test from another terminal:

```bash
nc 127.0.0.1 4444
```

> 🛡️ **Tip:** Bind to `127.0.0.1` (not `0.0.0.0`) while learning, so your toy server is never exposed to the network.

### 🎯 Self-Discovery Challenge

1. While the server runs: `ss -tlnp | grep 4444`. You should see `LISTEN`.
2. Connect with `nc`, then run `ss -tn` again. Spot `ESTABLISHED`.
3. After the client disconnects, look for **`TIME_WAIT`**. Why does the OS keep that around?
4. 🌶️ *Bonus:* Make the server loop forever, handling one client after another.

### 🌅 Morning Review

<details><summary><b>Q1.</b> Order the server calls.</summary>

`socket()` → `bind()` → `listen()` → `accept()` → `read()/write()` → `close()`.
</details>

<details><summary><b>Q2.</b> Why <code>htons(4444)</code>?</summary>

Network protocols use big-endian byte order; `htons` converts a 16-bit number from host order to network order.
</details>

<details><summary><b>Q3.</b> Why <code>SO_REUSEADDR</code>?</summary>

So you can restart the server immediately without waiting for the old socket's `TIME_WAIT` to expire.
</details>

---

# 📦 Week 8 · Raw Bytes & Packet Parsing

> **🧩 Big Picture:** A network packet is just an array of bytes with an agreed-upon layout. If you know the layout, a `struct` can be a window over those bytes.

### 🧠 Mental Model

```text
  IPv4 header (first 20 bytes, simplified):

  byte:  0        1        2   3
       ┌────────┬────────┬────────────┐
       │Ver│IHL │  TOS   │Total Length│
       ├────────┴────────┼────────────┤
       │  Identification │Flags/Frag  │
       ├────────┬────────┼────────────┤
       │  TTL   │Protocol│  Checksum  │
       ├────────┴────────┴────────────┤
       │      Source IP (4 bytes)     │
       ├──────────────────────────────┤
       │    Destination IP (4 bytes)  │
       └──────────────────────────────┘
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `struct` layout, `unsigned char` buffers, bit shifting, `ntohs()` |
| **Linux** | `tcpdump -X`, loopback interface `lo`, Wireshark (optional) |

### 🔬 Practical Lab

```c
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

struct __attribute__((packed)) ipv4_hdr {
    uint8_t  ver_ihl;
    uint8_t  tos;
    uint16_t total_len;
    uint16_t id;
    uint16_t flags_frag;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t src;
    uint32_t dst;
};

int main(void) {
    unsigned char pkt[20] = {
        0x45, 0x00, 0x00, 0x28, 0x1c, 0x46, 0x40, 0x00,
        0x40, 0x06, 0x00, 0x00, 192, 168, 1, 10,
        10, 0, 0, 1
    };

    struct ipv4_hdr *ip = (struct ipv4_hdr *)pkt;
    struct in_addr s = { .s_addr = ip->src };

    printf("Version:   %d\n", ip->ver_ihl >> 4);       // bit shift!
    printf("TTL:       %d\n", ip->ttl);
    printf("Total len: %d\n", ntohs(ip->total_len));
    printf("Source IP: %s\n", inet_ntoa(s));
    return 0;
}
```

Extend it: add a TCP header struct and print the **destination port** from bytes after the IP header.

### 🎯 Self-Discovery Challenge

1. In one terminal: `sudo tcpdump -X -i lo`. In another: `nc 127.0.0.1 4444` and type something. **Match the hex to header fields.**
2. Why is `__attribute__((packed))` needed? Try removing it and print `sizeof(struct ipv4_hdr)`.
3. What breaks if you forget `ntohs()` on a Little-Endian machine?

### 🌅 Morning Review

<details><summary><b>Q1.</b> What does <code>0x45</code> mean at the start of an IPv4 header?</summary>

High nibble `4` = IP version 4. Low nibble `5` = header length of 5 × 4 = 20 bytes.
</details>

<details><summary><b>Q2.</b> Why do compilers add padding to structs?</summary>

For CPU alignment. `packed` disables padding so the struct matches the wire format exactly.
</details>

<details><summary><b>Q3.</b> What's the danger of trusting a length field from a packet?</summary>

An attacker controls it. Using it unchecked for copying or indexing causes buffer over-reads/overflows (a classic real-world bug class).
</details>

---

# 🛡️ Week 9 · Memory Boundaries & Defensive Coding

> **🧩 Big Picture:** C trusts you completely. Write past the end of a buffer, and C will happily overwrite whatever lives next door. This week you learn *why* that's dangerous and *how* defenses work.

### 🧠 Mental Model

```text
  Stack frame of a function (simplified, grows downward):

  Higher addresses
  ┌───────────────────────┐
  │  return address       │  ← where the CPU goes after this function
  ├───────────────────────┤
  │  saved frame pointer  │
  ├───────────────────────┤
  │  🐤 stack canary      │  ← secret value, checked before return
  ├───────────────────────┤
  │  char buf[16]         │  ← overflow here ↑ spills toward the canary
  └───────────────────────┘
  Lower addresses
```

If something overwrites the canary, the program aborts with `*** stack smashing detected ***` instead of silently returning to a corrupted address.

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `strcpy`/`gets` (unbounded) vs. `fgets`/`strncpy`/`snprintf` (bounded) |
| **Linux** | Stack canaries, SIGSEGV, core dumps, ASLR, `checksec` |

### 🔬 Practical Lab

**Safe version:**

```c
#include <stdio.h>
int main(void) {
    char buf[16];
    printf("Name: ");
    if (fgets(buf, sizeof(buf), stdin)) printf("Hello, %s", buf);
    return 0;
}
```

**Unsafe version (for study only):**

```c
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
    char buf[16];
    if (argc > 1) strcpy(buf, argv[1]);   // ⚠️ no bounds check
    printf("You said: %s\n", buf);
    return 0;
}
```

```bash
gcc -fno-stack-protector -g unsafe.c -o unsafe
./unsafe hi
./unsafe AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA   # watch it crash
```

> ✅ **Goal this week is understanding and defense.** Your deliverable is a program that *safely* handles hostile input, not an exploit.

### 🎯 Self-Discovery Challenge

1. Recompile **without** `-fno-stack-protector` and rerun with the long input. Read the abort message.
2. Run `dmesg | tail` after a segfault. What does the kernel log?
3. Fix `unsafe.c` three different ways (`fgets`, `snprintf`, a length check with `strlen`).
4. 🌶️ *Bonus:* Compile with `-fsanitize=address -g` and read AddressSanitizer's report. It's the best bug-finding tool you'll ever meet.

### 🌅 Morning Review

<details><summary><b>Q1.</b> Why is <code>strcpy</code> dangerous?</summary>

It copies until it hits a null byte, with no knowledge of the destination's size.
</details>

<details><summary><b>Q2.</b> What does a stack canary do?</summary>

A random value placed before the return address; checked on function exit. If changed, the program aborts before using the corrupted return address.
</details>

<details><summary><b>Q3.</b> Name three defenses beyond canaries.</summary>

ASLR, non-executable stack (NX/DEP), `-D_FORTIFY_SOURCE`, PIE, bounds-checked APIs, sanitizers, safer languages.
</details>

---

# 🔍 Week 10 · Debugging with GDB

> **🧩 Big Picture:** A debugger lets you freeze a running program and look inside: registers, memory, instructions. It's the microscope for everything you learned in Weeks 1–9.

### 🧠 Mental Model

```text
  C source ─► compiler ─► assembly ─► machine code
     ▲                                     │
     └──────── GDB maps them together ◄────┘

  Key registers (x86-64):
    RIP  instruction pointer: the NEXT instruction to run
    RSP  stack pointer: top of the stack
    RBP  frame/base pointer: base of the current function's frame
    RAX  return value, general purpose
```

### 🎓 Concepts to Master

| Area | Topics |
|------|--------|
| **C** | `-g` debug symbols, stack frames, how a function call works |
| **Linux** | `gdb`, breakpoints, `info registers`, `x/` memory examine, `disassemble` |

### 🔬 Practical Lab

```bash
gcc -g -O0 program.c -o program
gdb ./program
```

```text
(gdb) break main              # pause at main
(gdb) run                     # start the program
(gdb) next                    # step one C line (over calls)
(gdb) step                    # step one C line (into calls)
(gdb) info registers          # dump all registers
(gdb) x/10x $rsp              # 10 hex words from the stack top
(gdb) x/s some_string_ptr     # print memory as a string
(gdb) disassemble main        # view the assembly
(gdb) bt                      # backtrace: who called whom?
(gdb) quit
```

**Run your Week 2, 3, and 9 programs inside GDB.** Watch the stack. See your canary. Find your variables' addresses. This is where the whole roadmap clicks together. 🧩

### 🎯 Self-Discovery Challenge

1. Switch syntax: `set disassembly-flavor intel`.
2. Same instruction, two dialects. **Which is which?**
   ```text
   Intel:   mov eax, ebx        ← destination FIRST
   AT&T:    movl %ebx, %eax     ← source FIRST (plus % and size suffixes)
   ```
3. Make it permanent: `echo "set disassembly-flavor intel" >> ~/.gdbinit`.
4. 🌶️ *Bonus:* Use `watch global_var` to pause whenever a variable changes.
5. 🌶️ *Bonus:* Install [pwndbg](https://github.com/pwndbg/pwndbg) or GEF to *rice* your GDB. 🎨

### 🌅 Morning Review

<details><summary><b>Q1.</b> What does <code>RIP</code> hold?</summary>

The address of the next instruction the CPU will execute.
</details>

<details><summary><b>Q2.</b> <code>next</code> vs. <code>step</code>?</summary>

`next` steps over function calls. `step` steps into them.
</details>

<details><summary><b>Q3.</b> Why compile with <code>-g</code>?</summary>

It embeds debug info (source lines, variable names) so GDB can map machine code back to your C.
</details>

---

## 📋 Master Cheat Sheet

<details open>
<summary><b>🔧 GCC</b></summary>

```bash
gcc file.c -o out                  # compile + link
gcc -Wall -Wextra -g file.c -o out # warnings + debug symbols
gcc -E / -S / -c                   # stop after preprocess / asm / object
gcc -shared -fPIC lib.c -o lib.so  # shared library
gcc -fsanitize=address -g f.c      # AddressSanitizer
```
</details>

<details>
<summary><b>🔎 Inspection</b></summary>

```bash
file bin            # file type
size bin            # section sizes
readelf -h bin      # ELF header
objdump -d bin      # disassemble
nm bin              # symbols
ldd bin             # shared libs
strings bin         # printable strings
xxd bin | head      # hex dump
strace ./bin        # system calls
ltrace ./bin        # library calls
```
</details>

<details>
<summary><b>🧠 Process & Memory</b></summary>

```bash
ps aux | grep name
pgrep -n name
cat /proc/PID/maps
cat /proc/PID/status
ls -l /proc/PID/fd
cat /proc/sys/kernel/randomize_va_space
```
</details>

<details>
<summary><b>🌐 Network</b></summary>

```bash
ss -tlnp                 # listening TCP sockets
ss -tn                   # active TCP connections
nc 127.0.0.1 4444        # connect with netcat
sudo tcpdump -X -i lo    # packet hex dump
```
</details>

<details>
<summary><b>🐞 GDB</b></summary>

```text
break / run / next / step / continue / bt / finish
info registers  |  x/NFU addr  |  print var  |  disassemble
set disassembly-flavor intel
```
</details>

---

## 📚 Glossary

| Term | Meaning |
|------|---------|
| **ELF** | Executable and Linkable Format: Linux's binary format |
| **Syscall** | A request from a program to the kernel |
| **FD** | File descriptor: an integer handle to an open resource |
| **Virtual memory** | Each process's private view of memory, mapped by the kernel |
| **Stack** | Fast, automatic memory for function calls and locals |
| **Heap** | Manual memory from `malloc()` |
| **SUID** | Run as the file's owner instead of the caller |
| **Endianness** | The byte order a number is stored in |
| **Canary** | A guard value that detects stack overwrites |
| **ASLR** | Randomized memory layout on each run |
| **Symbol** | A named function/variable in a binary |
| **Register** | A tiny, ultra-fast storage slot inside the CPU |

---

## 🏁 After Week 10

You now have the foundation. Pick your next quest:

| Path | Next steps |
|------|-----------|
| 🔐 **Secure Development** | Static analysis (`clang-tidy`, CodeQL), fuzzing (AFL++, libFuzzer), secure coding standards (CERT C) |
| 🧪 **Vulnerability Research** | Heap internals, ROP & modern mitigations, CTF platforms (pwn.college, OverTheWire, picoCTF) |
| 🐧 **Systems Engineering** | `fork`/`exec`, signals, threads, `epoll`, writing a mini shell |
| 🛰️ **Network Security** | Raw sockets, Wireshark dissectors, writing a packet sniffer |
| ⚙️ **Reverse Engineering** | x86-64 assembly deep-dive, Ghidra, calling conventions |

### 📖 Recommended Reading

- *The C Programming Language*, Kernighan & Ritchie
- *The Linux Programming Interface*, Michael Kerrisk
- *Computer Systems: A Programmer's Perspective*, Bryant & O'Hallaron
- *Beej's Guide to Network Programming* (free online)
- `man` pages. Always.

---

<div align="center">

### 🌱 *Slow is smooth. Smooth is fast.*

**One week. One concept. One cup of coffee in the morning.** ☕

⭐ *Keep this file in a repo and commit your progress each week.* ⭐

</div>
