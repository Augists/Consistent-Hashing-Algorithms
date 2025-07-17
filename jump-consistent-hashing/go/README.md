# Jump Consistent Hash in Go

This directory contains a Go implementation of Google's Jump Consistent Hash algorithm.
Jump Consistent Hash is a fast, minimal memory, consistent hash algorithm that maps 64-bit keys to a set of `num_buckets` buckets.

## Files

- `jumphash/jump_hash.go`: The jump consistent hash implementation.
- `jumphash/jump_hash_test.go`: Tests for the jump consistent hash implementation.
- `main.go`: A demo program that uses the jump consistent hash function.

## Installation

This project uses Go modules.

## Usage

To run the demo program:

```sh
go run main.go <key> <num_buckets>
```

Example:

```sh
go run main.go 123 10
Key 123 maps to bucket 4
```

To run the tests:

```sh
cd jumphash
go test
```
