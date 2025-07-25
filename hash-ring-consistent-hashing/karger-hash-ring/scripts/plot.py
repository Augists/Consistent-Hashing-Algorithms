import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Define paths
base_path = os.path.dirname(os.path.abspath(__file__))
go_results_dist_path = os.path.join(base_path, "..", "go", "results", "results_go.csv")
go_results_remap_path = os.path.join(base_path, "..", "go", "results", "remapping_results_go.csv")
output_dir = os.path.join(base_path, "..", "results")

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# Read the distribution results
df_dist_go = pd.DataFrame()

try:
    df_dist_go = pd.read_csv(go_results_dist_path)
    df_dist_go['Algorithm'] = 'Go'
except FileNotFoundError:
    print(f"Warning: {go_results_dist_path} not found.")

df_dist = df_dist_go

if df_dist.empty:
    print("Error: No distribution results found. Please run the experiments first.")

# Plotting Key Distribution
if not df_dist.empty:
    plt.figure(figsize=(12, 8))
    sns.barplot(data=df_dist, x="Node", y="Keys", hue="Algorithm", dodge=True)

    plt.title("Key Distribution Across Nodes (Karger Hash Ring)")
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

# Read the remapping results
df_remap_go = pd.DataFrame()

try:
    df_remap_go = pd.read_csv(go_results_remap_path)
    df_remap_go['Algorithm'] = 'Go'
except FileNotFoundError:
    print(f"Warning: {go_results_remap_path} not found.")

df_remap = df_remap_go

if df_remap.empty:
    print("Error: No remapping results found. Please run the experiments first.")

# Calculate remapping percentage and plot
if not df_remap.empty:
    df_remap['RemappedPercentage'] = (df_remap['RemappedKeys'] / df_remap['TotalKeys']) * 100

    plt.figure(figsize=(10, 6))
    sns.barplot(data=df_remap, x="Operation", y="RemappedPercentage", hue="Algorithm", dodge=True)

    plt.title("Impact of Node Addition/Removal on Key Remapping (Karger Hash Ring)")
    plt.xlabel("Operation")
    plt.ylabel("Percentage of Keys Remapped (%)")
    plt.ylim(0, 100)
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "remapping_impact.png"))

    print(f"Remapping impact plot saved to {os.path.join(output_dir, 'remapping_impact.png')}")
