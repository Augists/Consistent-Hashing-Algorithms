import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re
import os

def parse_experiment_output(filepath, algorithm_prefix, algorithm_name):
    """Parses the raw experiment output from C or Go into two DataFrames."""
    try:
        with open(filepath, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Warning: {filepath} not found. Skipping data for {algorithm_name}.")
        return pd.DataFrame(), pd.DataFrame()

    # Extract distribution data
    dist_match = re.search(rf'{algorithm_prefix}_DIST_START\n(.*?)\n{algorithm_prefix}_DIST_END', content, re.DOTALL)
    dist_df = pd.DataFrame()
    if dist_match:
        dist_csv = dist_match.group(1)
        dist_df = pd.read_csv(pd.io.common.StringIO(dist_csv))
        dist_df['Algorithm'] = algorithm_name

    # Extract remapping data
    remap_match = re.search(rf'{algorithm_prefix}_REMAP_START\n(.*?)\n{algorithm_prefix}_REMAP_END', content, re.DOTALL)
    remap_df = pd.DataFrame()
    if remap_match:
        remap_csv = remap_match.group(1)
        remap_df = pd.read_csv(pd.io.common.StringIO(remap_csv))
        remap_df['Algorithm'] = algorithm_name

    return dist_df, remap_df

# Define paths
base_path = os.path.dirname(os.path.abspath(__file__))
c_output_path = os.path.join(base_path, "..", "c", "results", "c_output.txt")
go_output_path = os.path.join(base_path, "..", "go", "results", "go_output.txt")
output_dir = os.path.join(base_path, "..", "results")

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# Parse C and Go outputs
c_dist_df, c_remap_df = parse_experiment_output(c_output_path, 'JUMP', 'C')
go_dist_df, go_remap_df = parse_experiment_output(go_output_path, 'JUMP', 'Go')

# Combine distribution results
df_dist = pd.concat([c_dist_df, go_dist_df], ignore_index=True)

if df_dist.empty:
    print("Error: No distribution results found. Please run the experiments first.")

# Plotting Key Distribution
if not df_dist.empty:
    plt.figure(figsize=(12, 8))
    sns.barplot(data=df_dist, x="Node", y="Keys", hue="Algorithm", dodge=True)

    plt.title("Key Distribution Across Nodes (Jump Consistent Hashing)")
    plt.xlabel("Node")
    plt.ylabel("Number of Keys")
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "key_distribution.png"))

    # Calculate and print peak-to-average ratio
    print("\n--- Peak-to-Average Ratio ---")
    for algorithm in df_dist['Algorithm'].unique():
        algo_df = df_dist[df_dist['Algorithm'] == algorithm]
        max_keys = algo_df['Keys'].max()
        avg_keys = algo_df['Keys'].mean()
        peak_to_average_ratio = max_keys / avg_keys
        print(f"{algorithm}: {peak_to_average_ratio:.2f}")

    print(f"\nKey distribution plot saved to {os.path.join(output_dir, 'key_distribution.png')}")

# Combine remapping results
df_remap = pd.concat([c_remap_df, go_remap_df], ignore_index=True)

if df_remap.empty:
    print("Error: No remapping results found. Please run the experiments first.")

# Calculate remapping percentage and plot
if not df_remap.empty:
    df_remap['RemappedPercentage'] = (df_remap['RemappedKeys'] / df_remap['TotalKeys']) * 100

    plt.figure(figsize=(10, 6))
    sns.barplot(data=df_remap, x="Operation", y="RemappedPercentage", hue="Algorithm", dodge=True)

    plt.title("Impact of Node Addition/Removal on Key Remapping (Jump Consistent Hashing)")
    plt.xlabel("Operation")
    plt.ylabel("Percentage of Keys Remapped (%)")
    plt.ylim(0, 100)
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "remapping_impact.png"))

    print(f"Remapping impact plot saved to {os.path.join(output_dir, 'remapping_impact.png')}")
