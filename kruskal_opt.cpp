#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

// ── Edge structure ────────────────────────────────────────────────────
struct Edge {
    int u, v, w;
    bool operator<(const Edge& o) const { return w < o.w; }
};

// ── Optimized Union-Find: path compression + union by rank ────────────
// find() amortised O(alpha(V)) — effectively O(1) for all practical n.
struct DSU {
    vector<int> parent, rank_;

    explicit DSU(int n) : parent(n), rank_(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    // Path compression: rewrites every node on the path to point directly
    // to the root. Subsequent calls on the same path cost O(1).
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // recursive path compression
        return parent[x];
    }

    // Union by rank: always attach the shorter tree under the taller one.
    // Keeps tree height O(log V), bounding find() depth.
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;  // same component — cycle detected

        // Attach smaller-rank tree under larger-rank tree
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;  // rank increases only on tie
        return true;
    }
};

// ── Kruskal's MST — Optimized ─────────────────────────────────────────
// Time:  O(E log E) for sort  +  O(E * alpha(V)) for DSU  ≈  O(E log E)
// Space: O(V + E)
// Returns the list of MST edges.
vector<Edge> kruskalOpt(vector<Edge> edges, int n) {
    sort(edges.begin(), edges.end());  // sort by weight ascending

    DSU dsu(n);
    vector<Edge> mst;
    mst.reserve(n - 1);  // MST has exactly n-1 edges — avoid reallocations

    for (const Edge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            // Early termination: once we have n-1 edges the MST is complete.
            // Avoids scanning remaining (potentially many) heavier edges.
            if ((int)mst.size() == n - 1) break;
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

    vector<Edge> mst = kruskalOpt(edges, n);

    int total = 0;
    cout << "Kruskal (Optimized DSU) MST edges:\n";
    for (const Edge& e : mst) {
        cout << "  " << e.u << " -- " << e.v
             << "  weight = " << e.w << "\n";
        total += e.w;
    }
    cout << "Total MST weight = " << total << "\n";

    // Verify: compile and run — expected MST for this graph:
    // 0-1 (2), 1-2 (3), 1-4 (5), 0-3 (6)  → total = 16
    return 0;
}
