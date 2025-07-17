package consistenthashing

import (
	"fmt"
	"math/rand"
	"strconv"
	"testing"
	time "time"
)

func TestHashRing(t *testing.T) {
	hashRing := NewHashRing(3) // 3 virtual nodes per real node

	// Add nodes
	hashRing.AddNode("nodeA")
	hashRing.AddNode("nodeB")
	hashRing.AddNode("nodeC")

	// Test distribution
	keyCounts := make(map[string]int)
	numKeys := 10000
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := hashRing.GetNode(key)
		keyCounts[node]++
	}

	fmt.Println("Initial distribution:")
	for node, count := range keyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Add a new node and observe re-distribution
	fmt.Println("\nAdding nodeD...")
	hashRing.AddNode("nodeD")

	newKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := hashRing.GetNode(key)
		newKeyCounts[node]++
	}

	fmt.Println("Distribution after adding nodeD:")
	for node, count := range newKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Remove a node and observe re-distribution
	fmt.Println("\nRemoving nodeB...")
	hashRing.RemoveNode("nodeB")

	finalKeyCounts := make(map[string]int)
	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := hashRing.GetNode(key)
		finalKeyCounts[node]++
	}

	fmt.Println("Distribution after removing nodeB:")
	for node, count := range finalKeyCounts {
		fmt.Printf("  %s: %d (%.2f%%)\n", node, count, float64(count)/float64(numKeys)*100)
	}

	// Test consistency for existing keys after node changes
	fmt.Println("\nTesting consistency for a few keys:")
	rand.Seed(time.Now().UnixNano())
	for i := 0; i < 5; i++ {
		key := "key" + strconv.Itoa(rand.Intn(numKeys))
		initialNode := hashRing.GetNode(key)
		fmt.Printf("  Key %s maps to %s\n", key, initialNode)
	}
}