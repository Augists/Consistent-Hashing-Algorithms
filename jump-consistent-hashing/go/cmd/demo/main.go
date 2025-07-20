package main

import (
	"fmt"
	"os"
	"strconv"

	"github.com/augists/Jump-CHash/go/jumphash"
)

func main() {
	if len(os.Args) < 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <key> <num_buckets>\n", os.Args[0])
		os.Exit(1)
	}
	key, err := strconv.ParseUint(os.Args[1], 10, 64)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Invalid key: %v\n", err)
		os.Exit(1)
	}
	numBuckets, err := strconv.ParseInt(os.Args[2], 10, 32)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Invalid number of buckets: %v\n", err)
		os.Exit(1)
	}
	if numBuckets <= 0 {
		fmt.Fprintln(os.Stderr, "Number of buckets must be positive.")
		os.Exit(1)
	}

	bucket := jumphash.Hash(key, int32(numBuckets))
	fmt.Printf("Key %d maps to bucket %d\n", key, bucket)
}