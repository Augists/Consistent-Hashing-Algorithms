package anchorhash

import (
	"testing"
)

func TestBasicMapping(t *testing.T) {
	h := NewAnchorHash(10, 10)
	for k := uint64(0); k < 1000; k++ {
		b := h.Map(k)
		if b < 0 || b >= 10 {
			t.Fatalf("key %d mapped to invalid bucket %d", k, b)
		}
	}
}

func TestRemoveAdd(t *testing.T) {
	h := NewAnchorHash(8, 8)
	before := h.Map(12345)
	h.Remove(before)
	after := h.Map(12345)
	if after == before {
		t.Fatalf("remove did not change mapping")
	}
	h.Add(before)
	again := h.Map(12345)
	if again != before {
		t.Fatalf("add did not restore mapping")
	}
}
