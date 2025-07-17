package main

import (
	"fmt"
	"log"

	"github.com/augists/Maglev-CHash/go"
)

const (
	// N is the total number of nodes in the healthy state.
	N = 100
	// M is the size of the Maglev lookup table.
	M = 65537
)

func main() {
	// Generate node names
	nodes := make([]string, N)
	for i := 0; i < N; i++ {
		nodes[i] = fmt.Sprintf("backend-%03d", i)
	}

	// Create the initial, healthy Maglev table
	healthyMaglev, err := maglev.New(nodes, M)
	if err != nil {
		log.Fatalf("Failed to create healthy Maglev table: %v", err)
	}

	fmt.Println("k,disruption_percent")

	// Simulate removing k nodes, from 1 to N-1
	for k := 1; k < N; k++ {
		// The first k nodes are considered "failed"
		failedNodes := make(map[string]bool)
		for i := 0; i < k; i++ {
			failedNodes[nodes[i]] = true
		}

		// Create the list of healthy nodes for the unhealthy table
		unhealthyNodes := nodes[k:]

		unhealthyMaglev, err := maglev.New(unhealthyNodes, M)
		if err != nil {
			log.Fatalf("Failed to create unhealthy Maglev table for k=%d: %v", k, err)
		}

		// Now, compare the lookup tables
		disruptions := 0
		for i := 0; i < M; i++ {
			healthyNodeIndex := healthyMaglev.Lookup[i]
			healthyNodeName := nodes[healthyNodeIndex]

			// If the original entry pointed to a now-failed node, we don't count it as a disruption.
			// This is what the paper's experiment measures: "minimal disruption to the lookup table".
			if _, isFailed := failedNodes[healthyNodeName]; isFailed {
				continue
			}

			unhealthyNodeIndex := unhealthyMaglev.Lookup[i]
			// The unhealthyMaglev.nodes slice is a sub-slice of the original nodes list (nodes[k:])
			unhealthyNodeName := unhealthyNodes[unhealthyNodeIndex]

			if healthyNodeName != unhealthyNodeName {
				disruptions++
			}
		}
		
		// Total entries that did not point to a failed node initially
		validEntries := M
		for i := 0; i < M; i++ {
			healthyNodeIndex := healthyMaglev.Lookup[i]
			healthyNodeName := nodes[healthyNodeIndex]
			if _, isFailed := failedNodes[healthyNodeName]; isFailed {
				validEntries--
			}
		}

		disruptionPercent := 0.0
		if validEntries > 0 {
			disruptionPercent = float64(disruptions) / float64(validEntries) * 100.0
		}

		fmt.Printf("%d,%.4f\n", k, disruptionPercent)
	}
}
