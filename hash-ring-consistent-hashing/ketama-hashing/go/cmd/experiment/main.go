package main

import (
	"fmt"
	"strconv" // Keep strconv for key generation

	"github.com/augists/consistent-hashing-algorithms/hash-ring-consistent-hashing/ketama-hashing/go"
)

const (
	numNodes          = 10
	numKeys           = 1000000
	numVirtualNodes = 160 // Number of virtual nodes per real node
	remapKeys         = 10000 // Number of keys to test for remapping
)

func main() {
	// --- Key Distribution Experiment ---
	krDist := ketama.NewKetamaRing(numVirtualNodes)
	nodesDist := make([]string, numNodes)
	countsDist := make(map[string]int)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		nodesDist[i] = nodeName
		countsDist[nodeName] = 0
		krDist.AddNode(nodeName)
	}

	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := krDist.GetNode(key)
		if node == "" {
			panic(fmt.Sprintf("Node not found for key %s", key))
		}
		countsDist[node]++
	}

	// Print distribution results to stdout
	fmt.Println("KETAMA_DIST_START")
	fmt.Println("Node,Keys,Algorithm")
	for _, nodeName := range nodesDist {
		fmt.Printf("%s,%d,ketama_go\n", nodeName, countsDist[nodeName])
	}
	fmt.Println("KETAMA_DIST_END")

	// --- Remapping Experiment ---
	krRemap := ketama.NewKetamaRing(numVirtualNodes)
	initialNodeMappings := make(map[string]string)
	remapKeysSlice := make([]string, remapKeys)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		krRemap.AddNode(nodeName)
	}

	for i := 0; i < remapKeys; i++ {
		key := "remap_key" + strconv.Itoa(i)
		remapKeysSlice[i] = key
		node := krRemap.GetNode(key)
		if node == "" {
			panic(fmt.Sprintf("Node not found for key %s", key))
		}
		initialNodeMappings[key] = node
	}

	// Remove one node (e.g., "node9")
	krRemap.RemoveNode("node9")

	remappedOnRemove := 0
	for _, key := range remapKeysSlice {
		newNode := krRemap.GetNode(key)
		if newNode == "" {
			if initialNodeMappings[key] != "" {
				remappedOnRemove++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnRemove++
		}
	}

	// Add the node back
	krRemap.AddNode("node9")

	remappedOnAdd := 0
	for _, key := range remapKeysSlice {
		newNode := krRemap.GetNode(key)
		if newNode == "" {
			if initialNodeMappings[key] != "" {
				remappedOnAdd++
			}
			continue
		}
		if initialNodeMappings[key] != newNode {
			remappedOnAdd++
		}
	}

	// Print remapping results to stdout
	fmt.Println("KETAMA_REMAP_START")
	fmt.Println("Operation,RemappedKeys,TotalKeys,Algorithm")
	fmt.Printf("Remove,%d,%d,ketama_go\n", remappedOnRemove, remapKeys)
	fmt.Printf("Add,%d,%d,ketama_go\n", remappedOnAdd, remapKeys)
	fmt.Println("KETAMA_REMAP_END")
}