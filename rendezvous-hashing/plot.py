import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the results
try:
    df = pd.read_csv("results.csv")
except FileNotFoundError:
    print("Error: results.csv not found. Please run the experiments first.")
    exit(1)

# Plotting
plt.figure(figsize=(12, 8))
sns.barplot(data=df, x="Node", y="Keys", hue="Algorithm", dodge=True)

plt.title("Key Distribution Across Nodes (Rendezvous Hashing)")
plt.xlabel("Node")
plt.ylabel("Number of Keys")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("rendezvous_hashing_distribution.png")

print("Plot saved to rendezvous_hashing_distribution.png")
