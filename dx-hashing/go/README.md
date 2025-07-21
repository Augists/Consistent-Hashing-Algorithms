
# DxHash Go Implementation

This directory contains a clean Go implementation of the DxHash algorithm, including:
- `dx_hash.go`: Core algorithm
- `dx_hash_test.go`: Unit tests
- `cmd/experiment/`: Experiment for peak/average ratio and remapping ratio
- `cmd/experiment_remap_curve/`: Experiment for remapping curve (multi-step)
- `results/`: Output data and figures
- `scripts/`: Python scripts for plotting

## Quick Start

### 1. Run Unit Tests
```sh
cd dx-hashing/go
go test
```

### 2. Run Experiments
```sh
# Peak/Average & single remap ratio
cd dx-hashing/go
mkdir -p results
go run ./cmd/experiment/main.go

# Remapping curve (multi-step)
go run ./cmd/experiment_remap_curve/main.go
```

### 3. Plot Results
```sh
cd scripts
python3 plot.py              # Bar chart for peak/average & remap ratio
python3 plot_remap_curve.py  # Remapping curve
```

## Output
- `results/dx_experiment.csv`, `dx_experiment.png`
- `results/dx_remap_curve.csv`, `dx_remap_curve.png`

## Algorithm
DxHash is a consistent hashing algorithm with minimal remapping and good load balance. See the main repo README for details.
