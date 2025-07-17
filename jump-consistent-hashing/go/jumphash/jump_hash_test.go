package jumphash

import (
	"fmt"
	"testing"
)

func TestHash(t *testing.T) {
	testCases := []struct {
		key        uint64
		numBuckets int32
		expected   int32
	}{
		{1, 1, 0},
		{1, 10, 6},
		{1, 100, 55},
		{2, 100, 62},
		{0xdeadbeef, 1000, 285},
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

func TestConsistency(t *testing.T) {
	key := uint64(12345)
	lastBucket := Hash(key, 1)
	if lastBucket != 0 {
		t.Errorf("Expected bucket 0 for numBuckets=1, got %d", lastBucket)
	}
	for i := int32(2); i < 1000; i++ {
		bucket := Hash(key, i)
		if bucket < 0 || bucket >= i {
			t.Errorf("Bucket %d is out of range [0, %d)", bucket, i)
		}
		previousBucket := Hash(key, i-1)
		if bucket != previousBucket && bucket != i-1 {
			t.Errorf("Inconsistent hash for key %d, numBuckets %d. Got %d, previous %d", key, i, bucket, previousBucket)
		}
	}
}