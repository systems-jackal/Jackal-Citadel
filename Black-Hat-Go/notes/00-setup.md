# 00 - Go environment setup
*Brief note that guide the set up of Golang language environment*
## Goal
Set up a Go module in Black-Hat-Go so I can write and run Go code.
First check the language version installed using the command `go version`.

To Bettwr Understand this we will look at the go environment tree
```Golang ENV Tree
/usr/local/go/                      <- GOROOT: Go itself (compiler, standard library)
│   └── bin/go                         the `go` command you run

/home/jackal/go/                    <- GOPATH: Go's shared storage
│   ├── bin/                           tools you install (gopls, staticcheck)
│   └── pkg/mod/                       downloaded third-party packages (cache)

~/Jackal-Arcanum/Jackal-Citadel/Black-Hat-Go/    <- YOUR PROJECT (the module)
│   ├── go.mod                         marks this folder as a module (created by go mod init)
│   ├── README.md
│   ├── notes/                         your documentation
│   ├── lesson01/main.go               one package per lesson
│   ├── lesson02/main.go
│   └── bin/                           compiled binaries (gitignored)
```

Go Layer Include :
`GOROOT` - The GO installer.
`GOPATH` - GO Automatically.
`MODULE` - You, Where all the work lives.

## Commands / code
The command i ran are 
`go mod init blackhatgo`
`go env GOMOD`
`cat go.mod`

What each command does 
```Command Purpose
1. go mod init blackhatgo

Action: creates a new file called go.mod in your current folder.

Think of it as planting a flag that says “this folder is a Go project.” Before this, Go had no idea your folder was a project, which is why GOMOD showed /dev/null.

blackhatgo is the module name. It’s the prefix Go uses to identify your project. Later, if you create a package at lesson05/models, you’ll import it as blackhatgo/lesson05/models.

It prints one line, something like go: creating new go.mod: module blackhatgo. That’s the only output.

2. go env GOMOD

Action: asks Go “which go.mod file are you using right now?”

This is your verification. Before, it printed /dev/null (meaning none). After the init, it should print the full path:

/home/jackal/Jackal-Arcanum/Jackal-Citadel/Black-Hat-Go/go.mod

cat go.mod

Action: prints the contents of the file so you can read what Go created.

Output:
jackal@jackal:~/Jackal-Arcanum/Jackal-Citadel/Black-Hat-Go$ cat go.mod
module blackhatgo

go 1.22.2
jackal@jackal:~/Jackal-Arcanum/Jackal-Citadel/Black-Hat-Go$
```
