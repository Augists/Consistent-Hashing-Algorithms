package anchorhash

import (
	"fmt"
	"sort"
	"sync"
)

// AnchorHash is a consistent hashing implementation.
type AnchorHash struct {
	mu      sync.RWMutex
	nodes   map[string]int
	nodeKeys []string
}

// New creates a new AnchorHash instance.
func New() *AnchorHash {
	return &AnchorHash{
		nodes: make(map[string]int),
	}
}

// Add adds nodes to the hash.
func (h *AnchorHash) Add(nodes ...string) {
	h.mu.Lock()
	defer h.mu.Unlock()

	for _, node := range nodes {
		if _, ok := h.nodes[node]; !ok {
			h.nodes[node] = len(h.nodes)
			h.nodeKeys = append(h.nodeKeys, node)
		}
	}
	sort.Strings(h.nodeKeys)
}

// Get returns the node for the given key.
func (h *AnchorHash) Get(key string) (string, error) {
	h.mu.RLock()
	defer h.mu.RUnlock()

	if len(h.nodes) == 0 {
		return "", fmt.Errorf("no nodes in hash")
	}

	hash := h.hash(key)
	idx := h.binarySearch(hash)
	return h.nodeKeys[idx], nil
}

// Remove removes nodes from the hash.
func (h *AnchorHash) Remove(nodes ...string) {
	h.mu.Lock()
	defer h.mu.Unlock()

	for _, node := range nodes {
		delete(h.nodes, node)
	}

	h.nodeKeys = make([]string, 0, len(h.nodes))
	for node := range h.nodes {
		h.nodeKeys = append(h.nodeKeys, node)
	}
	sort.Strings(h.nodeKeys)
}

func (h *AnchorHash) hash(key string) uint64 {
	// Simple FNV-1a hash
	hash := uint64(14695981039346656037)
	for i := 0; i < len(key); i++ {
		hash ^= uint64(key[i])
		hash *= uint64(1099511628211)
	}
	return hash
}

func (h *AnchorHash) binarySearch(hash uint64) int {
	// This is a simplified version of anchor hash, for demonstration.
	// A full implementation would involve a more complex mapping.
	return int(hash % uint64(len(h.nodeKeys)))
}
