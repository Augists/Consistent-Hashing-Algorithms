package main

import (
	"fmt"
	"os"
	

	"github.com/augists/consistent-hashing-algorithms/anchor-hashing/go"
)

const (
	numNodes   = 10
	numKeys    = 1000000
	remapKeys = 10000 // Number of keys to test for remapping
)

func main() {
	// --- Key Distribution Experiment (Existing) ---
	hDist := anchorhash.New()
	nodesDist := make([]string, numNodes)
	countsDist := make(map[string]int)

	// Add nodes
	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		nodesDist[i] = nodeName
		countsDist[nodeName] = 0
		hDist.Add(nodeName)
	}

	// Hash keys and count distribution
	for i := 0; i < numKeys; i++ {
		key := fmt.Sprintf("key%d", i)
		node, err := hDist.Get(key)
		if err != nil {
			panic(err)
		}
		countsDist[node]++
	}

	// Output distribution results to CSV
	fileDist, err := os.OpenFile("results.csv", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		panic(err)
	}
	defer fileDist.Close()

	for _, nodeName := range nodesDist {
		_, err := fileDist.WriteString(fmt.Sprintf("%s,%d,anchor_go\n", nodeName, countsDist[nodeName]))
		if err != nil {
			panic(err)
		}
	}
	fmt.Println("Go key distribution experiment finished. Results appended to results.csv")

	// --- Remapping Experiment (New) ---
	hRemap := anchorhash.New()
	initialNodeMappings := make(map[string]string)
	remapKeysSlice := make([]string, remapKeys)

	// Add initial nodes
	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		hRemap.Add(nodeName)
	}

	// Store initial mappings
	for i := 0; i < remapKeys; i++ {
		key := fmt.Sprintf("remap_key%d", i)
		remapKeysSlice[i] = key
		node, err := hRemap.Get(key)
		if err != nil {
			panic(err)
		}
		initialNodeMappings[key] = node
	}

	// Remove one node (e.g., "node9")
	fmt.Println("Removing node9...")
	hRemap.Remove("node9")

	// Count remappings after removal
	remappedOnRemove := 0
	for _, key := range remapKeysSlice {
		newNode, err := hRemap.Get(key)
		if err != nil {
			// If a node is removed and a key can no longer be mapped, it's a remapping
			if initialNodeMappings[key] != "" { // Check if it was mapped initially
				remappedOnRemove++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnRemove++
		}
	}
	fmt.Printf("Keys remapped after removal: %d\n", remappedOnRemove)

	// Add the node back
	fmt.Println("Adding node9 back...")
	hRemap.Add("node9")

	// Count remappings after addition
	remappedOnAdd := 0
	for _, key := range remapKeysSlice {
		newNode, err := hRemap.Get(key)
		if err != nil {
			// If a node is added and a key can no longer be mapped, it's a remapping
			if initialNodeMappings[key] != "" { // Check if it was mapped initially
				remappedOnAdd++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnAdd++
		}
	}
	fmt.Printf("Keys remapped after addition: %d\n", remappedOnAdd)

	// Output remapping results to CSV
	fileRemap, err := os.OpenFile("remapping_results.csv", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		panic(err)
	}
	defer fileRemap.Close()

	_, err = fileRemap.WriteString(fmt.Sprintf("Remove,%d,%d,anchor_go\n", remappedOnRemove, remapKeys))
	if err != nil {
		panic(err)
	}
	_, err = fileRemap.WriteString(fmt.Sprintf("Add,%d,%d,anchor_go\n", remappedOnAdd, remapKeys))
	if err != nil {
		panic(err)
	}
	fmt.Println("Go remapping experiment finished. Results appended to remapping_results.csv")
}
