import matplotlib.pyplot as plt
import csv
import os

result_file = os.path.join(os.path.dirname(__file__), '../results/dx_experiment.csv')
if not os.path.exists(result_file):
    print('Please run the experiment to generate dx_experiment.csv')
    exit(1)

x = []
y = []
with open(result_file) as f:
    reader = csv.reader(f)
    for row in reader:
        if row[0] == 'peak_to_average':
            x.append('Peak/Average Ratio')
            y.append(float(row[1]))

plt.bar(x, y)
plt.ylabel('Value')
plt.title('DxHash Experiment Results')
for i, v in enumerate(y):
    plt.text(i, v, f'{v:.4f}', ha='center', va='bottom')
plt.tight_layout()
plt.savefig(os.path.join(os.path.dirname(__file__), '../results/dx_experiment.png'))
plt.show()
