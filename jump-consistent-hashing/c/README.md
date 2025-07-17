# Jump Consistent Hash in C

This directory contains a C implementation of Google's Jump Consistent Hash algorithm.
Jump Consistent Hash is a fast, minimal memory, consistent hash algorithm that maps 64-bit keys to a set of `num_buckets` buckets.

## Files

- `jump_hash.h`: Header file for the jump consistent hash function.
- `jump_hash.c`: Implementation of the jump consistent hash function.
- `main.c`: A demo program that uses the jump consistent hash function.
- `test.c`: A test program for the jump consistent hash function.

## Compilation

To compile the demo program, run:

```sh
gcc -o demo main.c jump_hash.c
```

To compile the test program, run:

```sh
gcc -o test test.c jump_hash.c
```

## Usage

To run the demo program:

```sh
./demo <key> <num_buckets>
```

Example:

```sh
./demo 123 10
Key 123 maps to bucket 4
```

To run the tests:

```sh
./test
```
