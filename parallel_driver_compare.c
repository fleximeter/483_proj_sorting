/*
File: parallel_driver_compare.c
Authors: Jeff Martin, Justin Hendershot
Date: 4/14/23

This file is set up for running parallel MPI sorts, and comparing their results.
Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.

This is based on a previous version of driver.c in Jeff Martin's Homework 4,
as well as Jeff Martin's final Homework 4 driver.c.
*/

#include "parallel_sorting/quicksort_parallel.h"
#include "parallel_sorting/merge_sort_parallel.h"
#include "tests/tests.h"
#include "tests/timer.h"
#include "data_structures/array_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define ARR_LEN 1000000
int main(void) {
    int p, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm comm;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    comm = MPI_COMM_WORLD;

    double *arr1 = (double*)malloc(ARR_LEN * sizeof(double));
    double *arr2 = (double*)malloc(ARR_LEN * sizeof(double));

    double time_start, time_end, time_total;
    
    /* Initialize the array on rank 0 with random numbers */
    if (my_rank == 0) {
        time_t t;
        fill_array_rand(arr1, ARR_LEN, (unsigned)time(&t));
        copy_array(arr1, arr2, ARR_LEN);
        // printf("Before sorting:\n");
        // print_array(arr, ARR_LEN);
    }
    
    /* Sort 1 */
    MPI_Barrier(comm);
    GET_TIME(time_start);
    MPI_Bcast(arr1, ARR_LEN, MPI_DOUBLE, 0, comm);
    QuickSortParallel(arr1, 0, ARR_LEN - 1, p, my_rank, &comm, &status);
    GET_TIME(time_end);
    if (my_rank == 0) {
        time_total = time_end - time_start;
        printf("The total time taken for sort 1 was %lf\n\n", time_total);
    }
    MPI_Barrier(comm);

    /* Sort 2 */
    GET_TIME(time_start);
    MPI_Bcast(arr2, ARR_LEN, MPI_DOUBLE, 0, comm);
    MergeSortParallel(arr2, 0, ARR_LEN - 1, p, my_rank, &comm, &status);
    GET_TIME(time_end);
    if (my_rank == 0) {
        time_total = time_end - time_start;
        printf("The total time taken for sort 2 was %lf\n\n", time_total);
    }
    MPI_Barrier(comm);

    /* Compare sorts and verify that sorts worked correctly */
    if (my_rank == 0) {
        // printf("After sorting:\n");
        // print_array(arr, ARR_LEN);
        // printf("After sorting:\n");
        // print_array(arr2, ARR_LEN);
        if (TestSortCorrectness(arr1, ARR_LEN) && CompareSortedArrays(arr1, arr2, ARR_LEN)) {
           printf("The arrays sorted correctly.\n");
        } else {
           printf("The arrays did not sort correctly.\n");
        }

    }

    free(arr1);
    free(arr2);

    MPI_Finalize();
    return 0;
}
