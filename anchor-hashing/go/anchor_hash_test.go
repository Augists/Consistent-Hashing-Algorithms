package anchorhash

import (
	"fmt"
	"testing"
)

func TestAdd(t *testing.T) {
	h := New()
	h.Add("node1", "node2")
	if len(h.nodes) != 2 {
		t.Errorf("expected 2 nodes, got %d", len(h.nodes))
	}
}

func TestRemove(t *testing.T) {
	h := New()
	h.Add("node1", "node2")
	h.Remove("node1")
	if len(h.nodes) != 1 {
		t.Errorf("expected 1 node, got %d", len(h.nodes))
	}
	if _, ok := h.nodes["node1"]; ok {
		t.Errorf("node1 should have been removed")
	}
}

func TestGet(t *testing.T) {
	h := New()
	h.Add("node1", "node2", "node3")
	key := "my-key"
	node, err := h.Get(key)
	if err != nil {
		t.Errorf("unexpected error: %v", err)
	}
	if node == "" {
		t.Errorf("expected a node, got empty string")
	}
}

func TestGetNoNodes(t *testing.T) {
	h := New()
	_, err := h.Get("my-key")
	if err == nil {
		t.Errorf("expected an error, got nil")
	}
}

func BenchmarkGet(b *testing.B) {
	h := New()
	for i := 0; i < 100; i++ {
		h.Add(fmt.Sprintf("node%d", i))
	}
	for i := 0; i < b.N; i++ {
		h.Get(fmt.Sprintf("key%d", i))
	}
}
