package multiprobe

import (
	"hash/crc32"
	"sort"
	"strconv"
	"sync"
)

// MultiProbeNode represents a virtual node on the hash ring.
type MultiProbeNode struct {
	hash uint32
	node string
}

// MultiProbeHashRing represents the Multi-Probe Consistent Hash ring.
type MultiProbeHashRing struct {
	nodes    []MultiProbeNode // Sorted list of virtual node hashes
	mu       sync.RWMutex
	replicas int // Number of virtual nodes per real node
	probes   int // Number of probes to make on the ring
}

// NewMultiProbeHashRing creates a new MultiProbeHashRing.
func NewMultiProbeHashRing(replicas, probes int) *MultiProbeHashRing {
	return &MultiProbeHashRing{
		replicas: replicas,
		probes:   probes,
	}
}

// AddNode adds a node to the hash ring.
func (hr *MultiProbeHashRing) AddNode(node string) {
	hr.mu.Lock()
	defer hr.mu.Unlock()

	for i := 0; i < hr.replicas; i++ {
		hash := hr.hashKey(node + strconv.Itoa(i))
		hr.nodes = append(hr.nodes, MultiProbeNode{hash: hash, node: node})
	}

	sort.Slice(hr.nodes, func(i, j int) bool {
		return hr.nodes[i].hash < hr.nodes[j].hash
	})
}

// RemoveNode removes a node from the hash ring.
func (hr *MultiProbeHashRing) RemoveNode(node string) {
	hr.mu.Lock()
	defer hr.mu.Unlock()

	var newNodes []MultiProbeNode
	for _, n := range hr.nodes {
		if n.node != node {
			newNodes = append(newNodes, n)
		}
	}
	hr.nodes = newNodes

	sort.Slice(hr.nodes, func(i, j int) bool {
		return hr.nodes[i].hash < hr.nodes[j].hash
	})
}

// GetNode gets the node responsible for the given key using multiple probes.
func (hr *MultiProbeHashRing) GetNode(key string) string {
	hr.mu.RLock()
	defer hr.mu.RUnlock()

	if len(hr.nodes) == 0 {
		return ""
	}

	// Generate multiple probe hashes for the key
	probeHashes := make([]uint32, hr.probes)
	for i := 0; i < hr.probes; i++ {
		probeHashes[i] = hr.hashKey(key + "_probe_" + strconv.Itoa(i))
	}

	// Find the best node among the probes
	bestNode := ""
	minDistance := uint33(0xFFFFFFFF) // Max uint32 value + 1

	for _, ph := range probeHashes {
		// Find the first node with a hash greater than or equal to the probe hash
		idx := sort.Search(len(hr.nodes), func(i int) bool {
			return hr.nodes[i].hash >= ph
		})

		// If no such node is found, wrap around to the first node
		if idx == len(hr.nodes) {
			idx = 0
		}
		
		currentNode := hr.nodes[idx].node
		currentHash := hr.nodes[idx].hash

		// Calculate distance (clockwise distance on the ring)
		distance := uint33(0)
		if currentHash >= ph {
			distance = uint33(currentHash - ph)
		} else {
			distance = uint33(0xFFFFFFFF) - uint33(ph) + uint33(currentHash) + 1
		}

		if distance < minDistance {
			minDistance = distance
			bestNode = currentNode
		}
	}
	return bestNode
}

// hashKey generates a CRC32 hash for a given string.
func (hr *MultiProbeHashRing) hashKey(key string) uint32 {
	return crc32.ChecksumIEEE([]byte(key))
}

// uint33 is a helper type to handle potential overflow when calculating distance
// for uint32 values that wrap around the ring.
type uint33 uint64
