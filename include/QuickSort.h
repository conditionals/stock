//
// Created by Brenden Chouinard on 8/4/25.
//

#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "StockData.h"
#include <vector>
#include <algorithm>

//https://www.geeksforgeeks.org/dsa/quick-sort-algorithm/
//increases from left to right
int partition(std::vector<StockInfo*>& arr, int low, int high) {
  int pivot = arr[high]->avg_annual_return;  //last element is the pivot
  int i = low - 1;        //accounts for smaller elements

  for (int j = low; j < high; j++) {
    if (arr[j]->avg_annual_return <= pivot) {
      i++;
      //moves smaller elements to the left
      std::swap(arr[i], arr[j]);
    }
  }
  //puts pivot in the right location
  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

//sorts array using quicksort
void quicksort(std::vector<StockInfo*>& arr, int low, int high) {
  if (low < high) {
    int pivotIndex = partition(arr, low, high);
    quicksort(arr, low, pivotIndex - 1);      //left side
    quicksort(arr, pivotIndex + 1, high);     //right side
  }
}

//using helper function to call easier
void quickSort(std::vector<StockInfo*>& arr) {
  if (!arr.empty()) {
    quicksort(arr, 0, arr.size() - 1);
  }
}


#endif //QUICKSORT_H
