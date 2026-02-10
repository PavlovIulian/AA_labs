import time
import math
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec


def fib_recursive(n):
    if n <= 1:
        return n
    return fib_recursive(n - 1) + fib_recursive(n - 2)

def fib_dp(n):
    if n <= 1:
        return n
    dp = [0, 1]
    for i in range(2, n + 1):
        dp.append(dp[i - 1] + dp[i - 2])
    return dp[n]

def mat_mul(A, B):
    return [
        [A[0][0]*B[0][0] + A[0][1]*B[1][0],  A[0][0]*B[0][1] + A[0][1]*B[1][1]],
        [A[1][0]*B[0][0] + A[1][1]*B[1][0],  A[1][0]*B[0][1] + A[1][1]*B[1][1]],
    ]

def mat_pow(M, n):
    if n == 1:
        return M
    if n % 2 == 0:
        half = mat_pow(M, n // 2)
        return mat_mul(half, half)
    return mat_mul(M, mat_pow(M, n - 1))

def fib_matrix(n):
    if n <= 1:
        return n
    M = [[1, 1], [1, 0]]
    result = mat_pow(M, n)
    return result[0][1]

def fib_binet(n):
    phi = (1 + math.sqrt(5)) / 2
    return round(phi**n / math.sqrt(5))

def _fast_doubling(n):
    if n == 0:
        return (0, 1)
    a, b = _fast_doubling(n >> 1)
    c = a * (2 * b - a)
    d = a * a + b * b
    return (c, d) if n % 2 == 0 else (d, c + d)

def fib_fast_doubling(n):
    return _fast_doubling(n)[0]


METHODS = [
    {
        "name":       "Recursive",
        "fn":         fib_recursive,
        "max_n":      25,
        "color":      "#ff6b6b",
        "complexity": "O(2ⁿ)",
    },
    {
        "name":       "Dynamic Programming",
        "fn":         fib_dp,
        "max_n":      40,
        "color":      "#ffd93d",
        "complexity": "O(n)",
    },
    {
        "name":       "Matrix Power",
        "fn":         fib_matrix,
        "max_n":      40,
        "color":      "#6bcb77",
        "complexity": "O(log n)",
    },
    {
        "name":       "Binet Formula",
        "fn":         fib_binet,
        "max_n":      40,
        "color":      "#4ecdc4",
        "complexity": "O(1)*",
    },
    {
        "name":       "Fast Doubling",
        "fn":         fib_fast_doubling,
        "max_n":      40,
        "color":      "#c77dff",
        "complexity": "O(log n)",
    },
]

DEMO_N = 50   # value used for the terminal result printout


# ─────────────────────────────────────────────
# TERMINAL OUTPUT
# ─────────────────────────────────────────────
RESET  = "\033[0m"
BOLD   = "\033[1m"
CYAN   = "\033[96m"
YELLOW = "\033[93m"
GREEN  = "\033[92m"
RED    = "\033[91m"
PURPLE = "\033[95m"
TEAL   = "\033[36m"
GRAY   = "\033[90m"
WHITE  = "\033[97m"

COLORS = [RED, YELLOW, GREEN, TEAL, PURPLE]

print(f"\n{BOLD}{WHITE}{'━'*60}")
print(f"  φ  FIBONACCI ALGORITHMS  —  Results for n = {DEMO_N}")
print(f"{'━'*60}{RESET}\n")

for method, color in zip(METHODS, COLORS):
    fn  = method["fn"]
    max_n = method["max_n"]

    t_start = time.perf_counter()
    result  = fn(DEMO_N)
    elapsed = (time.perf_counter() - t_start) * 1000   # ms

    seq = [fn(i) for i in range(11)]   # F(0)…F(10)
    seq_str = ", ".join(str(v) for v in seq) + ", ..."

    print(f"{color}{BOLD}  {method['name']:<24}{RESET}  {GRAY}[{method['complexity']}]{RESET}")
    print(f"    F({DEMO_N})  =  {BOLD}{WHITE}{result}{RESET}")
    print(f"    time   =  {GRAY}{elapsed:.5f} ms{RESET}")
    print(f"    seq    =  {GRAY}{seq_str}{RESET}")
    print()

print(f"{BOLD}{WHITE}{'━'*60}{RESET}\n")


plt.style.use("dark_background")

fig = plt.figure(figsize=(18, 14), facecolor="#0a0a12")
fig.suptitle(
    "Fibonacci Algorithms — F(n) Growth",
    fontsize=20, fontweight="bold", color="white", y=0.98
)

# 6 subplots: 2 rows × 3 cols (first 5 individual, last is comparison)
gs = gridspec.GridSpec(2, 3, figure=fig, hspace=0.45, wspace=0.35)

axes = [fig.add_subplot(gs[i // 3, i % 3]) for i in range(5)]
ax_cmp = fig.add_subplot(gs[1, 2])

for ax, method in zip(axes, METHODS):
    ns  = list(range(method["max_n"] + 1))
    fns = [method["fn"](n) for n in ns]

    ax.fill_between(ns, fns, alpha=0.15, color=method["color"])
    ax.plot(ns, fns, color=method["color"], linewidth=2.2, zorder=3)
    ax.scatter(ns, fns, color=method["color"], s=16, zorder=4)

    ax.set_facecolor("#0d0d1a")
    ax.set_title(
        f"{method['name']}  [{method['complexity']}]",
        fontsize=11, fontweight="bold", color=method["color"], pad=8
    )
    ax.set_xlabel("n", fontsize=9, color="#888")
    ax.set_ylabel("F(n)", fontsize=9, color="#888")
    ax.tick_params(colors="#666", labelsize=8)
    for spine in ax.spines.values():
        spine.set_edgecolor("#2a2a3a")
    ax.grid(color="#1e1e2e", linewidth=0.8, zorder=0)

    # annotate last point
    ax.annotate(
        f"F({ns[-1]})={fns[-1]:,}",
        xy=(ns[-1], fns[-1]),
        xytext=(-10, 8), textcoords="offset points",
        fontsize=7, color=method["color"], alpha=0.9,
        ha="right"
    )

# ── Comparison subplot (all methods, same N=25)
COMP_N = 25
comp_ns = list(range(COMP_N + 1))

ax_cmp.set_facecolor("#0d0d1a")
ax_cmp.set_title("All Methods  [n = 0…25]", fontsize=11,
                  fontweight="bold", color="white", pad=8)
ax_cmp.set_xlabel("n", fontsize=9, color="#888")
ax_cmp.set_ylabel("F(n)", fontsize=9, color="#888")
ax_cmp.tick_params(colors="#666", labelsize=8)
for spine in ax_cmp.spines.values():
    spine.set_edgecolor("#2a2a3a")
ax_cmp.grid(color="#1e1e2e", linewidth=0.8, zorder=0)

for method, color in zip(METHODS, COLORS):
    vals = [method["fn"](n) for n in comp_ns]
    ax_cmp.plot(comp_ns, vals, color=method["color"], linewidth=1.8,
                label=method["name"].split()[0], zorder=3)

ax_cmp.legend(
    fontsize=8, framealpha=0.2, facecolor="#111122",
    edgecolor="#333344", labelcolor="white",
    loc="upper left"
)

out_path = r"C:\Users\pavlo\OneDrive\Desktop\oop\AA_labs\graphs\fibonacci_graphs.png"
plt.savefig(out_path, dpi=150, bbox_inches="tight", facecolor="#0a0a12")
plt.close()

print(f"  Graph saved → {out_path}\n")