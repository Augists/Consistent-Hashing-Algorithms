package main

import (
	"fmt"
	"github.com/augists/consistent-hashing-algorithms/hash-ring-consistent-hashing/ketama-hashing/go"
)

func main() {
	kr := ketama.NewKetamaRing(160) // 160 virtual nodes per real node

	// Add nodes
	kr.AddNode("node1")
	kr.AddNode("node2")
	kr.AddNode("node3")
	kr.AddNode("node4")
	kr.AddNode("node5")

	// Get a key
	key := "my-test-key"
	node := kr.GetNode(key)

	if node != "" {
		fmt.Printf("Key '%s' is mapped to node '%s'\n", key, node)
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}

	// Remove a node and test again
	kr.RemoveNode("node3")
	fmt.Println("\nAfter removing 'node3':")
	node = kr.GetNode(key)

	if node != "" {
		fmt.Printf("Key '%s' is now mapped to node '%s'\n", key, node)
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}
}
