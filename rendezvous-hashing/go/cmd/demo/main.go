package main

import (
	"fmt"
	"github.com/augists/consistent-hashing-algorithms/rendezvous-hashing/go"
	"sort"
)

func main() {
	fmt.Println("Rendezvous Hashing Demonstration")

	initialNodes := []string{"serverA", "serverB", "serverC"}
	fmt.Printf("Initial Nodes: %v\n", initialNodes)

	key := "my-test-key"
	assignedIdx := rendezvous.RendezvousHash(key, initialNodes)

	if assignedIdx != -1 {
		fmt.Printf("Key '%s' is mapped to node '%s'\n", key, initialNodes[assignedIdx])
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}

	// Demonstrate adding a node
	fmt.Println("\nAdding serverD...")
	nodesAfterAdd := append(initialNodes, "serverD")
	sort.Strings(nodesAfterAdd) // Keep nodes sorted for consistent iteration (though not strictly required by HRW)

	assignedIdx = rendezvous.RendezvousHash(key, nodesAfterAdd)
	if assignedIdx != -1 {
		fmt.Printf("Key '%s' is now mapped to node '%s'\n", key, nodesAfterAdd[assignedIdx])
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key)
	}

	// Demonstrate removing a node
	fmt.Println("\nRemoving serverB...")
	var finalNodes []string
	for _, node := range nodesAfterAdd {
		if node != "serverB" {
			finalNodes = append(finalNodes, node)
		}
	}
	sort.Strings(finalNodes)

	assignedIdx = rendezvous.RendezvousHash(key, finalNodes)
	if assignedIdx != -1 {
		fmt.Printf("Key '%s' is now mapped to node '%s'\n", key, finalNodes[assignedIdx])
	} else {
		fmt.Printf("Could not find a node for key '%s'\n", key);
	}
}
