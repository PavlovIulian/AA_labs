#include <vector>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;

// ── Prim's MST — Naive O(V^2), adjacency matrix ──────────────────────
// Returns parent array: parent[v] = u means edge (u,v) is in the MST.
// Use INT_MAX/2 in the weight matrix (not INT_MAX) to avoid overflow.
vector<int> primNaive(const vector<vector<int>>& w, int n) {
    vector<int>  key(n, INT_MAX);   // minimum edge weight to reach vertex v
    vector<bool> inMST(n, false);   // is v already in the MST?
    vector<int>  parent(n, -1);     // MST parent of v

    key[0] = 0;  // start from vertex 0

    for (int iter = 0; iter < n; iter++) {
        // O(V) linear scan: find the unvisited vertex with minimum key
        int u = -1;
        for (int v = 0; v < n; v++)
            if (!inMST[v] && (u == -1 || key[v] < key[u]))
                u = v;

        inMST[u] = true;

        // Update keys of u's neighbours
        for (int v = 0; v < n; v++)
            if (w[u][v] && !inMST[v] && w[u][v] < key[v]) {
                key[v]    = w[u][v];
                parent[v] = u;
            }
    }
    return parent;
}

// ── Helper: total MST weight from parent array ────────────────────────
int mstWeight(const vector<int>& parent,
              const vector<vector<int>>& w, int n) {
    int total = 0;
    for (int v = 1; v < n; v++)
        total += w[parent[v]][v];
    return total;
}

// ── Demo ─────────────────────────────────────────────────────────────
int main() {
    // Example: 5-vertex graph (0-indexed)
    // Edge list:
    //  0-1 (2), 0-3 (6)
    //  1-2 (3), 1-3 (8), 1-4 (5)
    //  2-4 (7)
    //  3-4 (9)
    int n = 5;
    const int INF = 0;  // 0 means no edge in this matrix
    vector<vector<int>> w(n, vector<int>(n, 0));

    auto addEdge = [&](int u, int v, int weight) {
        w[u][v] = w[v][u] = weight;
    };
    addEdge(0, 1, 2);
    addEdge(0, 3, 6);
    addEdge(1, 2, 3);
    addEdge(1, 3, 8);
    addEdge(1, 4, 5);
    addEdge(2, 4, 7);
    addEdge(3, 4, 9);

    vector<int> parent = primNaive(w, n);

    cout << "Prim (Naive) MST edges:\n";
    for (int v = 1; v < n; v++)
        cout << "  " << parent[v] << " -- " << v
             << "  weight = " << w[parent[v]][v] << "\n";
    cout << "Total MST weight = " << mstWeight(parent, w, n) << "\n";

    return 0;
}
