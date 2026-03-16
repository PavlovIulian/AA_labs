#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>
#include <functional>

using namespace std;
using Clock = chrono::high_resolution_clock;

vector<vector<int>> makeGraph(int n, double p, mt19937& rng) {
    vector<vector<int>> adj(n);
    uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (dist(rng) < p) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
    return adj;
}

void bfsOriginal(const vector<vector<int>>& adj, int start) {
    unordered_set<int> visited;
    queue<int> q;
    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int nb : adj[v])
            if (!visited.count(nb)) {
                visited.insert(nb);
                q.push(nb);
            }
    }
}

void bfsOpt(const vector<vector<int>>& adj, int start) {
    vector<bool> visited(adj.size(), false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int nb : adj[v])
            if (!visited[nb]) {
                visited[nb] = true;
                q.push(nb);
            }
    }
}


vector<int> bfsShortestPath(const vector<vector<int>>& adj, int start) {
    int n = (int)adj.size();
    vector<int> dist(n, -1);  
    queue<int> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int nb : adj[v])
            if (dist[nb] == -1) {
                dist[nb] = dist[v] + 1;
                q.push(nb);
            }
    }
    return dist;
}

vector<int> bfsGetPath(const vector<vector<int>>& adj, int start, int end) {
    int n = (int)adj.size();
    vector<int> dist(n, -1), parent(n, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        if (v == end) break;
        for (int nb : adj[v])
            if (dist[nb] == -1) {
                dist[nb] = dist[v] + 1;
                parent[nb] = v;
                q.push(nb);
            }
    }

    if (dist[end] == -1) return {};   

    vector<int> path;
    for (int v = end; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

double benchMs(function<void(const vector<vector<int>>&, int)> fn,
               const vector<vector<int>>& adj, int reps = 7) {
    double t = 0;
    for (int i = 0; i < reps; i++) {
        auto t0 = Clock::now();
        fn(adj, 0);
        t += chrono::duration<double, milli>(Clock::now() - t0).count();
    }
    return t / reps;
}

int main() {
    mt19937 rng(42);

    cout << "==============================================\n";
    cout << "  BFS Benchmark  (random graph, p=0.15)\n";
    cout << "==============================================\n";
    cout << left  << setw(8)  << "n"
         << right << setw(16) << "BFS (orig)"
         << setw(16) << "BFS (opt)"
         << setw(10) << "Speedup"
         << "\n" << string(50, '-') << "\n";

    for (int n : {500, 1000, 2000, 3500, 5000}) {
        auto g = makeGraph(n, 0.15, rng);
        double to = benchMs(bfsOriginal, g);
        double tp = benchMs(bfsOpt,      g);
        cout << left  << setw(8) << n
             << right << setw(13) << fixed << setprecision(3) << to << " ms"
             <<          setw(13) << fixed << setprecision(3) << tp << " ms"
             <<          setw(8)  << fixed << setprecision(2) << to / tp << "x"
             << "\n";
    }

    cout << "\n--- Shortest Path Demo (n=8, p=0.4) ---\n";
    {
        int n = 8;
        auto g = makeGraph(n, 0.4, rng);
        auto dist = bfsShortestPath(g, 0);

        cout << "Distances from node 0:  ";
        for (int i = 0; i < n; i++)
            cout << "node" << i << ":" << dist[i] << "  ";
        cout << "\n";

        auto path = bfsGetPath(g, 0, n - 1);
        if (!path.empty()) {
            cout << "Shortest path 0 -> " << n-1 << ":  ";
            for (int i = 0; i < (int)path.size(); i++) {
                if (i) cout << " -> ";
                cout << path[i];
            }
            cout << "  (" << path.size()-1 << " hops)\n";
        } else {
            cout << "No path from 0 to " << n-1 << "\n";
        }
    }

    return 0;
}
