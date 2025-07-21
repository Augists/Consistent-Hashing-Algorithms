# DxHash C Implementation

This directory contains a clean C implementation of the DxHash algorithm, including:
- `dx_hash.c` / `dx_hash.h`: Core algorithm
- `test_dx_hash.c`: Unit tests
- `experiment.c`: Experiment code (peak/average ratio, remapping curve, etc.)
- `Makefile`: Build/test/experiment automation
- `scripts/plot.py`, `scripts/plot_remap_curve.py`: Plotting scripts for experiment results
- `results/`: Output data and figures

## Quick Start

1. `make test`  # Run unit tests
2. `make experiment`  # Run experiments
3. `python3 scripts/plot.py`  # Generate peak/average plot
4. `python3 scripts/plot_remap_curve.py`  # Generate remapping curve plot

See the main README for algorithm details, analysis, and results.
