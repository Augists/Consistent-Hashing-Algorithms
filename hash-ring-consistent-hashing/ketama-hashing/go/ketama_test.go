package ketama

import (
	"fmt"
	"strconv"
	"testing"
)

func TestKetamaRing(t *testing.T) {
	// Using 160 virtual nodes (40 * 4 bytes from MD5)
	kr := NewKetamaRing(160)

	// Add nodes
	kr.AddNode("nodeA")
	kr.AddNode("nodeB")
	kr.AddNode("nodeC")

	// Test distribution
	keyCounts := make(map[string]int)
	numKeys := 100000 // More keys for better distribution observation
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := kr.GetNode(key)
		keyCounts[node]++
	}

	fmt.Println("Initial distribution:")
	for node, count := range keyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Add a new node and observe re-distribution
	fmt.Println("\nAdding nodeD...")
	kr.AddNode("nodeD")

	newKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := kr.GetNode(key)
		newKeyCounts[node]++
	}

	fmt.Println("Distribution after adding nodeD:")
	for node, count := range newKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Remove a node and observe re-distribution
	fmt.Println("\nRemoving nodeB...")
	kr.RemoveNode("nodeB")

	finalKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := kr.GetNode(key)
		finalKeyCounts[node]++
	}

	fmt.Println("Distribution after removing nodeB:")
	for node, count := range finalKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Test consistency for existing keys after node changes
	fmt.Println("\nTesting consistency for a few keys:")
	keysToObserve := []string{"key100", "key500", "key9999"}
	for _, key := range keysToObserve {
		initialNode := kr.GetNode(key)
		fmt.Printf("  Key %s maps to %s\n", key, initialNode)
	}
}