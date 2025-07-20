package main

import (
	"fmt"
	"log"
	"strconv" // Added for key generation

	"github.com/augists/Maglev-CHash/go" // Assuming this is the correct import for the maglev package
)

const (
	// N_DIST is the number of nodes for the distribution experiment.
	N_DIST = 10
	// M_DIST is the size of the Maglev lookup table for distribution experiment.
	M_DIST = 65537
	// NUM_KEYS_DISTRIBUTION is the number of keys for the distribution experiment.
	NUM_KEYS_DISTRIBUTION = 1000000

	// N_K_FAILURE is the total number of nodes in the healthy state for k-failure experiment.
	N_K_FAILURE = 100
	// M_K_FAILURE is the size of the Maglev lookup table for k-failure experiment.
	M_K_FAILURE = 65537
)

func main() {
	// --- Key Distribution Experiment ---
	distNodes := make([]string, N_DIST)
	for i := 0; i < N_DIST; i++ {
		distNodes[i] = fmt.Sprintf("node%d", i)
	}

	distMaglev, err := maglev.New(distNodes, M_DIST)
	if err != nil {
		log.Fatalf("Failed to create Maglev table for distribution experiment: %v", err)
	}

	countsDist := make(map[string]int)
	for i := 0; i < N_DIST; i++ {
		countsDist[distNodes[i]] = 0
	}

	for i := 0; i < NUM_KEYS_DISTRIBUTION; i++ {
		key := "key-" + strconv.Itoa(i)
		assignedNode := distMaglev.Get(key)
		countsDist[assignedNode]++
	}

	fmt.Println("MAGLEV_DIST_START")
	fmt.Println("Node,Keys,Algorithm")
	for i := 0; i < N_DIST; i++ {
		fmt.Printf("node%d,%d,maglev_go\n", i, countsDist[distNodes[i]])
	}
	fmt.Println("MAGLEV_DIST_END")


	// --- k-Failure Disruption Experiment (Existing, modified for stdout) ---
	// Generate node names for k-failure experiment
	kFailureNodes := make([]string, N_K_FAILURE)
	for i := 0; i < N_K_FAILURE; i++ {
		kFailureNodes[i] = fmt.Sprintf("backend-%03d", i)
	}

	healthyMaglev, err := maglev.New(kFailureNodes, M_K_FAILURE)
	if err != nil {
		log.Fatalf("Failed to create healthy Maglev table: %v", err)
	}

	fmt.Println("MAGLEV_REMAP_START")
	fmt.Println("k,disruption_percent,Algorithm") // Added Algorithm column

	// Simulate removing k nodes, from 1 to N_K_FAILURE-1
	for k := 1; k < N_K_FAILURE; k++ {
		failedNodes := make(map[string]bool)
		for i := 0; i < k; i++ {
			failedNodes[kFailureNodes[i]] = true
		}

		unhealthyNodes := kFailureNodes[k:]

		unhealthyMaglev, err := maglev.New(unhealthyNodes, M_K_FAILURE)
		if err != nil {
			log.Fatalf("Failed to create unhealthy Maglev table for k=%d: %v", k, err)
		}

		disruptions := 0
		for i := 0; i < M_K_FAILURE; i++ {
			healthyNodeIndex := healthyMaglev.Lookup[i]
			healthyNodeName := kFailureNodes[healthyNodeIndex]

			if _, isFailed := failedNodes[healthyNodeName]; isFailed {
				continue
			}

			unhealthyNodeIndex := unhealthyMaglev.Lookup[i]
			unhealthyNodeName := unhealthyNodes[unhealthyNodeIndex]

			if healthyNodeName != unhealthyNodeName {
				disruptions++
			}
		}
		
		validEntries := M_K_FAILURE
		for i := 0; i < M_K_FAILURE; i++ {
			healthyNodeIndex := healthyMaglev.Lookup[i]
			healthyNodeName := kFailureNodes[healthyNodeIndex]
			if _, isFailed := failedNodes[healthyNodeName]; isFailed {
				validEntries--
			}
		}

		disruptionPercent := 0.0
		if validEntries > 0 {
			disruptionPercent = float64(disruptions) / float64(validEntries) * 100.0
		}

		fmt.Printf("%d,%.4f,maglev_go\n", k, disruptionPercent) // Added Algorithm column

		// No explicit free for Go, garbage collector handles it
	}
	fmt.Println("MAGLEV_REMAP_END")
}