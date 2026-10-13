# Golang for Cyber Security

A hands-on learning journey: Go fundamentals through small offensive-security tools, with every lesson and project documented.

## Why I'm doing this

After months of moving from one programming language to another, I've decided to settle on **Go as my primary language**.

My earlier problem was not a lack of effort. I spent time on topics that didn't line up with my goal, and because of that I couldn't describe the problems I was hitting clearly. A problem I can't describe is a problem I can't solve, and the result was confusion.

This time:

1. **One language**: Go, until I can build real tools with it.
2. **One direction**: every topic must connect to offensive security and tooling.
3. **Problems written down**: when I'm stuck, I define the problem in writing *before* looking for a fix.
4. **Everything documented**: each lesson and project gets its own notes.

## Why Go

| Reason | Why it matters for security tooling |
|---|---|
| **Compiles to one native binary** | A tool is a single file I can copy to a lab machine and run, with no interpreter or dependencies to install |
| **Static linking** (`CGO_ENABLED=0`) | The binary carries what it needs, so it behaves the same everywhere |
| **Cross-compilation** | I build Windows, Linux, and ARM binaries from my Linux machine: `GOOS=windows GOARCH=amd64 go build` |
| **Goroutines and channels** | Concurrency is built in, so scanners, brute-forcers, and crawlers that handle thousands of connections are practical to write |
| **Strong standard library** | `net`, `net/http`, `crypto`, `encoding/json`, and `os` cover most of what network tools need |
| **Static typing and strict compiler** | Many bugs (type errors, unused variables and imports) are caught before the program runs |
| **Explicit error handling** | Every failure point is visible in the code, which suits tools that must cope with unreliable networks |
| **Memory safety with garbage collection** | No manual memory management, which removes a large class of bugs from my own code |
| **Fast compile times** | Quick edit, build, and run cycles keep practice moving |
| **Built-in tooling** | `go run`, `go build`, `go test`, `go fmt`, `go vet`, and `go mod` ship with the language |
| **Industry adoption** | Many widely used security tools are written in Go, so reading their source is itself part of the learning |

## Honest trade-offs

Choosing Go doesn't mean it wins everywhere.

- **Binary size**: even a small program is a couple of megabytes, because the runtime is bundled in.
- **Not invisible**: Go binaries carry function names, type information, and readable strings. Defenders and analysts have tooling for them, and I should understand that from both sides.
- **Garbage collector**: fine for tooling, but not the right choice for work that needs tight control of memory.
- **Other languages still have a place**: Python is quicker for throwaway scripts, and C or Rust suit low-level work. Go is my main language, not my only tool.

## Rules for this journey

- Type the code myself instead of pasting solutions.
- Write a short note for every lesson: goal, code, what it does (in my own words), mistakes and fixes.
- Run network tools only against localhost, my own lab machines, or hosts that explicitly permit testing.
- Never hardcode secrets, and never leave them in comments.
- Finish small projects instead of collecting resources.


## Resources

- [Roadmap: Go for offensive security](https://roadmap.sh/ai/roadmap/go-for-offensive-security)
- [Black Hat Go](https://www.kea.nu/files/textbooks/humblesec/blackhatgo.pdf) 
- A Tour of Go and Go by Example for syntax practice

## Scope and ethics

Everything here is for learning and for authorized testing only. I only test systems I own or have explicit permission to test.
