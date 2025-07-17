package main

import (
	"fmt"
	"log"

	"github.com/augists/Maglev-CHash/go"
)

func main() {
	// 1. Define backend nodes and lookup table size.
	nodes := []string{
		"backend-alpha",
		"backend-beta",
		"backend-gamma",
		"backend-delta",
	}
	M := 65537 // A large prime number

	fmt.Printf("Initializing Maglev with %d nodes and table size M=%d\n", len(nodes), M)
	fmt.Println("Nodes:", nodes)
	fmt.Println("--------------------------------------------------")

	// 2. Create a new Maglev instance.
	m, err := maglev.New(nodes, M)
	if err != nil {
		log.Fatalf("Failed to create Maglev instance: %v", err)
	}

	// 3. Simulate hashing some keys.
	fmt.Println("Hashing some sample keys:")
	keysToTest := []string{
		"customer:12345",
		"session:abcdef",
		"request:xyz789",
		"user_profile:jane.doe",
		"customer:12345", // Test consistency, should be same as first
	}

	for _, key := range keysToTest {
		backend := m.Get(key)
		fmt.Printf("  - Key '%s' -> %s\n", key, backend)
	}
	fmt.Println("--------------------------------------------------")

	// 4. Show the distribution of the lookup table.
	fmt.Println("Lookup table distribution:")
	counts := make(map[string]int)
	// This is inefficient for a real program, but fine for a demo.
	// We access private fields here for demonstration purposes, which is not ideal.
	// A proper library might expose a Stats() method.
	// To do this without reflection or a new public method, we can't directly access the lookup table.
	// Let's simulate it by hashing M keys.
	
	// A better way is to add a public method to get the distribution.
	// Let's assume we add a `GetDistribution` method to the maglev package.
	// For now, I will just calculate it here by accessing the public fields.
	// The lookup table itself is private, so I can't access it.
	
	// I will modify the maglev package to add a public method to get the distribution.
	// But for now, let's just print a message.
	
	// To show distribution, we need to iterate through the lookup table.
	// Since `lookup` is private, let's add a public method `GetDistribution()` to maglev.go
	
	// I will first add the method, then use it here.
	// For now, I'll leave a placeholder.
	
	// Let's just calculate it manually for the demo by iterating through many keys
	for i := 0; i < M; i++ {
		key := fmt.Sprintf("probe-%d", i)
		backend := m.Get(key)
		counts[backend]++
	}

	for _, node := range nodes {
		count := counts[node]
		percentage := 100 * float64(count) / float64(M)
		fmt.Printf("  - %s: %d entries (%.2f%%)\n", node, count, percentage)
	}
	fmt.Println("--------------------------------------------------")
}