#!/bin/bash

# This script runs the k-failure experiment for the Go implementation of Maglev hash.

# Get the directory of the script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

echo "Running k-failure experiment for Go Maglev..."
echo "This may take a minute."

# Run the experiment program and redirect output to results.csv
(cd "${SCRIPT_DIR}" && go run ./cmd/experiment) > "${SCRIPT_DIR}/results.csv"

echo "Experiment finished."
echo "Results saved to ${SCRIPT_DIR}/results.csv"
