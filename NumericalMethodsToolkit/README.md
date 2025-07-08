# 🧠 Numerical Methods & Function Approximation Toolkit

This academic project showcases a series of advanced numerical techniques implemented in Python. The core topics include function interpolation, root-finding, numerical integration, and model fitting under noise. Each component is structured as an independent module with robust test cases and is optimized for performance and accuracy.

---

## 📁 Contents

| File            | Description |
|------------------|-------------|
| `assignment1.py` | High-accuracy interpolation using cubic splines with Chebyshev node optimization. |
| `assignment2.py` | Efficient root-finding for function intersections using adaptive Brent’s method. |
| `assignment3.py` | Precision area computation between intersecting functions using float32 integration techniques. |
| `assignment4.py` | Curve fitting for noisy data via custom least squares without external optimizers. |

---

## 🧩 Key Features

- ✅ **Chebyshev-based interpolation** to minimize Runge's phenomenon and reduce evaluation calls.
- 🔁 **Brent's method** and recursive adaptive refinement for reliable root isolation.
- 🧮 **Custom numerical integration** using trapezoidal, Simpson’s, and 3/8 rules, all constrained to float32 precision.
- 📉 **Noise-resilient polynomial fitting** using Chebyshev sampling and manual least squares calculation (no external optimizers allowed).
- ⏱️ **Optimized runtime performance** and hard runtime limits for fit convergence under noisy conditions.

---

## 🔧 Installation

No installation needed. Just clone the repository and ensure dependencies are available:

```bash
git clone https://github.com/YOUR_USERNAME/numerical-methods-toolkit.git
cd numerical-methods-toolkit
pip install -r requirements.txt  # if using external requirements like matplotlib or tqdm
```

---

## ▶️ Running the Tests

Each assignment includes a built-in unit testing suite. To run all tests:

```bash
python assignment1.py
python assignment2.py
python assignment3.py
python assignment4.py
```

Ensure all necessary utility files (`functionUtils.py`, `sampleFunctions.py`, `commons.py`) are available in the directory.

---

## 🧠 Skills Demonstrated

- Python programming (OOP, NumPy, Torch)
- Numerical analysis and computational math
- Optimization under runtime/memory constraints
- Data smoothing, interpolation, and regression
- Unit testing and reproducibility in scientific code

---

## 👤 Author

**Moran Shavit**  
Undergraduate student passionate about numerical computing and algorithm design.

---

## 📄 License

This project is for academic and personal use only. Reach out before reuse in commercial or academic work.
