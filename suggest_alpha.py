import numpy as np
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from scipy.spatial.distance import pdist

# ---- Config ----
CSV_FILE = "data/base_4_grupos.csv"   # <- change to your dataset file
SUGGESTION_FACTORS = [0.25, 0.5, 1.0]  # multipliers for mean distance

# ---- Load data ----
# Assuming items.csv has rows = items, cols = features (no labels in this version)
df = pd.read_csv(CSV_FILE)

# Drop non-numeric columns if any (e.g., labels)
X = df.select_dtypes(include=[np.number]).values

# Normalize to [0,1]
scaler = MinMaxScaler()
X_scaled = scaler.fit_transform(X)

# ---- Pairwise distances ----
dists = pdist(X_scaled, metric="euclidean")

d_mean = np.mean(dists)
d_median = np.median(dists)
d_min, d_max = np.min(dists), np.max(dists)
d_p25, d_p75 = np.percentile(dists, [25, 75])

# ---- Suggestions ----
print("Distance stats (after normalization):")
print(f"  mean    = {d_mean:.4f}")
print(f"  median  = {d_median:.4f}")
print(f"  min     = {d_min:.4f}, max = {d_max:.4f}")
print(f"  25%     = {d_p25:.4f}, 75% = {d_p75:.4f}")

print("\nSuggested ALPHA candidates:")
for f in SUGGESTION_FACTORS:
    print(f"  ALPHA ≈ {d_mean*f:.4f}   (factor {f})")
