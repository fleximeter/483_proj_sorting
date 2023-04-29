/*
File: driver.c
Author: Jeff Martin
Date: 4/14/23

This file tests a number of different sorting algorithms.
Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.

This is based on a previous version of driver.c in Jeff Martin's Homework 4.
*/

#include "serial_sorting/bucket_sort.h"
#include "tests/tests.h"
#include "tests/timer.h"
#include "data_structures/array_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ARR_LEN 128

int main(void) {
    time_t t;
    double arr[ARR_LEN];
    double arr2[ARR_LEN];
    
    fill_array_rand(arr, ARR_LEN, (unsigned)time(&t));
    copy_array(arr, arr2, ARR_LEN);
    
    printf("Before sorting:\n");
    print_array(arr, ARR_LEN);

    BucketSort(arr, ARR_LEN);

    printf("After sorting:\n");
    print_array(arr, ARR_LEN);

    if (TestSortCorrectness(arr, ARR_LEN)) {
        printf("The array sorted correctly.\n");
    } else {
        printf("The array did not sort correctly.\n");
    }

    return 0;
}
