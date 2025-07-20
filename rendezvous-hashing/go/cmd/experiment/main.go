package main

import (
	"fmt"
	"os"
	"strconv"
	"github.com/augists/consistent-hashing-algorithms/rendezvous-hashing/go"
)

const (
	numNodes = 10
	numKeys  = 1000000
)

func main() {
	nodes := make([]string, numNodes)
	for i := 0; i < numNodes; i++ {
		nodes[i] = fmt.Sprintf("node%d", i)
	}

	counts := make(map[string]int)
	for _, node := range nodes {
		counts[node] = 0
	}

	for i := 0; i < numKeys; i++ {
		key := fmt.Sprintf("key%d", i)
		assignedIdx := rendezvous.RendezvousHash(key, nodes)
		if assignedIdx != -1 {
			counts[nodes[assignedIdx]]++
		}
	}

	// Output results to CSV
	file, err := os.OpenFile("results.csv", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	for _, nodeName := range nodes {
		_, err := file.WriteString(fmt.Sprintf("%s,%d,rendezvous_go\n", nodeName, counts[nodeName]))
		if err != nil {
			panic(err)
		}
	}

	fmt.Println("Go experiment finished. Results appended to results.csv")
}
