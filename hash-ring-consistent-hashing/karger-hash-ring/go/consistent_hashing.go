package consistenthashing

import (
	"hash/crc32"
	"sort"
	"strconv"
	"sync"
)

// HashRing represents the consistent hash ring.
type HashRing struct {
	nodes    []string // Sorted list of node hashes
	nodeMap  map[uint32]string // Map of hash to node name
	replicas int // Number of virtual nodes per real node
	mu       sync.RWMutex
}

// NewHashRing creates a new HashRing.
func NewHashRing(replicas int) *HashRing {
	return &HashRing{
		nodes:    []string{},
		nodeMap:  make(map[uint32]string),
		replicas: replicas,
	}
}

// AddNode adds a node to the hash ring.
func (hr *HashRing) AddNode(node string) {
	hr.mu.Lock()
	defer hr.mu.Unlock()

	for i := 0; i < hr.replicas; i++ {
		hash := hr.hashKey(node + strconv.Itoa(i))
		hr.nodeMap[hash] = node
		hr.nodes = append(hr.nodes, strconv.FormatUint(uint64(hash), 10))
	}
	sort.Slice(hr.nodes, func(i, j int) bool {
		return hr.parseHash(hr.nodes[i]) < hr.parseHash(hr.nodes[j])
	})
}

// RemoveNode removes a node from the hash ring.
func (hr *HashRing) RemoveNode(node string) {
	hr.mu.Lock()
	defer hr.mu.Unlock()

	for i := 0; i < hr.replicas; i++ {
		hash := hr.hashKey(node + strconv.Itoa(i))
		delete(hr.nodeMap, hash)
		hr.removeNodeFromSlice(strconv.FormatUint(uint64(hash), 10))
	}
}

// GetNode gets the node responsible for the given key.
func (hr *HashRing) GetNode(key string) string {
	hr.mu.RLock()
	defer hr.mu.RUnlock()

	if len(hr.nodes) == 0 {
		return ""
	}

	hash := hr.hashKey(key)

	// Find the first node with a hash greater than or equal to the key's hash
	idx := sort.Search(len(hr.nodes), func(i int) bool {
		return hr.parseHash(hr.nodes[i]) >= hash
	})

	// If no such node is found, wrap around to the first node
	if idx == len(hr.nodes) {
		idx = 0
	}
	return hr.nodeMap[hr.parseHash(hr.nodes[idx])]
}

// hashKey generates a CRC32 hash for a given string.
func (hr *HashRing) hashKey(key string) uint32 {
	return crc32.ChecksumIEEE([]byte(key))
}

// removeNodeFromSlice removes a specific hash string from the sorted nodes slice.
func (hr *HashRing) removeNodeFromSlice(hashStr string) {
	for i, n := range hr.nodes {
		if n == hashStr {
			hr.nodes = append(hr.nodes[:i], hr.nodes[i+1:]...)
			return
		}
	}
}

// parseHash converts a string hash back to uint32.
func (hr *HashRing) parseHash(hashStr string) uint32 {
	h, _ := strconv.ParseUint(hashStr, 10, 32)
	return uint32(h)
}
