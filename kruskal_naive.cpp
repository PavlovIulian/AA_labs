#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

// ── Edge structure ────────────────────────────────────────────────────
struct Edge {
    int u, v, w;
    bool operator<(const Edge& o) const { return w < o.w; }
};

// ── Simple Union-Find (NO optimizations) ─────────────────────────────
// find() can degenerate to O(V) on chain-shaped trees.
// unite() uses naive parent assignment with no rank balancing.
struct DSU_Naive {
    vector<int> parent;

    explicit DSU_Naive(int n) : parent(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    // O(n) worst case: follows chain to root with no path compression
    int find(int x) {
        while (parent[x] != x)
            x = parent[x];
        return x;
    }

    // No union by rank: always assigns parent[a] = b
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;  // same component — would form a cycle
        parent[a] = b;
        return true;
    }
};

// ── Kruskal's MST — Original (naive DSU) ─────────────────────────────
// Time: O(E log E) for sort + O(E * V) worst case for naive DSU
// Returns the list of MST edges.
vector<Edge> kruskalNaive(vector<Edge> edges, int n) {
    sort(edges.begin(), edges.end());  // sort by weight ascending

    DSU_Naive dsu(n);
    vector<Edge> mst;

    for (const Edge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            if ((int)mst.size() == n - 1) break;  // MST complete
        }
    }
    return mst;
}

// ── Demo ─────────────────────────────────────────────────────────────
int main() {
    int n = 5;
    vector<Edge> edges = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9},
    };

    vector<Edge> mst = kruskalNaive(edges, n);

    int total = 0;
    cout << "Kruskal (Naive DSU) MST edges:\n";
    for (const Edge& e : mst) {
        cout << "  " << e.u << " -- " << e.v
             << "  weight = " << e.w << "\n";
        total += e.w;
    }
    cout << "Total MST weight = " << total << "\n";

    return 0;
}
