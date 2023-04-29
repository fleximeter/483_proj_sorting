/*
File: heapsort.h
Author: Jeff Martin
Date: 4/29/23

This file implements heapsort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 6
*/

#ifndef HEAPSORT_SERIAL
#define HEAPSORT_SERIAL
#include <stdio.h>

static void BuildMaxHeap(double *arr, int length);
void HeapSort(double *arr, int length);
static void MaxHeapify(double *arr, int length, int heap_size, int i);

/// @brief Makes a max heap from an array that is not a heap
/// @param arr An array
/// @param length The length of the array
/// This is an implementation of the algorithm at page 157 of CLRS chapter 6.
static void BuildMaxHeap(double *arr, int length) {
    int i;
    for (i = length >> 1; i >= 0; i--)
        MaxHeapify(arr, length, length, i);
}

/// @brief Sorts an array with HeapSort
/// @param arr An array
/// @param length The length of the array
/// This is an implementation of the algorithm at page 160 of CLRS chapter 6.
void HeapSort(double *arr, int length) {
    int i;
    int heap_size = length;
    double temp;
    BuildMaxHeap(arr, length);
    for (i = length - 1; i > 0; i--) {
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heap_size--;
        MaxHeapify(arr, length, heap_size, 0);
    }
}

/// @brief Makes a heap a max heap if it has been modified and is no longer a max heap
/// @param arr An array
/// @param length The length of the array
/// @param i The index at which to verify that the heap is a max heap
/// This is an implementation of the algorithm at page 154 of CLRS chapter 6. It was
/// modified to make it iterative rather than recursive.
static void MaxHeapify(double *arr, int length, int heap_size, int i) {
    int l;
    int r;
    int largest;
    int done = 0;

    while (!done) {
        r = ((i + 1) << 1);
        l = r - 1;
        largest = i;
        
        if (l < length) {
            if (l < heap_size && arr[l] > arr[i]) {
                largest = l;
            } else {
                largest = i;
            }
        }

        if (r < length) {
            if (r < heap_size && arr[r] > arr[largest]) {
                largest = r;
            }
        }

        if (largest != i) {
            double temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            i = largest;
        } else {
            done = 1;
        }
    }
}



#endif