import matplotlib.pyplot as plt
import pandas as pd
import os

# Define paths
results_dir = os.path.join(os.path.dirname(__file__), '..', 'results')

# Ensure results directory exists
os.makedirs(results_dir, exist_ok=True)

# --- Plot Peak-to-Average Ratio ---
peak_to_average_file = os.path.join(results_dir, 'anchor_experiment.csv')
if os.path.exists(peak_to_average_file):
    df_p2a = pd.read_csv(peak_to_average_file, header=None, names=['metric', 'value'])
    if not df_p2a.empty and 'metric' in df_p2a.columns and 'value' in df_p2a.columns and df_p2a['metric'].iloc[0] == 'peak_to_average':
        p2a_value = df_p2a['value'].iloc[0]
        print(f"Peak-to-Average Ratio: {p2a_value:.4f}")
    else:
        print(f"Warning: {peak_to_average_file} is empty or does not contain expected 'peak_to_average' data. Skipping Peak-to-Average plot.")
else:
    print(f"Warning: {peak_to_average_file} not found. Skipping Peak-to-Average plot.")

# --- Plot Remapping Removal Curve ---
remap_removal_file = os.path.join(results_dir, 'anchor_remap_removal_curve.csv')
if os.path.exists(remap_removal_file):
    df_removal = pd.read_csv(remap_removal_file)

    plt.figure(figsize=(10, 6))
    plt.plot(df_removal['remove_ratio'], df_removal['remap_ratio'], marker='o')
    plt.title('AnchorHash Remapping Ratio vs. Node Removal')
    plt.xlabel('Percentage of Nodes Removed')
    plt.ylabel('Percentage of Keys Remapped')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'remapping_removal_impact.png'))
    plt.close()
    print(f"Remapping removal curve plotted to {os.path.join(results_dir, 'remapping_removal_impact.png')}")
else:
    print(f"Warning: {remap_removal_file} not found. Skipping Remapping Removal plot.")

# --- Plot Remapping Addition Curve ---
remap_addition_file = os.path.join(results_dir, 'anchor_remap_addition_curve.csv')
if os.path.exists(remap_addition_file):
    df_addition = pd.read_csv(remap_addition_file)

    plt.figure(figsize=(10, 6))
    plt.plot(df_addition['add_ratio'], df_addition['remap_ratio'], marker='o', color='green')
    plt.title('AnchorHash Remapping Ratio vs. Node Addition')
    plt.xlabel('Percentage of Nodes Added Back (relative to initially removed)')
    plt.ylabel('Percentage of Keys Remapped')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'remapping_addition_impact.png'))
    plt.close()
    print(f"Remapping addition curve plotted to {os.path.join(results_dir, 'remapping_addition_impact.png')}")
else:
    print(f"Warning: {remap_addition_file} not found. Skipping Remapping Addition plot.")

print("Plotting complete.")
