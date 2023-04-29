/*
File: radix_sort.h
Author: Jeff Martin
Date: 4/14/23

This file implements radix sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, 
Rivest, and Stein, chapter 8
*/

#ifndef RADIX_SORT_SERIAL
#define RADIX_SORT_SERIAL
#include <stdlib.h>
#include <math.h>

static void _countingSortAux(int *arr1, int *arr2, int length, int max_val, int digit);
static int _getDigit(int val, int digit);
void RadixSort(int *arr, int length, int num_digits);

/// @brief Implements RadixSort as described on p. 198 of CLRS edition 3.
/// @param arr The array to be sorted
/// @param length The length of the array
/// @param num_digits The number of digits to sort
void RadixSort(int *arr, int length, int num_digits) {
    int i, j;
    int *aux = (int*)malloc(length * sizeof(int));
    for (i = 1; i <= num_digits; i++) {
        _countingSortAux(arr, aux, length, 9, i);
        for (j = 0; j < length; j++)
            arr[j] = aux[j];
    }
    free(aux);
}

/// @brief Implements CountingSort as described on p. 195 of CLRS edition 3. This sorting
/// mechanism is O(n) if max_val is O(n).
/// @param arr1 The array to be sorted
/// @param arr2 The array to hold the sorted output (must be the same size as arr1)
/// @param length The length of the arrays 1 and 2
/// @param max_val The maximum value in arr1
/// @param digit The digit on which to sort
static void _countingSortAux(int *arr1, int *arr2, int length, int max_val, int digit) {
    int i;
    int *aux = (int*)malloc((max_val + 1) * sizeof(int));
    for (i = 0; i < max_val + 1; i++)
        aux[i] = 0;
    for (i = 0; i < length; i++)
        aux[_getDigit(arr1[i], digit)]++;
    for (i = 1; i < max_val + 1; i++)
        aux[i] += aux[i - 1];
    for (i = length - 1; i >= 0; i--) {
        arr2[aux[_getDigit(arr1[i], digit)] - 1] = arr1[i];
        aux[_getDigit(arr1[i], digit)]--;
    }
    free(aux);
}

/// @brief Gets a specific digit of a number.
/// For example, if the number is 1923 and the digit is 3, returns 9.
/// @param val The number
/// @param digit The digit
static int _getDigit(int val, int digit) {
    return (val % (int)pow(10, digit) - val % (int)pow(10, digit - 1)) / (int)pow(10, digit - 1);
}

#endif
