import matplotlib.pyplot as plt
import csv
import os

result_file = os.path.join(os.path.dirname(__file__), '../results/anchor_remap_curve.csv')
if not os.path.exists(result_file):
    print('Please run the experiment to generate anchor_remap_curve.csv')
    exit(1)

x = []
y = []
with open(result_file) as f:
    reader = csv.reader(f)
    next(reader)  # skip header
    for row in reader:
        x.append(float(row[0]))
        y.append(float(row[1]))

plt.plot(x, y, marker='o')
plt.xlabel('Remove Ratio')
plt.ylabel('Remapping Ratio')
plt.title('AnchorHash Remapping Curve')
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(os.path.dirname(__file__), '../results/anchor_remap_curve.png'))
plt.show()
