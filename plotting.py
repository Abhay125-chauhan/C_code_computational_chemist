import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
import numpy as np
from scipy.stats import gaussian_kde

# --- Plot Config ---
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 8,
    "axes.linewidth": 1,
    "font.weight": "normal"
})

# --- Create figure and axis ---
fig, ax = plt.subplots(figsize=(3.5, 2.5))  # A4 column width ~8.9 cm

# --- Input files ---
files = ['Rg2.xvg']
labels = ['10_mer']
colors = ['blue']

# --- Plot type: choose "xy" or "kde" manually ---
plot_type = "kde"  # Change to "xy" for XY plot

for file, label, color in zip(files, labels, colors):
    if plot_type == "xy":
        x_vals = np.loadtxt(file, skiprows=27, usecols=0)
        y_vals = np.loadtxt(file, skiprows=27, usecols=1)
        ax.plot(x_vals, y_vals, linestyle='-', label=label, color=color, linewidth=1)
        ax.fill_between(x_vals, y_vals, color=color, alpha=0.2)
    elif plot_type == "kde":
        data = np.loadtxt(file, skiprows=27, usecols=1)
        kde = gaussian_kde(data, bw_method='scott')
        x_vals = np.linspace(min(data), max(data), 500)
        y_vals = kde(x_vals)
        ax.plot(x_vals, y_vals, linestyle='-', label=label, color=color, linewidth=1)
        ax.fill_between(x_vals, y_vals, color=color, alpha=0.2)
    else:
        raise ValueError("Plot type must be either 'xy' or 'kde'.")

# --- Axis Limits ---
ax.set_xlim(min(x_vals), max(x_vals)*0.8)
ax.set_ylim(0, max(y_vals) * 1.1)

# --- Labels and Title ---
if plot_type == "xy":
    ax.set_xlabel("Time (ps)", fontweight='normal', fontstyle='normal')
    ax.set_ylabel("Radius of Gyration (nm)", fontweight='normal', fontstyle='normal')
elif plot_type == "kde":
    ax.set_xlabel("Radius of Gyration (nm)", fontweight='normal', fontstyle='normal')
    ax.set_ylabel("Probability Density", fontweight='normal', fontstyle='normal')

# --- Ticks (adjust automatically) ---
xtick = (max(x_vals) - min(x_vals)) / 5
ytick = (max(y_vals)) / 5

ax.xaxis.set_major_locator(MultipleLocator(xtick))
ax.yaxis.set_major_locator(MultipleLocator(ytick))
ax.xaxis.set_minor_locator(MultipleLocator(xtick / 2))
ax.yaxis.set_minor_locator(MultipleLocator(ytick / 2))

ax.tick_params(
    which='major',
    length=5,
    width=1.2,
    labelsize=7,
    direction='in'
)
ax.tick_params(
    which='minor',
    length=3,
    width=1,
    direction='in'
)

# --- Legend ---
ax.legend(fontsize=8, frameon=False, loc='upper right')

# --- Save figure ---
plt.tight_layout()
plt.savefig("testplot.png", dpi=600, bbox_inches='tight')
plt.show()

## fontweight normal,bold,light,ultralight,bold,semibold,black
## fontstyle normal ,italic,oblique 
## colour colors = ['blue', 'red', 'green', 'orange', 'purple', 'cyan', 'magenta', 'yellow', 'black', 'gray']
