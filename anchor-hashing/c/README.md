# AnchorHash C Implementation

本目录为 AnchorHash 算法的 C 语言实现，包含：
- `anchor_hash.c` / `anchor_hash.h`：算法核心实现
- `test_anchor_hash.c`：单元测试
- `experiment.c`：实验代码（峰均值比、扩缩容重映射等）
- `Makefile`：一键编译、测试、实验
- `scripts/plot.py`：实验结果绘图脚本
- `results/`：实验输出数据与图像

## 快速开始

1. `make test`  # 运行单元测试
2. `make experiment`  # 运行实验
3. `python3 scripts/plot.py`  # 绘制实验图像

详细算法原理、优劣分析、实验方法与结果请见主目录 README。
