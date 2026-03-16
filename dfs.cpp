#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <chrono>
#include <random>
#include <iomanip>
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

void dfsRecHelper(int v, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[v] = true;
    for (int nb : adj[v])
        if (!visited[nb])
            dfsRecHelper(nb, adj, visited);
}

void dfsRecursive(const vector<vector<int>>& adj, int start) {
    vector<bool> visited(adj.size(), false);
    dfsRecHelper(start, adj, visited);
}


void dfsIterativeOriginal(const vector<vector<int>>& adj, int start) {
    unordered_set<int> visited;
    stack<int> stk;
    stk.push(start);

    while (!stk.empty()) {
        int v = stk.top(); stk.pop();
        if (visited.count(v)) continue;
        visited.insert(v);
        for (int nb : adj[v])
            stk.push(nb);
    }
}

void dfsIterativeOpt(const vector<vector<int>>& adj, int start) {
    vector<bool> visited(adj.size(), false);
    stack<int> stk;
    stk.push(start);

    while (!stk.empty()) {
        int v = stk.top(); stk.pop();
        if (visited[v]) continue;
        visited[v] = true;
        for (int nb : adj[v])
            stk.push(nb);
    }
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
    cout << "  DFS Benchmark  (random graph, p=0.15)\n";
    cout << "==============================================\n";
    cout << left  << setw(8)  << "n"
         << right << setw(16) << "Recursive"
         << setw(16) << "Iter (orig)"
         << setw(16) << "Iter (opt)"
         << setw(10) << "Speedup"
         << "\n" << string(66, '-') << "\n";

    for (int n : {500, 1000, 2000, 3500, 5000}) {
        auto g = makeGraph(n, 0.15, rng);
        double tr = benchMs(dfsRecursive,         g);
        double to = benchMs(dfsIterativeOriginal, g);
        double tp = benchMs(dfsIterativeOpt,      g);
        cout << left  << setw(8) << n
             << right << setw(13) << fixed << setprecision(3) << tr << " ms"
             <<          setw(13) << fixed << setprecision(3) << to << " ms"
             <<          setw(13) << fixed << setprecision(3) << tp << " ms"
             <<          setw(8)  << fixed << setprecision(2) << to / tp << "x"
             << "\n";
    }

    return 0;
}
