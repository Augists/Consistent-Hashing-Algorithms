package main

import (
	"fmt"
	"hash/fnv"
	"sort"
	"strconv"
)

// RendezvousHash calculates the highest random weight for a given key and a list of nodes.
// It returns the index of the chosen node.
func RendezvousHash(key string, nodes []string) int {
	if len(nodes) == 0 {
		return -1 // No nodes available
	}

	maxWeight := -1.0
	chosenIdx := -1

	for i, node := range nodes {
		// Combine key and node name to create a unique string for hashing
		combined := key + "-" + node
		h := fnv.New64a()
		h.Write([]byte(combined))
		hashValue := float64(h.Sum64())

		// A simple way to get a "weight" from the hash value.
		// For true HRW, a more robust pseudo-random number generator seeded by the combined hash
		// would be used, often involving floating-point numbers for better distribution.
		// For demonstration, a direct use of hashValue is sufficient to show the principle.
		weight := hashValue // Using hash value directly as weight for simplicity

		if weight > maxWeight {
			maxWeight = weight
			chosenIdx = i
		}
	}
	return chosenIdx
}

func main() {
	fmt.Println("Rendezvous Hashing Demonstration")

	nodes := []string{"serverA", "serverB", "serverC"}
	fmt.Printf("Initial Nodes: %v\n", nodes)

	keyCounts := make(map[string]int)
	numKeys := 10000
	for i := 0; i < numKeys; i++ {
		key := "data_key_" + strconv.Itoa(i)
		idx := RendezvousHash(key, nodes)
		if idx != -1 {
			keyCounts[nodes[idx]]++
		}
	}

	fmt.Println("\nInitial Distribution:")
	for node, count := range keyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Add a new node
	fmt.Println("\nAdding serverD...")
	newNodes := append(nodes, "serverD")
	sort.Strings(newNodes) // Keep nodes sorted for consistent iteration (though not strictly required by HRW)

	newKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "data_key_" + strconv.Itoa(i)
		idx := RendezvousHash(key, newNodes)
		if idx != -1 {
			newKeyCounts[newNodes[idx]]++
		}
	}

	fmt.Println("Distribution after adding serverD:")
	for node, count := range newKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Observe reassignments
	fmt.Println("\nObserving reassignments for a few keys:")
	keysToObserve := []string{"data_key_100", "data_key_500", "data_key_9999"}
	for _, key := range keysToObserve {
		initialNodeIdx := RendezvousHash(key, nodes)
		newNodeIdx := RendezvousHash(key, newNodes)
		initialNode := "(none)"
		if initialNodeIdx != -1 {
			initialNode = nodes[initialNodeIdx]
		}
		newNode := "(none)"
		if newNodeIdx != -1 {
			newNode = newNodes[newNodeIdx]
		}
		fmt.Printf("  Key %s: Initial -> %s, After adding serverD -> %s\n", key, initialNode, newNode)
	}

	// Remove a node
	fmt.Println("\nRemoving serverB...")
	var finalNodes []string
	for _, node := range newNodes {
		if node != "serverB" {
			finalNodes = append(finalNodes, node)
		}
	}
	sort.Strings(finalNodes)

	finalKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "data_key_" + strconv.Itoa(i)
		idx := RendezvousHash(key, finalNodes)
		if idx != -1 {
			finalKeyCounts[finalNodes[idx]]++
		}
	}

	fmt.Println("Distribution after removing serverB:")
	for node, count := range finalKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}
}