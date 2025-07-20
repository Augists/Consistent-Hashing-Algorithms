package main

import (
	"fmt"
	// Removed "os" import as it's no longer needed for file operations
	// Removed "strconv" import as it's not used
	"github.com/augists/consistent-hashing-algorithms/jump-consistent-hashing/go/jumphash"
)

const (
	numNodes = 10
	numKeys  = 1000000
	remapKeys = 10000 // Number of keys to test for remapping
)

func main() {
	// --- Key Distribution Experiment ---
	countsDist := make([]int, numNodes)

	for i := 0; i < numKeys; i++ {
		bucket := jumphash.Hash(uint64(i), int32(numNodes))
		if bucket >= 0 && bucket < numNodes {
			countsDist[bucket]++
		}
	}

	// Print distribution results to stdout
	fmt.Println("JUMP_DIST_START")
	fmt.Println("Node,Keys,Algorithm")
	for i, count := range countsDist {
		fmt.Printf("node%d,%d,jump_go\n", i, count)
	}
	fmt.Println("JUMP_DIST_END")

	// --- Remapping Experiment ---
	initialBucket := make([]int32, remapKeys)
	remappedOnRemove := 0
	remappedOnAdd := 0

	// Store initial mappings
	for i := 0; i < remapKeys; i++ {
		initialBucket[i] = jumphash.Hash(uint64(i), int32(numNodes))
	}

	// Remove one node (simulate by reducing num_nodes)
	numBucketsAfterRemove := int32(numNodes - 1)

	// Count remappings after removal
	for i := 0; i < remapKeys; i++ {
		newBucket := jumphash.Hash(uint64(i), numBucketsAfterRemove)
		if initialBucket[i] != newBucket {
			remappedOnRemove++
		}
	}

	// Add the node back (simulate by restoring num_nodes)
	numBucketsAfterAdd := int32(numNodes)

	// Count remappings after addition
	for i := 0; i < remapKeys; i++ {
		newBucket := jumphash.Hash(uint64(i), numBucketsAfterAdd)
		if initialBucket[i] != newBucket {
			remappedOnAdd++
		}
	}

	// Print remapping results to stdout
	fmt.Println("JUMP_REMAP_START")
	fmt.Println("Operation,RemappedKeys,TotalKeys,Algorithm")
	fmt.Printf("Remove,%d,%d,jump_go\n", remappedOnRemove, remapKeys)
	fmt.Printf("Add,%d,%d,jump_go\n", remappedOnAdd, remapKeys)
	fmt.Println("JUMP_REMAP_END")
}