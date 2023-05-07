/*
File: array_helpers.h
Author: Jeff Martin
Date: 4/29/23

Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.

This is based on a previous version of driver.c in Jeff Martin's Homework 4.
*/

#ifndef ARRAY_HELPERS
#define ARRAY_HELPERS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void copy_array(double *arr_source, double *arr_dest, int length);
void fill_array(double *arr, int length, double min, double max, unsigned seed);
void print_array(double *arr, int length);

/// @brief Copies one array to another
/// @param arr_source The source array
/// @param arr_dest The destination array
/// @param length The length of the arrays
void copy_array(double *arr_source, double *arr_dest, int length) {
    int i;
    for (i = 0; i < length; i++)
        arr_dest[i] = arr_source[i];
}

/// @brief Fills an array with random numbers between 0 and 1.
/// @param arr An array
/// @param length The length of the array
/// @param seed A seed value
void fill_array_rand(double *arr, int length, unsigned seed) {
    int i;
    srand(seed);
    for (i = 0; i < length; i++)
        arr[i] = (rand() % 1000000) * pow(10, -6);
}

/// @brief Prints an array
/// @param arr The array
/// @param length The length of the array
void print_array(double *arr, int length) {
    int i;
    for (i = 0; i < length - 1; i++)
        printf("%lf, ", arr[i]);
    printf("%lf\n", arr[i]);
}

void print_array_ends(double *arr, int length){
    int i;
    for (i = 0; i < 5; i++)
        printf("%lf, ", arr[i]);
    printf("\n");
    for (i = length - 6; i < length - 1; i++)
        printf("%lf, ", arr[i]);
    printf("\n");
}

#endif
