package main

import (
	"fmt"
	"strconv" // Keep strconv for key generation

	"github.com/augists/consistent-hashing-algorithms/hash-ring-consistent-hashing/karger-hash-ring/go"
)

const (
	numNodes  = 10
	numKeys   = 1000000
	replicas = 100 // Number of virtual nodes per real node
	remapKeys = 10000 // Number of keys to test for remapping
)

func main() {
	// --- Key Distribution Experiment ---
	hrDist := consistenthashing.NewHashRing(replicas)
	nodesDist := make([]string, numNodes)
	countsDist := make(map[string]int)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		nodesDist[i] = nodeName
		countsDist[nodeName] = 0
		hrDist.AddNode(nodeName)
	}

	for i := 0; i < numKeys; i++ {
		key := "key" + strconv.Itoa(i)
		node := hrDist.GetNode(key)
		if node == "" {
			panic(fmt.Sprintf("Node not found for key %s", key))
		}
		countsDist[node]++
	}

	// Print distribution results to stdout
	fmt.Println("KARGER_DIST_START")
	fmt.Println("Node,Keys,Algorithm")
	for _, nodeName := range nodesDist {
		fmt.Printf("%s,%d,karger_go\n", nodeName, countsDist[nodeName])
	}
	fmt.Println("KARGER_DIST_END")

	// --- Remapping Experiment ---
	hrRemap := consistenthashing.NewHashRing(replicas)
	initialNodeMappings := make(map[string]string)
	remapKeysSlice := make([]string, remapKeys)

	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		hrRemap.AddNode(nodeName)
	}

	for i := 0; i < remapKeys; i++ {
		key := "remap_key" + strconv.Itoa(i)
		remapKeysSlice[i] = key
		node := hrRemap.GetNode(key)
		if node == "" {
			panic(fmt.Sprintf("Node not found for key %s", key))
		}
		initialNodeMappings[key] = node
	}

	// Remove one node (e.g., "node9")
	hrRemap.RemoveNode("node9")

	remappedOnRemove := 0
	for _, key := range remapKeysSlice {
		newNode := hrRemap.GetNode(key)
		if newNode == "" {
			// If a node is removed and a key can no longer be mapped, it's a remapping
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
	hrRemap.AddNode("node9")

	remappedOnAdd := 0
	for _, key := range remapKeysSlice {
		newNode := hrRemap.GetNode(key)
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
	fmt.Println("KARGER_REMAP_START")
	fmt.Println("Operation,RemappedKeys,TotalKeys,Algorithm")
	fmt.Printf("Remove,%d,%d,karger_go\n", remappedOnRemove, remapKeys)
	fmt.Printf("Add,%d,%d,karger_go\n", remappedOnAdd, remapKeys)
	fmt.Println("KARGER_REMAP_END")
}