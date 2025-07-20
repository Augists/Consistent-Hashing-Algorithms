#!/bin/bash

# Clean previous results in the root directory
rm -f results.csv remapping_results.csv results_go.csv remapping_results_go.csv

echo "Running Go experiment..."
# Run Go experiment and capture its stdout for distribution and remapping
(cd go && go run ./cmd/experiment/) | awk '/KARGER_DIST_START/{flag=1;next}/KARGER_DIST_END/{flag=0}flag' | tail -n +2 > results_go.csv
(cd go && go run ./cmd/experiment/) | awk '/KARGER_REMAP_START/{flag=1;next}/KARGER_REMAP_END/{flag=0}flag' | tail -n +2 > remapping_results_go.csv

echo "Experiments finished. Combining results..."

# Combine Go distribution results with header
echo "Node,Keys,Algorithm" > results.csv
cat results_go.csv >> results.csv

# Combine Go remapping results with header
echo "Operation,RemappedKeys,TotalKeys,Algorithm" > remapping_results.csv
cat remapping_results_go.csv >> remapping_results.csv

echo "Plotting results..."
# Activate virtual environment and run plot.py
source venv/bin/activate
python3 plot.py

echo "Done. Plots saved to key_distribution.png and remapping_impact.png"