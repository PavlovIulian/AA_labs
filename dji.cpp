#include <vector>
#include <climits>
#include <iostream>
using namespace std;

vector<int> dijkstraNaive(const vector<vector<pair<int,int>>>& adj, int src) {
    int n = (int)adj.size();
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);
    dist[src] = 0;

    for (int iter = 0; iter < n; iter++) {
        // O(V) linear scan for minimum
        int u = -1;
        for (int v = 0; v < n; v++)
            if (!visited[v] && (u == -1 || dist[v] < dist[u]))
                u = v;

        if (u == -1 || dist[u] == INT_MAX) break;
        visited[u] = true;

        for (auto [nb, w] : adj[u])
            if (dist[u] + w < dist[nb])
                dist[nb] = dist[u] + w;
    }
    return dist;
}

int main() {
    int n = 5;
    vector<vector<pair<int,int>>> adj(n);
    adj[0].push_back({1, 10});
    adj[0].push_back({2, 3});
    adj[1].push_back({3, 2});
    adj[2].push_back({1, 4});
    adj[2].push_back({3, 8});
    adj[3].push_back({4, 5});

    vector<int> dist = dijkstraNaive(adj, 0);
    for (int i = 0; i < n; i++)
        cout << "dist[0][" << i << "] = " << dist[i] << "\n";
}