/*
File: bubble_sort.h

This file implements bubble sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, Rivest, 
and Stein, chapter 2
*/

#ifndef BUBBLE_SORT_SERIAL
#define BUBBLE_SORT_SERIAL

/// @brief Implements BubbleSort as described on p. 40 of CLRS edition 3. This sorting
/// mechanism is O(n^2).
/// @param arr The array to be sorted
/// @param length The length of the array
void BubbleSort(double *arr, int length) {
    int i, j;
    double temp;
    for (i = 0; i < length; i++) {
        /* bubble down from the end */
        for (j = length - 1; j > i; j--) {
            /* swap adjacent elements */
            if (arr[j] < arr[j - 1]) {
                temp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

#endif
