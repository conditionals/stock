#pragma once
#include "../include/StockAnalysisCLI.h"
#include "../include/StockData.h"
#include <vector>
#include <algorithm> //used for "reverse" function


void heapify(std::vector<StockInfo*>& arr, int n, int i) {
    //recursively builds onto heap
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l]->avg_annual_return > arr[largest]->avg_annual_return)
        largest = l;

    if (r < n && arr[r]->avg_annual_return > arr[largest]->avg_annual_return)
        largest = r;

    if (largest != i) {
	std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest); // recursive call
    }
}

void heapSort(std::vector<StockInfo*>& arr) {
    int n = arr.size();

    for(int i = n/2 - 1; i >= 0; i--){
	heapify(arr, n, i);
    }

    for(int i = n - 1; i > 0; i--) {
	std::swap(arr[0], arr[i]);
	heapify(arr,i,0);
    }
}
