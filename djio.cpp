#include <vector>
#include <queue>
#include <climits>
#include <iostream>
using namespace std;

vector<int> dijkstraHeap(const vector<vector<pair<int,int>>>& adj, int src) {
    int n = (int)adj.size();
    vector<int> dist(n, INT_MAX);
    // min-heap: {distance, vertex}
    priority_queue<pair<int,int>,
                   vector<pair<int,int>>,
                   greater<pair<int,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // stale entry — skip

        for (auto [nb, w] : adj[u]) {
            if (dist[u] + w < dist[nb]) {
                dist[nb] = dist[u] + w;
                pq.push({dist[nb], nb});  // lazy deletion
            }
        }
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

    vector<int> dist = dijkstraHeap(adj, 0);
    for (int i = 0; i < n; i++)
        cout << "dist[0][" << i << "] = " << dist[i] << "\n";
}       