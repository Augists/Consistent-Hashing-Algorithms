#!/bin/bash

# Clean previous results
rm -f ./c/results/*.csv
rm -f ./go/results/*.csv
rm -f ./results/*.csv
rm -f ./results/*.png

# Run C experiment
echo "Running C experiment... (This might take a moment)"
cd c
make experiment > /dev/null 2>&1
./experiment
cd ..
mv ./c/results.csv ./c/results/results_c.csv # Move C results to its specific folder

# Run Go experiment
echo "Running Go experiment... (This might take a moment)"
cd go
go run ./cmd/experiment/
cd ..
mv ./go/results.csv ./go/results/results_go.csv # Move Go results to its specific folder

echo "Experiments finished. Plotting results... (This might take a moment)"

python3 scripts/plot.py

echo "Done. Plots saved to results/ directory."
