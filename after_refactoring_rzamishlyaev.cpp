#include <ctime>
#include <iostream>
#include <random>

static const int ARRAY_SIZE = 20;

void generateRandomArr(int array[], const int size, std::mt19937& mt);
void printArray(const int array[], const int size);
void reverseArray(int array[], const int size);

int main() {
    int array[ARRAY_SIZE];

    std::random_device rd;
    std::mt19937 mt(rd());
    generateRandomArr(array, ARRAY_SIZE, mt);
    
    std::cout << "Исходный массив: ";
    printArray(array, ARRAY_SIZE);

    reverseArray(array, ARRAY_SIZE);

    std::cout << "Массив после переворота: ";
    printArray(array, ARRAY_SIZE);

    return 0;
}

void generateRandomArr(int array[], const int size, std::mt19937& mt) {
    for (int i = 0; i < size; ++i) {
        array[i] = mt() % 20;
    }
}

void printArray(const int array[], const int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}

void reverseArray(int array[], const int size) {
    for (int i = 0; i < ARRAY_SIZE / 2; ++i) {
        int temp = array[i];
        array[i] = array[size - 1 - i];
        array[size - 1 - i] = temp;
    }
}