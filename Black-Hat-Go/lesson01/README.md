# Lesson 01: Structure of a Go source file

## Goal

Understand how a Go file is organized, how two files in one folder work together, and how to build and inspect a binary.

## Files

| File | Purpose |
|---|---|
| `main.go` | Holds `func main()`, the entry point |
| `message.go` | Holds `receiveMessage`, called from `main.go` |

## Run it

From the `Black-Hat-Go` folder (not from inside `lesson01`):

```bash
go run ./lesson01
```

Expected output:

```
Jackal Online
Received Message: Black Hat Go
Anonymous Who am I
```

## Concepts

### The three zones of a file, in order

1. **Package clause**: `package main`. Only comments may come before it.
2. **Imports**: `import "fmt"`. Unused imports are compile errors.
3. **Declarations**: `const`, `var`, `type`, `func`. Their order among themselves is not enforced.

### Package

A folder of `.go` files compiled together as one unit.
- `package main` with a `func main()` builds a **runnable program**.
- Any other name builds a **library** that other code imports.
- Every file in a folder must use the same package name.

### Import

A file's request to use another package. It is **per file**, and can point to the standard library, your own module, or third-party code.

### Calling across files

Files in one folder with the same package line are one package. They call each other directly: no import, no file name.

### Comments

```go
// single line
/* multi
   line */
```

### Run the folder, not one file

```bash
go run ./lesson01          # works
go run lesson01/main.go    # fails: undefined: receiveMessage
```

### Private vs public

A lowercase first letter (`receiveMessage`) is visible only inside the package. A capital (`ReceiveMessage`) is visible to other packages.

## Security notes

- **Binaries are readable.** Build and inspect:
  ```bash
  mkdir -p bin
  go build -o bin/lesson01 ./lesson01
  ls -lh bin/lesson01
  strings bin/lesson01 | grep -i "anonymous"
  ```
  Anything hardcoded in code (passwords, keys, tokens) can be pulled out of the binary. Never hardcode secrets, and never leave them in comments.
- **Every import is a dependency to trust.** Standard library, your own code, and third-party code carry different levels of trust.
- **Lowercase names keep your package's internals private**, which reduces what other code can misuse.

## Break it on purpose

| Experiment | Error to expect |
|---|---|
| Add `import "os"` and never use it | `"os" imported and not used` |
| Change `package main` to `package scanner` in one file | `found packages main ... and scanner ...` |
| Rename `func main()` to `func start()` | `function main is undeclared in the main package` |
| Put `fmt.Println("hi")` outside any function | `non-declaration statement outside function body` |
| Run `go run lesson01/main.go` | `undefined: receiveMessage` |

## What I learned (write in your own words)

-

## Mistakes and fixes

-
