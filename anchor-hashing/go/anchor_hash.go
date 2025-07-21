package anchorhash

import (
	"hash/fnv"
)

type AnchorHash struct {
	A      []bool // Active buckets
	W      []int  // Working set
	L      []int  // Location of each bucket in W
	R      []int  // Restore chain (removal parent)
	N      int    // Total buckets
	Acount int    // Number of active buckets
}

func NewAnchorHash(N, a int) *AnchorHash {
	ah := &AnchorHash{
		A:      make([]bool, N),
		W:      make([]int, N),
		L:      make([]int, N),
		R:      make([]int, N),
		N:      N,
		Acount: a,
	}
	for i := 0; i < N; i++ {
		ah.A[i] = i < a
		ah.W[i] = i
		ah.L[i] = i
		ah.R[i] = -1
	}
	return ah
}

func (ah *AnchorHash) hash(key uint64, salt int) int {
	h := fnv.New64a()
	var b [16]byte
	for i := 0; i < 8; i++ {
		b[i] = byte(key >> (8 * i))
	}
	for i := 0; i < 8; i++ {
		b[8+i] = byte(salt >> (8 * i))
	}
	h.Write(b[:])
	return int(h.Sum64() % uint64(ah.N))
}

func (ah *AnchorHash) Map(key uint64) int {
	b := ah.hash(key, 0)
	for !ah.A[b] {
		b = ah.R[b]
		if b < 0 {
			return -1
		}
	}
	return b
}

func (ah *AnchorHash) Remove(b int) bool {
	if !ah.A[b] || ah.Acount <= 1 {
		return false
	}
	pos := ah.L[b]
	last := ah.Acount - 1
	lastB := ah.W[last]
	ah.W[pos] = ah.W[last]
	ah.L[lastB] = pos
	ah.Acount--
	ah.A[b] = false
	ah.R[b] = lastB
	return true
}

func (ah *AnchorHash) Add(b int) bool {
	if ah.A[b] || ah.Acount >= ah.N {
		return false
	}
	ah.A[b] = true
	ah.W[ah.Acount] = b
	ah.L[b] = ah.Acount
	ah.Acount++
	ah.R[b] = -1
	return true
}
