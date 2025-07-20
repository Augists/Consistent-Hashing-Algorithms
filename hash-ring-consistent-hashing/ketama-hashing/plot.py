import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re

def parse_experiment_output(filepath, algorithm_prefix):
    """Parses the raw experiment output from C or Go into two DataFrames."""
    with open(filepath, 'r') as f:
        content = f.read()

    # Extract distribution data
    dist_match = re.search(rf'{algorithm_prefix}_DIST_START\n(.*?)\n{algorithm_prefix}_DIST_END', content, re.DOTALL)
    dist_df = pd.DataFrame()
    if dist_match:
        dist_csv = dist_match.group(1)
        dist_df = pd.read_csv(pd.io.common.StringIO(dist_csv))

    # Extract remapping data
    remap_match = re.search(rf'{algorithm_prefix}_REMAP_START\n(.*?)\n{algorithm_prefix}_REMAP_END', content, re.DOTALL)
    remap_df = pd.DataFrame()
    if remap_match:
        remap_csv = remap_match.group(1)
        remap_df = pd.read_csv(pd.io.common.StringIO(remap_csv))

    return dist_df, remap_df

# Parse C and Go outputs
c_dist_df, c_remap_df = parse_experiment_output('c_output.txt', 'KETAMA')
go_dist_df, go_remap_df = parse_experiment_output('go_output.txt', 'KETAMA')

# Combine distribution results
df_dist = pd.concat([c_dist_df, go_dist_df], ignore_index=True)

# Plotting Key Distribution
plt.figure(figsize=(12, 8))
sns.barplot(data=df_dist, x="Node", y="Keys", hue="Algorithm", dodge=True)

plt.title("Key Distribution Across Nodes (Ketama Hashing)")
plt.xlabel("Node")
plt.ylabel("Number of Keys")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("key_distribution.png")

# Calculate and print peak-to-average ratio
print("\n--- Peak-to-Average Ratio ---")
for algorithm in df_dist['Algorithm'].unique():
    algo_df = df_dist[df_dist['Algorithm'] == algorithm]
    max_keys = algo_df['Keys'].max()
    avg_keys = algo_df['Keys'].mean()
    peak_to_average_ratio = max_keys / avg_keys
    print(f"{algorithm}: {peak_to_average_ratio:.2f}")

print("\nKey distribution plot saved to key_distribution.png")

# Combine remapping results
df_remap = pd.concat([c_remap_df, go_remap_df], ignore_index=True)

# Calculate remapping percentage
df_remap['RemappedPercentage'] = (df_remap['RemappedKeys'] / df_remap['TotalKeys']) * 100

# Plotting Remapping Impact
plt.figure(figsize=(10, 6))
sns.barplot(data=df_remap, x="Operation", y="RemappedPercentage", hue="Algorithm", dodge=True)

plt.title("Impact of Node Addition/Removal on Key Remapping (Ketama Hashing)")
plt.xlabel("Operation")
plt.ylabel("Percentage of Keys Remapped (%)")
plt.ylim(0, 100)
plt.tight_layout()
plt.savefig("remapping_impact.png")

print("Remapping impact plot saved to remapping_impact.png")