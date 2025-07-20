#!/bin/bash

# Clean previous results
rm -f ./c/results.csv results.csv

# Run C experiment
echo "Running C experiment... (This might take a moment)"
cd c
make experiment > /dev/null 2>&1
./experiment
cd ..
mv ./c/results.csv .

# Run Go experiment
echo "Running Go experiment... (This might take a moment)"
cd go
go run ./cmd/experiment/
cd ..

echo "Experiments finished. Combining results..."

# The Go experiment appends, so the file now contains both results.
echo "Plotting results... (This might take a moment)"
python3 plot.py

echo "Done. Plot saved to rendezvous_hashing_distribution.png"
