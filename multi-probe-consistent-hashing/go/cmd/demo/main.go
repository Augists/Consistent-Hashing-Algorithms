package main

import (
	"fmt"
	"github.com/augists/consistent-hashing-algorithms/multi-probe-consistent-hashing/go"
)

const (
	replicas = 10
	probes   = 5
)

func main() {
	hr := multiprobe.NewMultiProbeHashRing(replicas, probes)

	// Add nodes
	hr.AddNode("node1")
	hr.AddNode("node2")
	hr.AddNode("node3")

	// Get a key
	key := "my-special-key"
	node := hr.GetNode(key)

	fmt.Printf("Key '%s' is mapped to node '%s'\n", key, node)
}
