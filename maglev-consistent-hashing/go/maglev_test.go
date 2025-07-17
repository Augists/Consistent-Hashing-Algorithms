package maglev

import (
	"fmt"
	"math"
	"strconv"
	"testing"
)

func TestNew(t *testing.T) {
	nodes := []string{"backend-0", "backend-1", "backend-2"}
	M := 65537 // A prime number

	m, err := New(nodes, M)
	if err != nil {
		t.Fatalf("New() failed with error: %v", err)
	}
	if m == nil {
		t.Fatal("New() returned a nil Maglev instance")
	}
	if m.N != len(nodes) {
		t.Errorf("Expected N to be %d, got %d", len(nodes), m.N)
	}
	if m.M != M {
		t.Errorf("Expected M to be %d, got %d", M, m.M)
	}
	if len(m.Lookup) != M {
		t.Errorf("Expected lookup table size to be %d, got %d", M, len(m.Lookup))
	}

	// Test for empty nodes
	_, err = New([]string{}, M)
	if err == nil {
		t.Error("New() should return an error for empty nodes list")
	}

	// Test for invalid M
	_, err = New(nodes, 0)
	if err == nil {
		t.Error("New() should return an error for M <= 0")
	}
}

func TestGet(t *testing.T) {
	nodes := []string{"backend-0", "backend-1"}
	M := 101
	m, _ := New(nodes, M)

	key := "my-test-key"
	node := m.Get(key)

	found := false
	for _, n := range nodes {
		if n == node {
			found = true
			break
		}
	}
	if !found {
		t.Errorf("Get() returned a node '%s' which is not in the initial list", node)
	}
}

func TestConsistency(t *testing.T) {
	nodes := []string{"backend-0", "backend-1", "backend-2"}
	M := 101
	m, _ := New(nodes, M)

	key := "my-consistent-key"
	node1 := m.Get(key)
	node2 := m.Get(key)

	if node1 != node2 {
		t.Errorf("Get() is not consistent. Got '%s' then '%s' for the same key.", node1, node2)
	}
}

func TestDistribution(t *testing.T) {
	nodes := make([]string, 10)
	for i := range nodes {
		nodes[i] = fmt.Sprintf("backend-%02d", i)
	}
	M := 65537
	m, _ := New(nodes, M)

	counts := make(map[string]int)
	for _, nodeIndex := range m.Lookup {
		counts[m.nodes[nodeIndex]]++
	}

	expected := float64(M) / float64(len(nodes))
	for node, count := range counts {
		delta := math.Abs(float64(count) - expected)
		// Allow for a small deviation, e.g., 1% of total size M
		if delta > float64(M)*0.01 {
			t.Errorf("Distribution for node %s is %d, expected around %.2f. Deviation is too high.", node, count, expected)
		}
	}
	if len(counts) != len(nodes) {
		t.Errorf("Not all nodes were present in the lookup table. Expected %d, got %d", len(nodes), len(counts))
	}
}


func TestRebalancing(t *testing.T) {
	nodesN := make([]string, 10)
	for i := range nodesN {
		nodesN[i] = fmt.Sprintf("backend-%02d", i)
	}
	M := 65537

	// Maglev with N nodes
	maglevN, _ := New(nodesN, M)

	// Maglev with N-1 nodes
	nodesN1 := nodesN[:len(nodesN)-1]
	maglevN1, _ := New(nodesN1, M)

	numKeys := 100000
	changes := 0
	for i := 0; i < numKeys; i++ {
		key := "key-" + strconv.Itoa(i)
		nodeN := maglevN.Get(key)
		nodeN1 := maglevN1.Get(key)

		// If the key mapped to the removed node, it's expected to change.
		// We are interested in changes for keys that did NOT map to the removed node.
		if nodeN != nodesN[len(nodesN)-1] && nodeN != nodeN1 {
			changes++
		}
	}

	// The number of "unexpected" changes should be very small.
	// The simple greedy algorithm doesn't guarantee zero changes, but it should be minimal.
	changeRate := float64(changes) / float64(numKeys)
	if changeRate > 0.002 { // Allow for a <0.2% unexpected change rate
		t.Errorf("Found %d unexpected key mapping changes (%.4f%%) after removing one node, which is above the threshold.", changes, changeRate*100)
	}
}

func BenchmarkGet(b *testing.B) {
	nodes := make([]string, 100)
	for i := range nodes {
		nodes[i] = fmt.Sprintf("backend-%02d", i)
	}
	M := 65537
	m, _ := New(nodes, M)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		m.Get("key-" + strconv.Itoa(i))
	}
}
