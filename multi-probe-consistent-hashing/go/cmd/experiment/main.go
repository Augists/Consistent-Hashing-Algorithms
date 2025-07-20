package main

import (
	"fmt"
	"os"

	"github.com/augists/consistent-hashing-algorithms/multi-probe-consistent-hashing/go"
)

const (
	numNodes = 10
	numKeys  = 100000
	replicas = 20
	probes   = 5
)

func main() {
	hr := multiprobe.NewMultiProbeHashRing(replicas, probes)
	nodes := make([]string, numNodes)
	counts := make(map[string]int)

	// Add nodes
	for i := 0; i < numNodes; i++ {
		nodeName := fmt.Sprintf("node%d", i)
		nodes[i] = nodeName
		counts[nodeName] = 0
		hr.AddNode(nodeName)
	}

	// Hash keys and count distribution
	for i := 0; i < numKeys; i++ {
		key := fmt.Sprintf("key%d", i)
		node := hr.GetNode(key)
		counts[node]++
	}

	// Output results to CSV
	file, err := os.OpenFile("results.csv", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	for _, nodeName := range nodes {
		_, err := file.WriteString(fmt.Sprintf("%s,%d,multiprobe_go\n", nodeName, counts[nodeName]))
		if err != nil {
			panic(err)
		}
	}

	fmt.Println("Go experiment finished. Results appended to results.csv")
}
