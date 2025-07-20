package main

import (
	"fmt"
	"github.com/augists/consistent-hashing-algorithms/anchor-hashing/go"
)

func main() {
	h := anchorhash.New()

	// Add nodes
	h.Add("node1", "node2", "node3", "node4", "node5")

	// Get a key
	key := "my-test-key"
	node, err := h.Get(key)
	if err != nil {
		fmt.Printf("Error getting node: %v\n", err)
		return
	}
	fmt.Printf("Key '%s' is mapped to node '%s'\n", key, node)

	// Remove a node and test again
	h.Remove("node3")
	fmt.Println("\nAfter removing 'node3':")
	node, err = h.Get(key)
	if err != nil {
		fmt.Printf("Error getting node: %v\n", err)
		return
	}
	fmt.Printf("Key '%s' is now mapped to node '%s'\n", key, node)
}
