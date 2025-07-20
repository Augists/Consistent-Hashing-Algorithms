#!/bin/bash

# Clean previous results in the root directory
rm -f results.csv remapping_results.csv results_c.csv results_go.csv remapping_results_c.csv remapping_results_go.csv

echo "Running C experiment..."
# Run C experiment and capture its stdout for distribution and remapping
./c/experiment | awk '/DX_DIST_START/{flag=1;next}/DX_DIST_END/{flag=0}flag' > results_c.csv
./c/experiment | awk '/DX_REMAP_START/{flag=1;next}/DX_REMAP_END/{flag=0}flag' > remapping_results_c.csv

echo "Running Go experiment..."
# Run Go experiment and capture its stdout for distribution and remapping
(cd go && go run ./cmd/experiment/) | awk '/DX_DIST_START/{flag=1;next}/DX_DIST_END/{flag=0}flag' > results_go.csv
go run ./go/cmd/experiment/ | awk '/DX_REMAP_START/{flag=1;next}/DX_REMAP_END/{flag=0}flag' > remapping_results_go.csv

echo "Experiments finished. Combining results..."

# Combine C and Go distribution results
cat results_c.csv > results.csv
tail -n +2 results_go.csv >> results.csv

# Combine C and Go remapping results
cat remapping_results_c.csv > remapping_results.csv
tail -n +2 remapping_results_go.csv >> remapping_results.csv

echo "Plotting results..."
# Activate virtual environment and run plot.py
source venv/bin/activate
python3 plot.py

echo "Done. Plots saved to key_distribution.png and remapping_impact.png"