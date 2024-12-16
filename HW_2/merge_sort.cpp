#include "merge_sort.hpp"
#include "merge.hpp"

void mergeSort(int array[], int left, int right) {
    
    if (left < right) {
        const int middle = (left + right) / 2;
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        Merge(array, left, middle, right);
    }
}