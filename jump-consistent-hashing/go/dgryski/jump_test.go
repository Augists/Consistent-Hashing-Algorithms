package dgryski

import (
	"fmt"
	"testing"
)

func Hash(key uint64, numBuckets int) int32 {

	var b int64 = -1
	var j int64

	for j < int64(numBuckets) {
		b = j
		key = key*2862933555777941757 + 1
		j = int64(float64(b+1) * (float64(1<<31) / float64((key>>33)+1)))
	}

	return int32(b)
}

func TestHash(t *testing.T) {
	t.Logf("1<<31 = %d", 1<<31)
	testCases := []struct {
		key        uint64
		numBuckets int
		expected   int32
	}{
		{1, 1, 0},
		{1, 10, 6},
		{1, 100, 75},
		{2, 100, 13},
		{0xdeadbeef, 1000, 834},
	}

	for _, tc := range testCases {
		t.Run(fmt.Sprintf("key=%d,buckets=%d", tc.key, tc.numBuckets), func(t *testing.T) {
			actual := Hash(tc.key, tc.numBuckets)
			if actual != tc.expected {
				t.Errorf("Hash(%d, %d): expected %d, got %d", tc.key, tc.numBuckets, tc.expected, actual)
			}
		})
	}
}