import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Define paths
base_path = os.path.dirname(os.path.abspath(__file__))
c_results_path = os.path.join(base_path, "..", "c", "results", "results_c.csv")
go_results_path = os.path.join(base_path, "..", "go", "results", "results_go.csv")
output_dir = os.path.join(base_path, "..", "results")

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# Read the results
df_c = pd.DataFrame()
df_go = pd.DataFrame()

try:
    df_c = pd.read_csv(c_results_path)
    df_c['Algorithm'] = 'C'
except FileNotFoundError:
    print(f"Warning: {c_results_path} not found.")

try:
    df_go = pd.read_csv(go_results_path)
    df_go['Algorithm'] = 'Go'
except FileNotFoundError:
    print(f"Warning: {go_results_path} not found.")

df = pd.concat([df_c, df_go], ignore_index=True)

if df.empty:
    print("Error: No results found. Please run the experiments first.")

# Plotting
if not df.empty:
    plt.figure(figsize=(12, 8))
    sns.barplot(data=df, x="Node", y="Keys", hue="Algorithm", dodge=True)

    plt.title("Key Distribution Across Nodes")
    plt.xlabel("Node")
    plt.ylabel("Number of Keys")
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "k_failure_disruption.png"))

    print(f"Plot saved to {os.path.join(output_dir, 'k_failure_disruption.png')}")
