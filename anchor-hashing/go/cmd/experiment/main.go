package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"sort"

	anchorhash "github.com/Augists/Consistent-Hashing-Algorithms/anchor-hashing/go" // Assuming the package name is jumphash
)

const (
	InitialNodes = 1000
	NumKeys      = 1000000 // Increased keys for better statistical significance with more nodes

	// For remapping curve experiments
	RemovalSteps               = 11 // 0% to 50% removal in 5% increments
	AdditionSteps              = 11 // 0% to 50% addition in 5% increments (relative to removed nodes)
	RemovePercentageForAddition = 0.20 // Remove 20% of nodes before adding back
)

func main() {
	// Peak-to-Average Ratio Experiment
	h := anchorhash.NewAnchorHash(InitialNodes, InitialNodes)
	counts := make([]int, InitialNodes)
	for k := uint64(0); k < NumKeys; k++ {
		b := h.Map(k)
		if b >= 0 {
			counts[b]++
		}
	}
	vals := make([]int, 0, InitialNodes)
	for i := 0; i < InitialNodes; i++ {
		if h.A[i] {
			vals = append(vals, counts[i])
		}
	}
	sort.Ints(vals)
	peak := vals[len(vals)-1]
	sum := 0
	for _, v := range vals {
		sum += v
	}
	avg := float64(sum) / float64(len(vals))
	peakToAvg := float64(peak) / avg

	// Write peak-to-average data
	fPeak, err := os.Create("../../results/anchor_experiment.csv")
	if err != nil {
		fPeak, err = os.Create("../results/anchor_experiment.csv")
		if err != nil {
			fmt.Println("Cannot open results/anchor_experiment.csv for write!")
			return
		}
	}
	wPeak := csv.NewWriter(fPeak)
	wPeak.Write([]string{"peak_to_average", fmt.Sprintf("%.4f", peakToAvg)})
	wPeak.Flush()
	fPeak.Close()
	fmt.Println("Peak/average data written to results/anchor_experiment.csv")

	// Remapping Ratio Experiment (Node Removal)
	remapRemovalExperiment()

	// Remapping Ratio Experiment (Node Addition)
	remapAdditionExperiment()
}

func remapRemovalExperiment() {
	f, err := os.Create("../../results/anchor_remap_removal_curve.csv")
	if err != nil {
		f, err = os.Create("../results/anchor_remap_removal_curve.csv")
		if err != nil {
			fmt.Println("Cannot open results/anchor_remap_removal_curve.csv for write!")
			return
		}
	}
	w := csv.NewWriter(f)
	w.Write([]string{"remove_ratio", "remap_ratio"})

	for step := 0; step < RemovalSteps; step++ {
		removeRatio := float64(step) / float64(RemovalSteps-1) * 0.5 // 0% to 50%
		removeCnt := int(float64(InitialNodes) * removeRatio)
		if removeCnt > InitialNodes {
			removeCnt = InitialNodes
		}

		h2 := anchorhash.NewAnchorHash(InitialNodes, InitialNodes)
		before := make([]int, NumKeys)
		for k := uint64(0); k < NumKeys; k++ {
			before[k] = h2.Map(k)
		}

		removedActual := 0
		for i := 0; i < InitialNodes && removedActual < removeCnt; i++ {
			if h2.A[i] { // Only remove if active
				h2.Remove(i)
				removedActual++
			}
		}

		changed := 0
		for k := uint64(0); k < NumKeys; k++ {
			after := h2.Map(k)
			if after != before[k] {
				changed++
			}
		}
		remapRatio := float64(changed) / float64(NumKeys)
		w.Write([]string{fmt.Sprintf("%.2f", float64(removedActual)/InitialNodes), fmt.Sprintf("%.4f", remapRatio)})
	}
	w.Flush()
	f.Close()
	fmt.Println("Remapping removal curve data written to results/anchor_remap_removal_curve.csv")
}

func remapAdditionExperiment() {
	f, err := os.Create("../../results/anchor_remap_addition_curve.csv")
	if err != nil {
		f, err = os.Create("../results/anchor_remap_addition_curve.csv")
		if err != nil {
			fmt.Println("Cannot open results/anchor_remap_addition_curve.csv for write!")
			return
		}
	}
	w := csv.NewWriter(f)
	w.Write([]string{"add_ratio", "remap_ratio"})

	// Initial state: remove a fixed percentage of nodes
	initialRemovedCount := int(float64(InitialNodes) * RemovePercentageForAddition)
	if initialRemovedCount == 0 && InitialNodes > 0 {
		initialRemovedCount = 1
	}
	if initialRemovedCount > InitialNodes {
		initialRemovedCount = InitialNodes
	}

	baseCtx := anchorhash.NewAnchorHash(InitialNodes, InitialNodes)
	initialMappings := make([]int, NumKeys)
	for k := uint64(0); k < NumKeys; k++ {
		initialMappings[k] = baseCtx.Map(k)
	}

	// Perform initial removal
	removedIndices := make([]int, 0, initialRemovedCount)
	for i := 0; i < InitialNodes && len(removedIndices) < initialRemovedCount; i++ {
		if baseCtx.A[i] {
			baseCtx.Remove(i)
			removedIndices = append(removedIndices, i)
		}
	}

	// Store mappings after initial removal
	mappingsAfterInitialRemoval := make([]int, NumKeys)
	for k := uint64(0); k < NumKeys; k++ {
		mappingsAfterInitialRemoval[k] = baseCtx.Map(k)
	}

	// Now, add nodes back iteratively
	for step := 0; step < AdditionSteps; step++ {
		addRatio := float64(step) / float64(AdditionSteps-1) // 0% to 100% of removed nodes added back
		addCnt := int(float64(initialRemovedCount) * addRatio)
		if addCnt > initialRemovedCount {
			addCnt = initialRemovedCount
		}

		// Create a new context for each step to ensure clean state
		currentCtx := anchorhash.NewAnchorHash(InitialNodes, InitialNodes-initialRemovedCount) // Start with removed nodes

		// Re-remove nodes for current step
		for _, idx := range removedIndices {
			currentCtx.Remove(idx)
		}

		// Add back nodes for current step
		for i := 0; i < addCnt; i++ {
			currentCtx.Add(removedIndices[i])
		}

		changed := 0
		for k := uint64(0); k < NumKeys; k++ {
			currentMap := currentCtx.Map(k)
			if currentMap != mappingsAfterInitialRemoval[k] {
				changed++
			}
		}
		w.Write([]string{fmt.Sprintf("%.2f", float64(addCnt)/floatMap(initialRemovedCount)), fmt.Sprintf("%.4f", float64(changed)/NumKeys)})
	}
	w.Flush()
	f.Close()
	fmt.Println("Remapping addition curve data written to results/anchor_remap_addition_curve.csv")
}

// Helper function to avoid division by zero if initialRemovedCount is 0
func floatMap(val int) float64 {
	if val == 0 {
		return 1.0 // Avoid division by zero, effectively 100% if no nodes were removed
	}
	return float64(val)
}