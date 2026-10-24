<div align="center">

```text
  ┌──────────────────────────────────────────────────────────┐
  │                                                          │
  │   malloc( (uint16_t) 65540 )   →   4 bytes               │
  │   memcpy( dst, src, 65540 )    →   💥 65,536 bytes over  │
  │                                                          │
  │          A N Y P W N - I N S P I R E D                   │
  │       L I N U X   M E M O R Y - S A F E T Y   L A B      │
  │                                                          │
  └──────────────────────────────────────────────────────────┘
```

# 🧪 AnyPwn-Inspired Linux Memory-Safety Lab

**Integer truncation → heap-buffer overflow → caught by AddressSanitizer → fixed by validation.**

![language](https://img.shields.io/badge/language-C-f38ba8?style=for-the-badge)
![tooling](https://img.shields.io/badge/tooling-GCC_%2B_ASan_%2B_UBSan-89b4fa?style=for-the-badge)
![harness](https://img.shields.io/badge/harness-Python_3-a6e3a1?style=for-the-badge)
![platform](https://img.shields.io/badge/platform-Ubuntu_%7C_Debian_%7C_Kali-fab387?style=for-the-badge)
![scope](https://img.shields.io/badge/scope-local_only-cba6f7?style=for-the-badge)

</div>

---

> ## ⚠️ Scope & Disclaimer
>
> This is an **intentionally vulnerable, standalone C parser** that models one bug class: **integer truncation leading to a heap-buffer overflow**.
>
> - ❌ **Not** a verified reproduction of any AnyDesk proprietary vulnerability
> - ❌ **Not** an exploit against AnyDesk
> - ❌ **Not** evidence that any real installation is compromised
> - ❌ **No** networking, shellcode, or remote execution is included
>
> 🛡️ Run the unsafe binaries only in a **disposable VM or container**, never on a system you care about.

---

## 📑 Table of Contents

- [🔍 What This Demonstrates](#-what-this-demonstrates)
- [🗂️ Project Layout](#️-project-layout)
- [🧰 Requirements](#-requirements)
- [🚀 Quick Start](#-quick-start)
- [🧪 Test Matrix](#-test-matrix)
- [📊 Report Categories](#-report-categories)
- [🐞 GDB Debugging](#-gdb-debugging)
- [🩺 Troubleshooting](#-troubleshooting)
- [📜 Source-Log Evidence](#-source-log-evidence)
- [🚧 Limitations](#-limitations)

---

## 🔍 What This Demonstrates

A **32-bit declared length** is narrowed to **16 bits** when sizing the destination buffer, but `memcpy` still uses the **original 32-bit length**.

```text
  declared length (32-bit)     = 65,540
  allocation size  (16-bit)    = 65,540 mod 65,536 = 4      ◄── truncated!
  memcpy length    (32-bit)    = 65,540                     ◄── original!

  ┌──────────┐
  │ 4 bytes  │ ◄── the buffer that actually exists
  └──────────┘
  ├──────────────── 65,536 bytes written past the end ────────────────►
```

At **65,540 bytes** the destination holds only **4 bytes**. AddressSanitizer catches the invalid write.

The **patched** version rejects any length above **65,535** *before* allocation.

> 📝 **Precise name for the bug:** this is **integer truncation** (a narrowing conversion), **not** signed arithmetic overflow.

### The pattern in miniature (illustrative)

```c
uint32_t len  = declared_length;        // 65,540
uint16_t size = (uint16_t)len;          // truncated → 4
char *dst = malloc(size);               // 4 bytes
memcpy(dst, src, len);                  // copies 65,540 bytes ← overflow
```

```c
// The fix: validate BEFORE allocating
if (len > UINT16_MAX) return 3;         // reject
```

*(The real code lives in `src/vulnerable.c` and `src/patched.c`.)*

### How truncation changes the numbers

| Declared length | Allocated (`len mod 65,536`) | Copied | Result |
|----------------:|-----------------------------:|-------:|--------|
| 100 | 100 | 100 | ✅ Fits |
| 65,535 | 65,535 | 65,535 | ✅ Fits (max 16-bit value) |
| 65,536 | **0** | 65,536 | 💥 Overflow |
| 65,540 | **4** | 65,540 | 💥 Overflow |
| 70,000 | **4,464** | 70,000 | 💥 Overflow |

---

## 🗂️ Project Layout

```text
anypwn-memory-lab/
├── README.md
├── src/
│   ├── vulnerable.c       # intentionally unsafe
│   ├── patched.c          # validated length
│   └── integer_demo.c     # integer truncation demonstration
├── scripts/
│   ├── build.py           # GCC + ASan/UBSan
│   └── test_lab.py        # categorized regression testing
├── build/                 # generated executables
└── reports/               # generated JSON and Markdown
```

---

## 🧰 Requirements

- Ubuntu 24.04 / Debian / Kali
- GCC, Python 3, and (optionally) GDB
- **No third-party Python dependencies**

```bash
sudo apt update
sudo apt install -y build-essential gcc gdb python3
```

---

## 🚀 Quick Start

Run everything from the directory containing this README.

```bash
python3 scripts/build.py              # 1. Build with ASan + UBSan

./build/integer_demo                  # 2. See truncation in isolation

./build/vulnerable 100                # 3. Valid length: works
./build/vulnerable 65540              #    ⚠️ intentionally exits abnormally under ASan

./build/patched 65540                 # 4. Rejected → exit code 3
./build/patched 100                   #    Accepted → exit code 0

python3 scripts/test_lab.py           # 5. Run the full categorized test suite
cat reports/results.md                # 6. Read the report
```

### 📋 Capturing the full sanitizer trace

```bash
./build/vulnerable 65540 2>&1 | tee reports/asan.log
```

> ⚠️ Without `pipefail`, the pipeline's exit code is `tee`'s, not the program's. To keep the real exit code:
>
> ```bash
> set -o pipefail
> ./build/vulnerable 65540 2>&1 | tee reports/asan.log; echo "exit: ${PIPESTATUS[0]}"
> ```
>
> For **reliable test classification**, use `python3 scripts/test_lab.py`.

---

## 🧪 Test Matrix

| Length | 🔓 Vulnerable | 🔒 Patched |
|-------:|---------------|-----------|
| 100 | ✅ Accepted | ✅ Accepted |
| 65,535 | ✅ Accepted | ✅ Accepted |
| 65,536 | 💥 ASan heap overflow | 🚫 Rejected |
| 65,540 | 💥 ASan heap overflow | 🚫 Rejected |
| 70,000 | 💥 ASan heap overflow | 🚫 Rejected |

The Python harness **records each actual observation** rather than assuming these results occurred.

- A sanitizer finding is an **expected success** for the *vulnerable* binary.
- A rejected oversized length is an **expected success** for the *patched* binary.

---

## 📊 Report Categories

| Category | Meaning |
|----------|---------|
| ✅ `PASS_VALID` | Valid input processed successfully |
| 🎯 `VULNERABILITY_CONFIRMED` | Expected ASan heap overflow in the synthetic vulnerable program |
| 🛡️ `PATCH_VERIFIED` | Patched program rejects oversized length (exit `3`) |
| 🔴 `REGRESSION` | ASan reports an overflow in the **patched** program |
| 🟠 `UNEXPECTED_REJECTION` | A **valid** length fails |
| 🧱 `BUILD_FAILURE` | Compilation/linking failure (reported by the build script) |
| ⚙️ `RUNTIME_FAILURE` | Other unexpected runtime result |
| ⏱️ `TIMEOUT` | Test exceeded 15 seconds |
| ❔ `INCONCLUSIVE` | Oversized vulnerable test did not produce expected sanitizer evidence |

### 📁 Generated outputs

| File | Contents |
|------|----------|
| `reports/results.json` | Per-test stdout/stderr, exit codes, and a summary |
| `reports/results.md` | Readable category tables |

> A **nonzero harness exit code** signals unexpected results.

---

## 🐞 GDB Debugging

Build an **unsanitized** binary for line-by-line inspection:

```bash
gcc -g -O0 -fno-omit-frame-pointer src/vulnerable.c -o build/vulnerable_debug
gdb ./build/vulnerable_debug
```

Run each GDB command **separately** (one line at a time):

```text
(gdb) set pagination off
(gdb) break main
(gdb) run 65540
(gdb) break vulnerable.c:25
(gdb) continue
(gdb) list
(gdb) print payload_length
(gdb) print capacity
```

> 💡 **Important:**
> - `break vulnerable.c:25` is **illustrative only**. Use `list` to find the right line, then break **after `capacity` is assigned and before `memcpy`**.
> - Printing a variable before it's initialized shows **arbitrary values**. Use `next` until the allocation statement has executed.
> - **Don't continue past `memcpy`** in the unsanitized binary. Behavior is undefined.

### 🔭 What to look for

```text
payload_length = 65540     ← the original 32-bit value
capacity       = 4         ← the truncated 16-bit value
```

The mismatch between those two numbers *is* the bug.

---

## 🩺 Troubleshooting

*(From the supplied terminal log.)*

| # | Symptom | Cause | Fix |
|:-:|---------|-------|-----|
| 1 | `undefined reference to main` | GCC compiled a file, but the linker found no valid `main()`. Happened twice for `patched.c` in the log; a later compile succeeded. | The provided `patched.c` includes `main()`. |
| 2 | `scripts/build.py` or `scripts/test_lab.py` missing | The directory existed but the files hadn't been created. | This archive includes both. |
| 3 | `results.json` missing | It's generated only after the test run. | Run `python3 scripts/test_lab.py`. |
| 4 | `cd anypwn-memory-lab` fails | You're already inside that directory. | Run `pwd` and `ls` before changing directories. |
| 5 | GDB: `"on" or "off" expected` | Commands were pasted together as one input. | Execute **one line at a time**. |
| 6 | GDB shows a bizarre `capacity` | Inspected before initialization. | Step past the assignment first. |
| 7 | `LS: command not found` | Shell commands are **case-sensitive**. | Use lowercase `ls`. |
| 8 | Pending kernel upgrade notice | `apt` flagged a pending reboot. | Unrelated to the C build errors. Reboot when convenient. |

---

## 📜 Source-Log Evidence

The uploaded terminal transcript (user-provided) shows:

- ✅ A successful normal run
- 💥 A reproducible **ASan heap-buffer-overflow** at the copy operation, a **65,540-byte write beyond a 4-byte allocation**
- 🛡️ Successful patched tests
- 📂 Missing automation scripts (since added)

> These are **historical observations from the supplied log**, not results from a real AnyDesk installation.

---

## 🚧 Limitations

- This lab does **not** prove a CVE's exact root cause or affected versions.
- **No** production software, network service, or AnyDesk binary is accessed.
- Sanitizer reports and return codes **can vary across platforms**.
- **Do not** run intentionally unsafe binaries on important systems. Use a disposable VM or container.

---

<div align="center">

### 🧠 Takeaway

**Validate the length *before* you narrow it. Allocate and copy using the *same* number.**

⭐ *Break it safely. Understand it. Fix it.* ⭐

</div>
