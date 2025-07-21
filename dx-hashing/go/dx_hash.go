package dxhash

import (
	"hash/fnv"
)

// Active returns the active bucket slice (for experiment use)
func (h *DxHash) Active() []bool {
	return h.active
}

type DxHash struct {
	numBuckets int
	active     []bool
	L          int // number of hash functions
}

func NewDxHash(numBuckets, L int) *DxHash {
	h := &DxHash{
		numBuckets: numBuckets,
		active:     make([]bool, numBuckets),
		L:          L,
	}
	for i := 0; i < numBuckets; i++ {
		h.active[i] = true
	}
	return h
}

func (h *DxHash) Map(key uint64) int {
	for i := 0; i < h.L; i++ {
		b := int(hash64WithSalt(key, uint64(i)) % uint64(h.numBuckets))
		if h.active[b] {
			return b
		}
	}
	// fallback: return first active bucket
	for b := 0; b < h.numBuckets; b++ {
		if h.active[b] {
			return b
		}
	}
	return -1 // no active bucket
}

func (h *DxHash) Remove(bucket int) {
	if bucket >= 0 && bucket < h.numBuckets {
		h.active[bucket] = false
	}
}

func (h *DxHash) Add(bucket int) {
	if bucket >= 0 && bucket < h.numBuckets {
		h.active[bucket] = true
	}
}

func hash64WithSalt(key uint64, salt uint64) uint64 {
	hash := fnv.New64a()
	var b [8]byte
	for i := 0; i < 8; i++ {
		b[i] = byte(key >> (8 * i))
	}
	hash.Write(b[:])
	for i := 0; i < 8; i++ {
		b[i] = byte(salt >> (8 * i))
	}
	hash.Write(b[:])
	return hash.Sum64()
}
