/*
File: quicksort.h

This file implements quicksort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 7
*/

#ifndef QUICKSORT_SERIAL
#define QUICKSORT_SERIAL
#include <math.h>

/// @brief Implements Quicksort as described on p. 171 of CLRS edition 3. This sorting
/// mechanism is O(n^2).
/// @param arr The array to be sorted
/// @param first_index The first index of the sub array
/// @param first_index The last index of the sub array
void Quicksort(double *arr, int first_index, int last_index) {
    if (first_index < last_index) {
        int q = Partition(arr, first_index, last_index);
        Quicksort(arr, first_index, q - 1);
        Quicksort(arr, q + 1, last_index);
    }
}

/// @brief Implements Quicksort Partition as described on p. 171 of CLRS edition 3.
/// @param arr The array to be sorted
/// @param first_index The first index of the sub array
/// @param first_index The last index of the sub array
int Partition(double *arr, int first_index, int last_index) {
    int j;
    double temp;
    double pivot = arr[last_index];
    int i = first_index - 1;
    for (j = first_index; j < last_index; j++) {
        if (arr[j] <= pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    temp = arr[i + 1];
    arr[i + 1] = arr[last_index];
    arr[last_index] = temp;
    return i + 1;
}

#endif
