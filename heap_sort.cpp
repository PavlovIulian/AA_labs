#include <iostream>
#include <vector>

using namespace std;

void heapify(vector<double>& arr, int n, int i) {

    int largest = i;        
    int l = 2 * i + 1;      
    int r = 2 * i + 2;      

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest]) 
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<double>& arr) {
    int n = arr.size();

    for (int j = n / 2 - 1; j >= 0; j--) {
        heapify(arr, n, j);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    int size;
    cin >> size;

    vector<double> arr(size);
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    heapSort(arr);

    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
}