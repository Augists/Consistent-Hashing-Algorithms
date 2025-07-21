package main

import (
	"dxhash"
	"encoding/csv"
	"fmt"
	"os"
)

func main() {
	const (
		NumBuckets = 64
		NumActive  = 64
		NumKeys    = 100000
		Steps      = 11
	)

	h := dxhash.NewDxHash(NumBuckets, NumActive)
	before := make([]int, NumKeys)
	for k := uint64(0); k < NumKeys; k++ {
		before[k] = h.Map(k)
	}

	f, err := os.Create("results/dx_remap_curve.csv")
	if err != nil {
		fmt.Println("Cannot open results/dx_remap_curve.csv for write!")
		return
	}
	w := csv.NewWriter(f)
	w.Write([]string{"remove_ratio", "remap_ratio"})

	for step := 0; step < Steps; step++ {
		ratio := float64(step) / float64(Steps-1)
		h2 := dxhash.NewDxHash(NumBuckets, NumActive)
		removed := 0
		targetRemove := int(ratio * float64(NumBuckets))
		for i := 0; i < NumBuckets && removed < targetRemove; i++ {
			if h2.Active()[i] {
				h2.Remove(i)
				removed++
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
		w.Write([]string{fmt.Sprintf("%.2f", ratio), fmt.Sprintf("%.4f", remapRatio)})
	}
	w.Flush()
	f.Close()
	fmt.Println("Remapping curve data written to results/dx_remap_curve.csv")
}
