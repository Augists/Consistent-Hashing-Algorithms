package rendezvous

import (
	"fmt"
	"testing"
)

func TestRendezvousHashBasic(t *testing.T) {
	nodes := []string{"nodeA", "nodeB", "nodeC"}
	key := "mykey"

	chosenIdx := RendezvousHash(key, nodes)
	if chosenIdx < 0 || chosenIdx >= len(nodes) {
		t.Errorf("RendezvousHash(%s, %v) returned invalid index %d", key, nodes, chosenIdx)
	}
	fmt.Printf("  Key '%s' mapped to '%s'.\n", key, nodes[chosenIdx])

	key2 := "anotherkey"
	chosenIdx2 := RendezvousHash(key2, nodes)
	if chosenIdx2 < 0 || chosenIdx2 >= len(nodes) {
		t.Errorf("RendezvousHash(%s, %v) returned invalid index %d", key2, nodes, chosenIdx2)
	}
	fmt.Printf("  Key '%s' mapped to '%s'.\n", key2, nodes[chosenIdx2])
}

func TestRendezvousHashEmptyNodes(t *testing.T) {
	nodes := []string{}
	key := "mykey"

	chosenIdx := RendezvousHash(key, nodes)
	if chosenIdx != -1 {
		t.Errorf("RendezvousHash(%s, %v) expected -1, got %d", key, nodes, chosenIdx)
	}
	fmt.Println("  Correctly returned -1 for empty nodes.")
}

