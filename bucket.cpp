#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<float>& bucket) {
    for (int i = 1; i < bucket.size(); ++i) {
        float key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

void bucketSort(vector<float>& arr) {
    int n = arr.size();
    if (n == 0) return;

    vector<vector<float>> b(n);

    for (int i = 0; i < n; i++) {
        int bi = n * arr[i];       
        if (bi == n) bi = n - 1;     
        b[bi].push_back(arr[i]);
    }

    for (int i = 0; i < n; i++)
        insertionSort(b[i]);

    int index = 0;
    for (int i = 0; i < n; i++)
        for (float x : b[i])
            arr[index++] = x;
}

int main() {
    int size;
    cin >> size;

    vector<float> arr(size);
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    bucketSort(arr);

    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
}