/*
File: counting_sort.h

This file implements counting sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 8
*/

#ifndef COUNTING_SORT_SERIAL
#define COUNTING_SORT_SERIAL
#include <stdlib.h>

/// @brief Implements CountingSort as described on p. 195 of CLRS edition 3. This sorting
/// mechanism is O(n) if max_val is O(n).
/// @param arr1 The array to be sorted
/// @param arr2 The array to hold the sorted output (must be the same size as arr1)
/// @param length The length of the arrays 1 and 2
/// @param max_val The maximum value in arr1
void CountingSort(int *arr1, int *arr2, int length, int max_val) {
    int i;
    int *aux = (int*)malloc((max_val + 1) * sizeof(int));
    for (i = 0; i < max_val + 1; i++)
        aux[i] = 0;
    for (i = 0; i < length; i++)
        aux[arr1[i]]++;
    for (i = 1; i < max_val + 1; i++)
        aux[i] += aux[i - 1];
    for (i = length - 1; i >= 0; i--) {
        arr2[aux[arr1[i]] - 1] = arr1[i];
        aux[arr1[i]]--;
    }
    free(aux);
}

#endif
