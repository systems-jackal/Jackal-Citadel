# Before we start coding and writing functions and all that it is important that we undersatand the dynamics of the language an how the language handles tasks at the low level.This is important since we will be dealing with what we understand and also easy to debug and fix errors as we learn.

# Go Properties

Core features of the Go language, with what each means and why it matters for security tooling.

## 1. Statically typed
Every variable has a fixed type, checked before the program runs.
```go
var port int = 80
port = "http"   // compile error: can't assign string to int
```

## 2. Compiled to a native binary
`go build` turns code into one executable that the CPU runs directly. No interpreter needed on the target machine.

## 3. Fast compilation
Builds take seconds, so the edit, build, run loop stays quick.

## 4. Cross-compilation
Build for another OS or CPU from your own machine:
```bash
GOOS=windows GOARCH=amd64 go build ./lesson01
```

## 5. Static linking
Pure Go programs bundle everything they need into one file. Copy it to a machine and run it. (`CGO_ENABLED=0` guarantees this.)

## 6. Garbage collection
Go frees unused memory automatically. There is no `free()`.

## 7. Simple, small syntax
Only 25 keywords and usually one way to do things, which makes other people's Go code easy to read.

## 8. Goroutines
Lightweight concurrent tasks. Put `go` before a function call:
```go
go scanPort(80)
```
Thousands can run at once.

## 9. Channels
A safe way for goroutines to pass data to each other without sharing memory.

## 10. Structs and methods
Data is grouped into **structs**, and functions are attached to them as **methods**. This replaces classes.

## 11. Interfaces
A contract: "anything with these methods qualifies". Types satisfy interfaces **implicitly**, with no `implements` keyword.

## 12. Composition over inheritance
No inheritance. Bigger types are built by embedding smaller ones.

## 13. Explicit error handling
Errors are ordinary return values that you check:
```go
conn, err := net.Dial("tcp", "127.0.0.1:80")
if err != nil {
    // handle it
}
```
No hidden exceptions: every failure point is visible in the code.

## 14. Multiple return values
A function can return more than one value, which is what makes the `value, err` pattern work.

## 15. Pointers (without pointer arithmetic)
References to data are allowed, but C-style memory math is not, which removes many memory-corruption bugs.

## 16. `defer`
Schedules cleanup to run when the function ends:
```go
defer conn.Close()
```

## 17. Powerful standard library
`net`, `net/http`, `crypto`, `encoding/json`, `os`, `testing` and more are included, so real tools need little third-party code.

## 18. Built-in tooling
| Command | Job |
|---|---|
| `go run` | build and run |
| `go build` | make a binary |
| `go test` | run tests |
| `go fmt` | format code in the one official style |
| `go vet` | catch suspicious code |
| `go mod` | manage dependencies |

## 19. Strict about waste
Unused variables and unused imports are compile errors.

## 20. Open source with a compatibility promise
Created at Google and released as open source. Old Go code keeps compiling on new Go versions.

---

## Where each feature shows up in the lessons

| Feature | Lesson |
|---|---|
| Types, `fmt` | Lesson 1 |
| Multiple returns, errors | Lesson 3 |
| Structs, interfaces | Lesson 5 |
| `defer`, file I/O | Lesson 6 |
| Goroutines, channels | Lesson 8 (concurrent scanner) |
| Standard library `net` | Lessons 8 and 9 |
