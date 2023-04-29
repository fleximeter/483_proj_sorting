/*
File: merge_sort.h
Author: Jeff Martin
Date: 4/14/23

This file implements merge sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 2

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef MERGE_SORT_SERIAL
#define MERGE_SORT_SERIAL
#include <stdlib.h>

void Merge(int *arr, int first_index, int middle_index, int last_index);
void MergeSort(int *arr, int index1, int index2);

/// @brief Implements Merge as described on p. 31 of CLRS edition 3. We assume that prior
/// to calling this function, that the array is sorted between indices first_index and
/// middle_index, and between middle_index + 1 and last_index. We merge the two sub
/// arrays of arr.
/// @param arr The array to be merged
/// @param first_index The first index
/// @param middle_index The middle index
/// @param last_index The last index
void Merge(int *arr, int first_index, int middle_index, int last_index) {
    int i, j, k;

    /* Make copies of the left and right sub arrays */
    int len1 = middle_index - first_index + 1;
    int len2 = last_index - middle_index;
    int *left = (int*)malloc(len1 * sizeof(int));
    int *right = (int*)malloc(len2 * sizeof(int));
    for (i = 0; i < len1; i++)
        left[i] = arr[first_index + i];
    for (i = 0; i < len2; i++)
        right[i] = arr[middle_index + i + 1];

    /* Merge the two sorted sub arrays */
    i = 0;
    j = 0;
    for (k = first_index; k <= last_index; k++) {
        if (i < len1 && j < len2) {
            if (left[i] <= right[j]) {
                arr[k] = left[i];
                i++;
            } else {
                arr[k] = right[j];
                j++;
            }
        } else if (i < len1) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
    }

    free(left);
    free(right);
}

/// @brief Performs MergeSort on an array, as described on p. 34 of CLRS edition 3.
/// This sorting method is O(n log n).
/// @param arr An array
/// @param index1 The first index of the sub array to be sorted
/// @param index2 The last index of the sub array to be sorted
void MergeSort(int *arr, int index1, int index2) {
    if (index1 < index2) {
        int index_mid = (index1 + index2) / 2;
        MergeSort(arr, index1, index_mid);
        MergeSort(arr, index_mid + 1, index2);
        Merge(arr, index1, index_mid, index2);
    }
}

#endif
