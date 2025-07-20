package main

import (
	"fmt"
	"github.com/augists/consistent-hashing-algorithms/hash-ring-consistent-hashing/karger-hash-ring/go"
)

func main() {
	hr := consistenthashing.NewHashRing(100) // 100 virtual nodes per real node

	// Add nodes
	hr.AddNode("node1")
	hr.AddNode("node2")
	hr.AddNode("node3")
	hr.AddNode("node4")
	hr.AddNode("node5")

	// Get a key
	key := "my-test-key"
	node := hr.GetNode(key)

	if node != "" {
		fmt.Printf("Key '%s' is mapped to node '%s'\n", key, node)
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}

	// Remove a node and test again
	hr.RemoveNode("node3")
	fmt.Println("\nAfter removing 'node3':")
	node = hr.GetNode(key)

	if node != "" {
		fmt.Printf("Key '%s' is now mapped to node '%s'\n", key, node)
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}
}
