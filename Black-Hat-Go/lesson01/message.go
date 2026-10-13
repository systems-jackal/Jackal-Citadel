// Same folder as main.go, so the package name must match: "main".
// Mixing package names in one folder is a compile error.
package main

// This file uses fmt itself, so it imports fmt itself.
// Importing it in main.go does not make it available here.
import "fmt"

// receiveMessage prints a message as received.
// It starts with a lowercase letter, so it is private to this package.
// A capital letter (ReceiveMessage) would make it visible to other packages.
func receiveMessage(msg string) {
	fmt.Println("Received Message:", msg)
}
