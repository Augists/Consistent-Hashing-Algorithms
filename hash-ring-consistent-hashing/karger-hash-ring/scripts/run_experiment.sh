#!/bin/bash

# Clean previous results
rm -f ./go/results/*.csv
rm -f ./results/*.csv
rm -f ./results/*.png

echo "Running Go experiment..."
# Run Go experiment and capture its stdout for distribution and remapping
(cd go && go run ./cmd/experiment/) | awk '/KARGER_DIST_START/{flag=1;next}/KARGER_DIST_END/{flag=0}flag' | tail -n +2 > ./go/results/results_go.csv
(cd go && go run ./cmd/experiment/) | awk '/KARGER_REMAP_START/{flag=1;next}/KARGER_REMAP_END/{flag=0}flag' | tail -n +2 > ./go/results/remapping_results_go.csv

echo "Experiments finished. Plotting results..."

# Activate virtual environment and run plot.py
source venv/bin/activate
python3 scripts/plot.py

echo "Done. Plots saved to results/ directory."
