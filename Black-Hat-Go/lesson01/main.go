// Lesson 01: the structure of a Go source file.
//
// Every .go file has three zones, always in this order:
//   1. Package clause  - which package this file belongs to
//   2. Imports         - which other packages this file uses
//   3. Declarations    - const, var, type and func
//
// Comments are the only thing allowed before the package clause.

// ZONE 1: package clause.
// "main" means this folder builds into a runnable program.
// Any other name would build a library that other code imports.
package main

// ZONE 2: imports.
// Imports are per file: every file imports what it uses itself.
// An import that is never used is a compile error.
import "fmt"

// ZONE 3: declarations.
// The order of functions inside this zone is not enforced by Go.

// main is the entry point. Go starts running here, and only one
// func main() may exist in this folder.
func main() {
	fmt.Println("Jackal Online")

	// receiveMessage lives in message.go. No import is needed because
	// both files say "package main" and sit in the same folder, so Go
	// treats them as one package.
	receiveMessage("Black Hat Go")

	// printMessage is declared below in this same file.
	printMessage("Who am I")
}

// printMessage prints a label followed by the given text.
// fmt.Println puts a space between its arguments automatically.
func printMessage(text string) {
	fmt.Println("Anonymous", text)
}
