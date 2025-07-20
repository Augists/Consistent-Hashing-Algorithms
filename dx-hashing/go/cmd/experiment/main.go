package main

import (
	"fmt"

	"github.com/augists/consistent-hashing-algorithms/dx-hashing/go"
)

const (
	numNodes  = 10
	numKeys   = 1000000
	hashSize  = 20 // Must be >= numNodes
	remapKeys = 10000 // Number of keys to test for remapping
)

func main() {
	// --- Key Distribution Experiment ---
	hDist := dxhash.New(hashSize)
	nodesDist := make([]string, numNodes)
	countsDist := make(map[string]int)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		nodesDist[i] = nodeName
		countsDist[nodeName] = 0
		hDist.Add(nodeName)
	}

	for i := 0; i < numKeys; i++ {
		key := fmt.Sprintf("key%d", i)
		node, err := hDist.Get(key)
		if err != nil {
			panic(err)
		}
		countsDist[node]++
	}

	// Print distribution results to stdout
	fmt.Println("DX_DIST_START")
	fmt.Println("Node,Keys,Algorithm")
	for _, nodeName := range nodesDist {
		fmt.Printf("%s,%d,dx_go\n", nodeName, countsDist[nodeName])
	}
	fmt.Println("DX_DIST_END")

	// --- Remapping Experiment ---
	hRemap := dxhash.New(hashSize)
	initialNodeMappings := make(map[string]string)
	remapKeysSlice := make([]string, remapKeys)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		hRemap.Add(nodeName)
	}

	for i := 0; i < remapKeys; i++ {
		key := fmt.Sprintf("remap_key%d", i)
		remapKeysSlice[i] = key
		node, err := hRemap.Get(key)
		if err != nil {
			panic(err)
		}
		initialNodeMappings[key] = node
	}

	// Removed "Removing node9..." print
	hRemap.Remove("node9")

	remappedOnRemove := 0
	for _, key := range remapKeysSlice {
		newNode, err := hRemap.Get(key)
		if err != nil {
			if initialNodeMappings[key] != "" {
				remappedOnRemove++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnRemove++
		}
	}
	// Removed "Keys remapped after removal: %d" print

	// Removed "Adding node9 back..." print
	hRemap.Add("node9")

	remappedOnAdd := 0
	for _, key := range remapKeysSlice {
		newNode, err := hRemap.Get(key)
		if err != nil {
			if initialNodeMappings[key] != "" {
				remappedOnAdd++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnAdd++
		}
	}
	// Removed "Keys remapped after addition: %d" print

	// Print remapping results to stdout
	fmt.Println("DX_REMAP_START")
	fmt.Println("Operation,RemappedKeys,TotalKeys,Algorithm")
	fmt.Printf("Remove,%d,%d,dx_go\n", remappedOnRemove, remapKeys)
	fmt.Printf("Add,%d,%d,dx_go\n", remappedOnAdd, remapKeys)
	fmt.Println("DX_REMAP_END")
}