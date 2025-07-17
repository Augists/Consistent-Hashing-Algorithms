import pandas as pd
import matplotlib.pyplot as plt
import os

# Get the directory of the script
script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, 'results.csv')
output_path = os.path.join(script_dir, 'k_failure_disruption.png')

# Check if the results file exists
if not os.path.exists(csv_path):
    print(f"Error: Results file not found at {csv_path}")
    print("Please run './run_experiment.sh' first to generate the results.")
    exit(1)

# Read the data using pandas
try:
    data = pd.read_csv(csv_path)
except pd.errors.EmptyDataError:
    print(f"Error: The file {csv_path} is empty.")
    print("The experiment might have failed to produce output.")
    exit(1)


# Create the plot
plt.style.use('seaborn-v0_8-whitegrid')
fig, ax = plt.subplots(figsize=(10, 6))

ax.plot(data['k'], data['disruption_percent'], marker='.', linestyle='-', label='Maglev Disruption')

# Set titles and labels
ax.set_title('Maglev Lookup Table Disruption vs. Number of Failures (k)')
ax.set_xlabel('Number of Failed Nodes (k)')
ax.set_ylabel('Disruption (%)')
ax.legend()
ax.grid(True)

# Set axis limits
ax.set_xlim(0, data['k'].max())
ax.set_ylim(0, data['disruption_percent'].max() * 1.1) # Add 10% padding to y-axis

# Save the plot to a file
plt.savefig(output_path, dpi=300)

print(f"Plot saved successfully to {output_path}")
