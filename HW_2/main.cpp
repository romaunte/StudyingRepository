#include <iostream>
#include "merge_sort.hpp"

static const int ARRAY_SIZE = 7;

int main() {
    int array[ARRAY_SIZE] = {38, 27, 43, 3, 9, 82, 10};

    std::cout << "Исходный массив: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    mergeSort(array, 0, ARRAY_SIZE - 1);

    std::cout << "Отсортированный массив: ";
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
