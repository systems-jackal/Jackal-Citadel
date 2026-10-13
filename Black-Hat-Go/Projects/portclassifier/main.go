package main

import (
	"fmt"

	"blackhatgo/Projects/portkit"
)

func main() {
	fmt.Println("PORT   CLASS        SERVICE")
	show(22)
	show(80)
	show(443)
	show(3306)
	show(8080)
	show(51000)
	show(70000) // invalid: above 65535
	show(-5)    // invalid: below 1
}

func show(p int) {
	fmt.Printf("%-6d %-12s %s\n", p, portkit.Classify(p), portkit.ServiceName(p))
}
