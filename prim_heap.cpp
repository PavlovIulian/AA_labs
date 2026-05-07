#include <vector>
#include <queue>
#include <climits>
#include <iostream>
using namespace std;

using P = pair<int, int>;  // {weight, vertex}

// ── Prim's MST — Optimized O((V+E) log V), adjacency list + min-heap ─
// Returns parent array: parent[v] = u means edge (u,v) is in the MST.
vector<int> primHeap(const vector<vector<P>>& adj, int n) {
    vector<int>  key(n, INT_MAX);
    vector<bool> inMST(n, false);
    vector<int>  parent(n, -1);

    // min-heap: {key_value, vertex}
    priority_queue<P, vector<P>, greater<P>> pq;

    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();

        // Lazy deletion: stale entry — vertex already added to MST
        if (inMST[u]) continue;
        inMST[u] = true;

        for (auto [w, v] : adj[u]) {
            if (!inMST[v] && w < key[v]) {
                key[v]    = w;
                parent[v] = u;
                pq.push({w, v});  // lazy update — old entry becomes stale
            }
        }
    }
    return parent;
}

// ── Helper: total MST weight from parent array ────────────────────────
int mstWeight(const vector<int>& parent,
              const vector<vector<P>>& adj, int n) {
    // Rebuild a lookup from the parent array using key values
    // Simpler: re-derive from adjacency list
    vector<int> key(n, INT_MAX);
    {
        vector<bool> inMST(n, false);
        priority_queue<P, vector<P>, greater<P>> pq;
        key[0] = 0; pq.push({0, 0});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true;
            for (auto [w, v] : adj[u])
                if (!inMST[v] && w < key[v]) { key[v] = w; pq.push({w, v}); }
        }
    }
    int total = 0;
    for (int v = 1; v < n; v++) total += key[v];
    return total;
}

// ── Demo ─────────────────────────────────────────────────────────────
int main() {
    int n = 5;
    vector<vector<P>> adj(n);

    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    };
    addEdge(0, 1, 2);
    addEdge(0, 3, 6);
    addEdge(1, 2, 3);
    addEdge(1, 3, 8);
    addEdge(1, 4, 5);
    addEdge(2, 4, 7);
    addEdge(3, 4, 9);

    vector<int> parent = primHeap(adj, n);

    cout << "Prim (Heap) MST edges:\n";
    // Retrieve edge weights for display
    vector<int> key(n, INT_MAX);
    {
        vector<bool> inMST(n, false);
        priority_queue<P, vector<P>, greater<P>> pq;
        key[0] = 0; pq.push({0, 0});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true;
            for (auto [w, v] : adj[u])
                if (!inMST[v] && w < key[v]) { key[v] = w; pq.push({w, v}); }
        }
    }
    int total = 0;
    for (int v = 1; v < n; v++) {
        cout << "  " << parent[v] << " -- " << v
             << "  weight = " << key[v] << "\n";
        total += key[v];
    }
    cout << "Total MST weight = " << total << "\n";

    return 0;
}
