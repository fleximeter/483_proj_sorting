/*
File: bucket_sort.h
Author: Jeff Martin
Date: 4/14/23

This file implements bucket sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 8

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef BUCKET_SORT_SERIAL
#define BUCKET_SORT_SERIAL
#include "../data_structures/vector.h"
#include <stdlib.h>

void BucketSort(double *arr, int length);
static void _insertionSortAux(double *arr, int length);

/// @brief Implements BucketSort as described on p. 201 of CLRS edition 3. We assume that
/// all the numbers are from 0 to 1 in a uniform random distribution. They do not need
/// to be in a uniform random distribution for the sort to work, but it would be less 
/// efficient otherwise.
/// @param arr An array to sort
/// @param length The length of the array
void BucketSort(double *arr, int length) {
    vector_d *buckets = (vector_d*)malloc(length * sizeof(vector_d));
    int i, j;
    int k = 0;

    for (i = 0; i < length; i++)
        buckets[i] = VectorDConstruct();
    
    for (i = 0; i < length; i++) {
        int bucket_id = (int)(length * arr[i]);
        VectorDPushBack(&(buckets[bucket_id]), arr[i]);
    }

    for (i = 0; i < length; i++)
        _insertionSortAux(buckets[i].data, buckets[i].size);

    for (i = 0; i < length; i++) {
        for (j = 0; j < buckets[i].size; j++) {
            arr[k] = buckets[i].data[j];
            k++;
        }
        VectorDDestruct(&(buckets[i]));
    }
    free(buckets);
}

/// @brief Implements InsertionSort as described on p. 18 of CLRS edition 3. This sorting
/// mechanism is O(n^2).
/// @param arr The array to be sorted
/// @param length The length of the array
static void _insertionSortAux(double *arr, int length) {
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
