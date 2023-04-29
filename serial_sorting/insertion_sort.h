/*
File: insertion_sort.h
Author: Jeff Martin
Date: 4/14/23

This file implements insertion sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 2

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef INSERTION_SORT_SERIAL
#define INSERTION_SORT_SERIAL

/// @brief Implements InsertionSort as described on p. 18 of CLRS edition 3. This sorting
/// mechanism is O(n^2).
/// @param arr The array to be sorted
/// @param length The length of the array
void InsertionSortI(int *arr, int length) {
    int i, j, key;
    for (j = 1; j < length; j++) {
        key = arr[j];
        i = j - 1;
        /* iterate down the array, shuffling numbers right one position until
        we find the place to insert */
        while (i >= 0 && arr[i] > key) {
            arr[i + 1] = arr[i];
            i--;
        }
        /* insert the current key */
        arr[i + 1] = key;
    }
}

/// @brief Implements InsertionSort as described on p. 18 of CLRS edition 3. This sorting
/// mechanism is O(n^2).
/// @param arr The array to be sorted
/// @param length The length of the array
void InsertionSortD(double *arr, int length) {
    int i, j;
    double key;
    for (j = 1; j < length; j++) {
        key = arr[j];
        i = j - 1;
        /* iterate down the array, shuffling numbers right one position until
        we find the place to insert */
        while (i >= 0 && arr[i] > key) {
            arr[i + 1] = arr[i];
            i--;
        }
        /* insert the current key */
        arr[i + 1] = key;
    }
}

#endif
