/*
File: tests.h
Author: Jeff Martin
Date: 4/29/23
This file contains tests to prove the correctness of a sorting algorithm.
*/

#ifndef TESTS_H
#define TESTS_H

int CompareSortedArrays(double *arr1, double *arr2, int length);
double Efficiency(double serial_time, double parallel_time, int p);
double Speedup(double serial_time, double parallel_time);
int TestSortCorrectness(double *arr, int length);

/// @brief Tests if two sorted arrays were sorted the same; i.e. each element
/// in arr1 is the same as its corresponding element in arr2.
/// @param arr1 An array
/// @param arr2 Another array
/// @param length The lengths of both arrays
/// @return 1 if the two arrays are sorted the same; 0 otherwise
int CompareSortedArrays(double *arr1, double *arr2, int length) {
    int i;
    for (i = 0; i < length; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }
    return 1;
}

/// @brief Calculates efficiency
/// @param serial_time The time taken by a good serial implementation
/// @param length The time taken by a parallel implementation
/// @param p The number of processors used
/// @return The efficiency
double Efficiency(double serial_time, double parallel_time, int p) {
    return serial_time / (p * parallel_time);
}

/// @brief Calculates speedup
/// @param serial_time The time taken by a good serial implementation
/// @param length The time taken by a parallel implementation
/// @return The speedup
double Speedup(double serial_time, double parallel_time) {
    return serial_time / parallel_time;
}

/// @brief Tests if a sort finished correctly; i.e. the numbers in the array
/// are sorted. We assume that a correct sort is in descending order, so
/// item 0 <= item 1 <= item 2 <= ... <= item n.
/// @param arr An array to test
/// @param length The length of the array
/// @return 1 if the array is correctly sorted; 0 otherwise
int TestSortCorrectness(double *arr, int length) {
    int i;
    for (i = 1; i < length; i++) {
        if (arr[i] < arr[i-1])
            return 0;
    }
    return 1;
}

#endif