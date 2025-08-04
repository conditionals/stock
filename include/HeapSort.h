#pragma once
#include "../include/StockAnalysisCLI.h"
#include "../include/StockData.h"
#include <vector>
#include <algorithm> //used for "reverse" function

using namespace std;

void heapify(StockInfo **arr, int n, int i) {
    //recursively builds onto heap
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l]->avg_annual_return > arr[largest]->avg_annual_return)
        largest = l;

    if (r < n && arr[r]->avg_annual_return > arr[largest]->avg_annual_return)
        largest = r;

    if (largest != i) {
        StockInfo *temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest); // recursive call
    }
}

vector<StockInfo *> StockAnalysisCLI::heap_sort_by_avg_return() const {
    StockInfo **arr = hash_lookup.get_all_array();
    int size = hash_lookup.size();


    for (int i = size / 2 - 1; i >= 0; --i) // Builds max-heap using heapify function
        heapify(arr, size, i);


    for (int i = size - 1; i > 0; --i) {
        // puts elements into array
        StockInfo *temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }


    // Convert sorted array into a vector in ascending order, lowest to highest
    vector<StockInfo *> sorted_list;
    for (int i = 0; i < size; ++i) {
        sorted_list.push_back(arr[i]);
    }

    //reverse(sorted_list.begin(), sorted_list.end()); //if wanting descending order then just uncomment


    delete[] arr; // clean up
    return sorted_list;
}
