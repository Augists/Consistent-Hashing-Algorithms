#!/bin/bash

# Clean previous results
rm -f ./c/results.csv

# Run C experiment
echo "Running C experiment..."
cd c
make > /dev/null 2>&1
./experiment
cd ..
mv ./c/results.csv .

# Run Go experiment
echo "Running Go experiment..."
cd go
go run ./cmd/experiment/
cd ..

echo "Experiments finished. Combining results..."

# The Go experiment appends, so the file now contains both results.
echo "Plotting results..."
python3 plot.py

echo "Done. Plot saved to k_failure_disruption.png"
