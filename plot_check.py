import pandas as pd
import matplotlib.pyplot as plt

# ---- Config ----
CSV_FILE = "snapshots_4/item_list.csv"  # change to your filename

# ---- Load ----
df = pd.read_csv(CSV_FILE)

# ---- Plot ----
plt.figure(figsize=(8,8))

# scatter plot by label
for label, group in df.groupby("label"):
    plt.scatter(group["x"], group["y"], label=label, alpha=0.7)

plt.title("Ant Clustering Visualization")
plt.xlabel("X position")
plt.ylabel("Y position")
plt.gca().invert_yaxis()   # optional: makes grid look like a matrix
plt.legend(title="Label")
plt.grid(True, linestyle="--", alpha=0.5)
plt.show()
