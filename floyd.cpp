#include <vector>
#include <iostream>
using namespace std;

const int INF = 1e9;  // use 1e9, not INT_MAX, to avoid overflow on addition

vector<vector<int>> floydWarshall(const vector<vector<int>>& weightMatrix) {
    int n = (int)weightMatrix.size();
    vector<vector<int>> dist = weightMatrix;  // copy input

    for (int k = 0; k < n; k++)           // intermediate vertex
        for (int i = 0; i < n; i++)        // source
            for (int j = 0; j < n; j++)    // destination
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

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

    auto dist = floydWarshall(w);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << (dist[i][j] == INF ? -1 : dist[i][j]) << "\t";
        cout << "\n";
    }
}