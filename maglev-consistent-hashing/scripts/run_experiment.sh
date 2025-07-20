#!/bin/bash

# Clean previous results and temporary output files
rm -f ./c/results/*.txt
rm -f ./go/results/*.txt
rm -f ./results/*.png

echo "Running C experiment..."
# Run C experiment and redirect its stdout to a temporary file
./c/experiment > ./c/results/c_output.txt 2>&1

echo "Running Go experiment..."
# Run Go experiment and redirect its stdout to a temporary file
(cd go && go run ./cmd/experiment/) > ./go/results/go_output.txt 2>&1

echo "Experiments finished. Plotting results..."

# Activate virtual environment and run plot.py
source venv/bin/activate
python3 scripts/plot.py

echo "Done. Plots saved to results/ directory."
