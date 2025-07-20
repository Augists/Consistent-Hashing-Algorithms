import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the distribution results
try:
    df_dist = pd.read_csv("results.csv")
except FileNotFoundError:
    print("Error: results.csv not found. Please run the experiments first.")
    exit(1)

# Plotting Key Distribution
plt.figure(figsize=(12, 8))
sns.barplot(data=df_dist, x="Node", y="Keys", hue="Algorithm", dodge=True)

plt.title("Key Distribution Across Nodes")
plt.xlabel("Node")
plt.ylabel("Number of Keys")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("key_distribution.png") # Changed filename for clarity

# Calculate and print peak-to-average ratio
print("\n--- Peak-to-Average Ratio ---")
for algorithm in df_dist['Algorithm'].unique():
    algo_df = df_dist[df_dist['Algorithm'] == algorithm]
    max_keys = algo_df['Keys'].max()
    avg_keys = algo_df['Keys'].mean()
    peak_to_average_ratio = max_keys / avg_keys
    print(f"{algorithm}: {peak_to_average_ratio:.2f}")

print("\nKey distribution plot saved to key_distribution.png")


# Read the remapping results
try:
    df_remap = pd.read_csv("remapping_results.csv")
except FileNotFoundError:
    print("Error: remapping_results.csv not found. Please run the experiments first.")
    exit(1)

# Calculate remapping percentage
df_remap['RemappedPercentage'] = (df_remap['RemappedKeys'] / df_remap['TotalKeys']) * 100

# Plotting Remapping Impact
plt.figure(figsize=(10, 6))
sns.barplot(data=df_remap, x="Operation", y="RemappedPercentage", hue="Algorithm", dodge=True)

plt.title("Impact of Node Addition/Removal on Key Remapping")
plt.xlabel("Operation")
plt.ylabel("Percentage of Keys Remapped (%)")
plt.ylim(0, 100) # Percentage should be between 0 and 100
plt.tight_layout()
plt.savefig("remapping_impact.png")

print("Remapping impact plot saved to remapping_impact.png")