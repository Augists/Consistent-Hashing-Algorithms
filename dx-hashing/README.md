# DxHash

DxHash is an efficient consistent hashing algorithm that does not require virtual nodes, offering fast node additions/removals and minimal key remapping. It is well-suited for applications like distributed caches and load balancers.

## Algorithm Principle

DxHash generates multiple candidate slots for each key using multiple hash functions. It prioritizes hitting the first active slot, achieving an approximately uniform distribution and minimal remapping.

### Pseudocode

```c
for i in 0..K-1: // Iterate through K hash functions
    h = H_i(key) % N // Hash key with i-th hash function to get a candidate slot
    if bucket[h] is active: // If the candidate slot is active
        return h // Map key to this slot
// fallback: return the first active bucket (should ideally not be reached in a well-configured system)
```

## Directory Structure

-   `c/`: C language implementation, tests, and experiments.
-   `go/`: Go language implementation, tests, and experiments.
-   `results/`: Stores experiment results (CSV files, plots).
-   `scripts/`: Contains unified Python scripts for running experiments and plotting results.

## How to Run Experiments

To run the experiments and generate plots, follow these steps for both C and Go implementations:

### C Implementation

1.  Navigate to the C directory:
    ```sh
    cd dx-hashing/c
    ```
2.  Build the experiment executable:
    ```sh
    make experiment
    ```
3.  Run the experiment:
    ```sh
    ./experiment
    ```
    This will generate `dx_experiment.csv`, `dx_remap_removal_curve.csv`, and `dx_remap_addition_curve.csv` in the `dx-hashing/results/` directory.

### Go Implementation

1.  Navigate to the Go directory:
    ```sh
    cd dx-hashing/go
    ```
2.  Run the experiment:
    ```sh
    go run ./cmd/experiment/main.go
    ```
    This will generate `dx_experiment.csv`, `dx_remap_removal_curve.csv`, and `dx_remap_addition_curve.csv` in the `dx-hashing/results/` directory.

### Plotting Results

1.  Navigate to the scripts directory (from the project root):
    ```sh
    cd dx-hashing/scripts
    ```
2.  Run the plotting script:
    ```sh
    python3 plot.py
    ```
    This will generate `key_distribution.png`, `remapping_removal_impact.png`, and `remapping_addition_impact.png` in the `dx-hashing/results/` directory.

## Experiment Details

The experiments measure the following aspects of DxHash:

### Key Distribution (Peak-to-Average Ratio)

This metric assesses how evenly keys are distributed across the active nodes. A ratio closer to 1.00 indicates a more uniform distribution.

### Remapping Ratio (Node Removal)

This experiment simulates node failures by progressively removing a percentage of nodes from an initial set (e.g., 1000 nodes). It measures the percentage of keys that change their assigned node after each removal step. A lower percentage indicates better consistency.

### Remapping Ratio (Node Addition)

This experiment simulates node recovery or cluster expansion. It starts with an initial set of nodes, removes a fixed percentage, and then progressively adds nodes back. It measures the percentage of keys that change their assigned node during the addition process, relative to the state after the initial removal.

## Expected Results

DxHash is expected to show good key distribution (peak-to-average ratio close to 1.00) and low remapping ratios during both node removal and addition operations, demonstrating its efficiency and consistency.

## References

-   DxHash Paper and Official Implementation:
    [https://arxiv.org/abs/2102.10260](https://arxiv.org/abs/2102.10260)