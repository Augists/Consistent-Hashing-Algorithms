#!/bin/bash

# Clean previous results and temporary output files
rm -f results.csv remapping_results.csv c_output.txt go_output.txt

echo "Running C experiment..."
# Run C experiment and redirect its stdout to a temporary file
./c/experiment > c_output.txt 2>&1

echo "Running Go experiment..."
# Run Go experiment and redirect its stdout to a temporary file
(cd go && go run ./cmd/experiment/) > go_output.txt 2>&1

echo "Experiments finished. Plotting results..."

# Activate virtual environment and run plot.py
source venv/bin/activate
python3 plot.py

echo "Done. Plots saved to key_distribution.png and remapping_impact.png"