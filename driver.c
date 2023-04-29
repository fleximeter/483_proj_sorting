/*
File: driver.c
Author: Jeff Martin
Date: 4/14/23

This file tests a number of different sorting algorithms.
Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.
*/

#include "bucket_sort.h"
#include "insertion_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ARR_LEN 128

int main(void) {
    time_t t;
    double arr[ARR_LEN];
    double arr2[ARR_LEN];
    int i;
    srand((unsigned) time(&t));
    
    for (i = 0; i < ARR_LEN; i++) {
        arr[i] = (rand() % 1000) * pow(10, -3);
        arr2[i] = arr[i];
        printf("%lf ", arr[i]);
    }
    printf("\n");
    
    BucketSort(arr, ARR_LEN);
    InsertionSortD(arr2, ARR_LEN);
    
    for (i = 0; i < ARR_LEN; i++) {
        printf("%lf %lf | ", arr[i], arr2[i]);
    }
    printf("\n");
    return 0;
}
