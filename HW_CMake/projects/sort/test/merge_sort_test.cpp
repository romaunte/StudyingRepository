#include <gtest/gtest.h>
#include "merge_sort.hpp"
#include "merge.hpp"

TEST(MergeSortTest, SortsArrayCorrectly) {
    int array[] = {38, 27, 43, 3, 9, 82, 10};
    const int size = sizeof(array) / sizeof(array[0]);

    mergeSort(array, 0, size - 1);

    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 9);
    EXPECT_EQ(array[2], 10);
    EXPECT_EQ(array[3], 27);
    EXPECT_EQ(array[4], 38);
    EXPECT_EQ(array[5], 43);
    EXPECT_EQ(array[6], 82);
}

TEST(MergeSortTest, SortsEmptyArray) {
    int array[] = {};
    mergeSort(array, 0, 0);
    SUCCEED(); 
}

TEST(MergeSortTest, SortsSingleElementArray) {
    int array[] = {42};
    mergeSort(array, 0, 0);
    EXPECT_EQ(array[0], 42);
}

TEST(MergeSortTest, SortsArrayWithNegativeNumbers) {
    int array[] = {-1, -5, 3, 0, -2, 8};
    const int size = sizeof(array) / sizeof(array[0]);
    mergeSort(array, 0, size - 1);
    EXPECT_EQ(array[0], -5);
    EXPECT_EQ(array[1], -2);
    EXPECT_EQ(array[2], -1);
    EXPECT_EQ(array[3], 0);
    EXPECT_EQ(array[4], 3);
    EXPECT_EQ(array[5], 8);
}