package rendezvous

import (
	"hash/fnv"
	"sort"
	"strconv"
)

// RendezvousHash calculates the highest random weight for a given key and a list of nodes.
// It returns the index of the chosen node.
func RendezvousHash(key string, nodes []string) int {
	if len(nodes) == 0 {
		return -1 // No nodes available
	}

	maxWeight := -1.0
	chosenIdx := -1

	for i, node := range nodes {
		// Combine key and node name to create a unique string for hashing
		combined := key + "-" + node
		h := fnv.New64a()
		h.Write([]byte(combined))
		hashValue := float64(h.Sum64())

		// A simple way to get a "weight" from the hash value.
		// For true HRW, a more robust pseudo-random number generator seeded by the combined hash
		// would be used, often involving floating-point numbers for better distribution.
		// For demonstration, a direct use of hashValue is sufficient to show the principle.
		weight := hashValue // Using hash value directly as weight for simplicity

		if weight > maxWeight {
			maxWeight = weight
			chosenIdx = i
		}
	}
	return chosenIdx
}
