package maglev

import (
	"errors"
	"hash/fnv"
	"sort"
)

// Maglev is the main struct for the Maglev hashing algorithm.
type Maglev struct {
	nodes   []string // Names of the backend nodes.
	Lookup  []int    // The lookup table.
	M       int      // Size of the lookup table.
	N       int      // Number of backend nodes.
}

// New creates a new Maglev instance.
// nodes is the list of backend node names.
// M is the size of the lookup table. It should be a prime number.
func New(nodes []string, M int) (*Maglev, error) {
	if len(nodes) == 0 {
		return nil, errors.New("nodes list cannot be empty")
	}
	if M <= 0 {
		return nil, errors.New("lookup table size M must be positive")
	}

	m := &Maglev{
		nodes:   make([]string, len(nodes)),
		M:       M,
		N:       len(nodes),
		Lookup:  make([]int, M),
	}
	copy(m.nodes, nodes)
	sort.Strings(m.nodes) // Sort nodes for consistent permutation generation.

	m.populate()
	return m, nil
}

// Get returns the backend node name for a given key.
func (m *Maglev) Get(key string) string {
	hash := m.hashKey(key)
	index := hash % uint64(m.M)
	return m.nodes[m.Lookup[index]]
}

// populate fills the lookup table. This is the core of the Maglev algorithm.
func (m *Maglev) populate() {
	if m.N == 0 {
		return
	}

	permutations := m.generatePermutations()
	next := make([]int, m.N)
	
	for i := range m.Lookup {
		m.Lookup[i] = -1
	}

	filledCount := 0
	for filledCount < m.M {
		for i := 0; i < m.N; i++ {
			c := permutations[i][next[i]]
			for m.Lookup[c] != -1 {
				next[i]++
				c = permutations[i][next[i]]
			}
			m.Lookup[c] = i
			next[i]++
			filledCount++
			if filledCount == m.M {
				return
			}
		}
	}
}

// generatePermutations creates the permutation table for all nodes.
func (m *Maglev) generatePermutations() [][]int {
	permutations := make([][]int, m.N)
	for i := 0; i < m.N; i++ {
		permutations[i] = m.generatePermutation(m.nodes[i])
	}
	return permutations
}

// generatePermutation creates a single permutation list for a given node name.
func (m *Maglev) generatePermutation(node string) []int {
	offset, skip := m.hashNode(node)
	permutation := make([]int, m.M)
	for j := 0; j < m.M; j++ {
		permutation[j] = int((offset + uint64(j)*skip) % uint64(m.M))
	}
	return permutation
}

// hashNode generates two separate hashes for a node name, used for offset and skip.
func (m *Maglev) hashNode(node string) (uint64, uint64) {
	// Use FNV-1a for hashing
	h1 := fnv.New64a()
	h1.Write([]byte(node))
	hash1 := h1.Sum64()

	h2 := fnv.New32a()
	h2.Write([]byte(node))
	hash2 := h2.Sum32()

	offset := hash1 % uint64(m.M)
	skip := uint64(hash2)%(uint64(m.M)-1) + 1
	return offset, skip
}

// hashKey generates a single hash for a given key.
func (m *Maglev) hashKey(key string) uint64 {
	h := fnv.New64a()
	h.Write([]byte(key))
	return h.Sum64()
}
