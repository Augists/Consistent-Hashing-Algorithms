#!/bin/bash

# Clean previous results
rm -f ./c/results/*.csv
rm -f ./go/results/*.csv
rm -f ./results/*.csv
rm -f ./results/*.png

echo "Running C experiment..."
# Run C experiment and capture its stdout for distribution and remapping
./c/experiment | awk '/DX_DIST_START/{flag=1;next}/DX_DIST_END/{flag=0}flag' > ./c/results/results_c.csv
./c/experiment | awk '/DX_REMAP_START/{flag=1;next}/DX_REMAP_END/{flag=0}flag' > ./c/results/remapping_results_c.csv

echo "Running Go experiment..."
# Run Go experiment and capture its stdout for distribution and remapping
(cd go && go run ./cmd/experiment/) | awk '/DX_DIST_START/{flag=1;next}/DX_DIST_END/{flag=0}flag' > ./go/results/results_go.csv
(cd go && go run ./cmd/experiment/) | awk '/DX_REMAP_START/{flag=1;next}/DX_REMAP_END/{flag=0}flag' > ./go/results/remapping_results_go.csv

echo "Experiments finished. Plotting results..."

# Activate virtual environment and run plot.py
source venv/bin/activate
python3 scripts/plot.py

echo "Done. Plots saved to results/ directory."
