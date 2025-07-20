package dxhash

import (
	"fmt"
	"testing"
)

func TestAdd(t *testing.T) {
	h := New(10)
	_, err := h.Add("node1")
	if err != nil {
		t.Errorf("unexpected error: %v", err)
	}
	if len(h.nodeMap) != 1 {
		t.Errorf("expected 1 node, got %d", len(h.nodeMap))
	}
}

func TestRemove(t *testing.T) {
	h := New(10)
	h.Add("node1")
	err := h.Remove("node1")
	if err != nil {
		t.Errorf("unexpected error: %v", err)
	}
	if len(h.nodeMap) != 0 {
		t.Errorf("expected 0 nodes, got %d", len(h.nodeMap))
	}
}

func TestGet(t *testing.T) {
	h := New(10)
	h.Add("node1")
	h.Add("node2")
	node, err := h.Get("my-key")
	if err != nil {
		t.Errorf("unexpected error: %v", err)
	}
	if node == "" {
		t.Errorf("expected a node, got empty string")
	}
}

func TestFull(t *testing.T) {
	h := New(2)
	h.Add("node1")
	h.Add("node2")
	_, err := h.Add("node3")
	if err == nil {
		t.Errorf("expected an error, got nil")
	}
}

func BenchmarkGet(b *testing.B) {
	h := New(100)
	for i := 0; i < 100; i++ {
		h.Add(fmt.Sprintf("node%d", i))
	}
	for i := 0; i < b.N; i++ {
		h.Get(fmt.Sprintf("key%d", i))
	}
}
