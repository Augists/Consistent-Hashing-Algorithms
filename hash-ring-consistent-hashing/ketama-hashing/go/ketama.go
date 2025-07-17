package ketama

import (
	"crypto/md5"
	"encoding/binary"
	"sort"
	"strconv"
	"sync"
)

// KetamaNode represents a virtual node on the hash ring.
type KetamaNode struct {
	hash uint32
	node string
}

// KetamaRing represents the Ketama consistent hash ring.
type KetamaRing struct {
	nodes    []KetamaNode
	mu       sync.RWMutex
	// Number of points on the ring per server. A common value is 160 (40 * 4, as MD5 produces 4 32-bit integers).
	// This can be adjusted for finer granularity.
	numVirtualNodes int
}

// NewKetamaRing creates a new KetamaRing.
func NewKetamaRing(numVirtualNodes int) *KetamaRing {
	return &KetamaRing{
		numVirtualNodes: numVirtualNodes,
	}
}

// AddNode adds a node to the hash ring.
func (kr *KetamaRing) AddNode(node string) {
	kr.mu.Lock()
	defer kr.mu.Unlock()

	for i := 0; i < kr.numVirtualNodes/4; i++ { // MD5 produces 16 bytes (4 * 4 bytes)
		data := md5.Sum([]byte(node + strconv.Itoa(i)))
		for j := 0; j < 4; j++ {
			hash := binary.LittleEndian.Uint32(data[j*4 : (j+1)*4])
			kr.nodes = append(kr.nodes, KetamaNode{hash: hash, node: node})
		}
	}

	sort.Slice(kr.nodes, func(i, j int) bool {
		return kr.nodes[i].hash < kr.nodes[j].hash
	})
}

// RemoveNode removes a node from the hash ring.
func (kr *KetamaRing) RemoveNode(node string) {
	kr.mu.Lock()
	defer kr.mu.Unlock()

	var newNodes []KetamaNode
	for _, n := range kr.nodes {
		if n.node != node {
			newNodes = append(newNodes, n)
		}
	}
	kr.nodes = newNodes

	sort.Slice(kr.nodes, func(i, j int) bool {
		return kr.nodes[i].hash < kr.nodes[j].hash
	})
}

// GetNode gets the node responsible for the given key.
func (kr *KetamaRing) GetNode(key string) string {
	kr.mu.RLock()
	defer kr.mu.RUnlock()

	if len(kr.nodes) == 0 {
		return ""
	}

	data := md5.Sum([]byte(key))
	keyHash := binary.LittleEndian.Uint32(data[0:4]) // Use first 4 bytes of MD5 hash

	// Find the first node with a hash greater than or equal to the key's hash
	idx := sort.Search(len(kr.nodes), func(i int) bool {
		return kr.nodes[i].hash >= keyHash
	})

	// If no such node is found, wrap around to the first node
	if idx == len(kr.nodes) {
		idx = 0
	}
	return kr.nodes[idx].node
}
