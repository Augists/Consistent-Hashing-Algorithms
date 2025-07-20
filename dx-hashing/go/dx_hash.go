package dxhash

import (
	"fmt"
	"hash/fnv"
	"sync"
)

// DxHash represents the DxHash consistent hashing algorithm.
type DxHash struct {
	mu         sync.RWMutex
	nodes      []string
	nodeStatus []bool
	inactive   []int
	nodeMap    map[string]int
}

// New creates a new DxHash instance with a given size.
func New(size int) *DxHash {
	h := &DxHash{
		nodes:      make([]string, size),
		nodeStatus: make([]bool, size),
		inactive:   make([]int, size),
		nodeMap:    make(map[string]int),
	}
	for i := 0; i < size; i++ {
		h.inactive[i] = size - 1 - i
	}
	return h;
}

// Add adds a node to the hash.
func (h *DxHash) Add(node string) (int, error) {
	h.mu.Lock()
	defer h.mu.Unlock()

	if len(h.inactive) == 0 {
		return -1, fmt.Errorf("hash is full")
	}

	idx := h.inactive[len(h.inactive)-1]
	h.inactive = h.inactive[:len(h.inactive)-1]

	h.nodes[idx] = node
	h.nodeStatus[idx] = true
	h.nodeMap[node] = idx

	return idx, nil
}

// Remove removes a node from the hash.
func (h *DxHash) Remove(node string) error {
	h.mu.Lock()
	defer h.mu.Unlock()

	idx, ok := h.nodeMap[node]
	if !ok {
		return fmt.Errorf("node not found")
	}

	h.nodeStatus[idx] = false
	h.inactive = append(h.inactive, idx)
	delete(h.nodeMap, node)

	return nil
}

// Get finds the node for a given key.
func (h *DxHash) Get(key string) (string, error) {
	h.mu.RLock()
	defer h.mu.RUnlock()

	if len(h.nodeMap) == 0 {
		return "", fmt.Errorf("no nodes in hash")
	}

	hasher := fnv.New64a()
	hasher.Write([]byte(key))
	r := hasher.Sum64()

	for {
		nodeID := r % uint64(len(h.nodes))
		if h.nodeStatus[nodeID] {
			return h.nodes[nodeID], nil
		}
		r = h.nextRandom(r)
	}
}

func (h *DxHash) nextRandom(r uint64) uint64 {
	// Simple LCG for pseudo-random number generation
	return (r*1664525 + 1013904223)
}
