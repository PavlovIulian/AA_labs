#include <vector>
#include <iostream>
using namespace std;

const int INF = 1e9;

vector<vector<int>> floydWarshallOpt(const vector<vector<int>>& weightMatrix) {
    int n = (int)weightMatrix.size();
    vector<vector<int>> dist = weightMatrix;

    for (int k = 0; k < n; k++) {
        const auto& distK = dist[k];        // cache row k — avoids repeated indexing
        for (int i = 0; i < n; i++) {
            if (dist[i][k] == INF) continue; // early exit: i→k unreachable, skip entire row
            int dik = dist[i][k];            // hoist invariant out of j-loop
            auto& distI = dist[i];           // cache row i
            for (int j = 0; j < n; j++) {
                if (distK[j] != INF) {       // only relax reachable pairs
                    int nd = dik + distK[j];
                    if (nd < distI[j])
                        distI[j] = nd;
                }
            }
        }
    }
    return dist;
}

int main() {
    int n = 4;
    vector<vector<int>> w(n, vector<int>(n, INF));
    for (int i = 0; i < n; i++) w[i][i] = 0;
    w[0][1] = 3; w[0][3] = 7;
    w[1][0] = 8; w[1][2] = 2;
    w[2][0] = 5; w[2][3] = 1;
    w[3][0] = 2;

    auto dist = floydWarshallOpt(w);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << (dist[i][j] == INF ? -1 : dist[i][j]) << "\t";
        cout << "\n";
    }
}