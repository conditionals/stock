//
// Created by Brenden Chouinard on 8/4/25.
//

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "StockData.h"
#include <vector>
#include <algorithm>
using namespace std;

#include <vector>
#include <algorithm>
using namespace std;
//https://www.geeksforgeeks.org/dsa/quick-sort-algorithm/
//increases from left to right
int partition(vector<int>& arr, int low, int high) {
  int pivot = arr[high];  //last element is the pivot
  int i = low - 1;        //accounts for smaller elements

  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      //moves smaller elements to the left
      swap(arr[i], arr[j]);
    }
  }
  //puts pivot in the right location
  swap(arr[i + 1], arr[high]);
  return i + 1;
}

//sorts array using quicksort
void quicksort(vector<int>& arr, int low, int high) {
  if (low < high) {
    int pivotIndex = partition(arr, low, high);
    quicksort(arr, low, pivotIndex - 1);      //left side
    quicksort(arr, pivotIndex + 1, high);     //right side
  }
}

//using helper function to call easier
void quickSort(vector<int>& arr) {
  if (!arr.empty()) {
    quicksort(arr, 0, arr.size() - 1);
  }
}


#endif //QUICKSORT_H
