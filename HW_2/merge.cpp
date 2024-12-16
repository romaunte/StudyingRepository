#include "merge.hpp"
#include <iostream>

void Merge(int array[], int left, int middle, int right) {

    const int leftSize = middle - left + 1;
    const int rightSize = right - middle;

    int leftArray[leftSize];
    int rightArray[rightSize];

    for (int i = 0; i < leftSize; ++i) {
        leftArray[i] = array[left + i];
    }

    for (int j = 0; j < rightSize; ++j) {
        rightArray[j] = array[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            ++i;
        } else {
            array[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < leftSize) {
        array[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < rightSize) {
        array[k] = rightArray[j];
        ++j;
        ++k;
    }
}
