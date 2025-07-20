#!/bin/bash

# Clean previous results
rm -f ./c/results/results_c.csv
rm -f ./go/results/results_go.csv
rm -f ./results/*.csv
rm -f ./results/*.png

# Run C experiment
echo "Running C experiment..."
cd c
make > /dev/null 2>&1
./experiment
cd ..
mv ./c/results.csv ./c/results/results_c.csv # Move C results to its specific folder

# Run Go experiment
echo "Running Go experiment..."
cd go
go run ./cmd/experiment/
cd ..
mv ./go/results.csv ./go/results/results_go.csv # Move Go results to its specific folder

echo "Experiments finished. Plotting results..."

python3 scripts/plot.py

echo "Done. Plots saved to results/ directory."
