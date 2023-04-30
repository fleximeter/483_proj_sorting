/*
File: parallel_driver.c
Author: Jeff Martin
Date: 4/14/23

This file is set up for running parallel MPI sorts.
Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.

This is based on a previous version of driver.c in Jeff Martin's Homework 4,
as well as Jeff Martin's final Homework 4 driver.c.
*/

#include "parallel_sorting/merge_sort_parallel.h"
#include "tests/tests.h"
#include "tests/timer.h"
#include "data_structures/array_helpers.h"
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define ARR_LEN 10000

int main(void) {
    int p, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm comm;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    comm = MPI_COMM_WORLD;

    double arr[ARR_LEN];
    double time_start, time_end, time_total;
    
    /* Initialize the array on rank 0 with random numbers */
    if (my_rank == 0) {
        time_t t;
        fill_array_rand(arr, ARR_LEN, (unsigned)time(&t));
        // printf("Before sorting:\n");
        // print_array(arr, ARR_LEN);
        GET_TIME(time_start);
    }

    /* Broadcast the randomly generated array to all ranks */
    MPI_Bcast(arr, ARR_LEN, MPI_DOUBLE, 0, comm);

    MergeSortParallel(arr, 0, ARR_LEN - 1, p, my_rank, &comm, &status);

    if (my_rank == 0) {
        GET_TIME(time_end);
        time_total = time_end - time_start;
        printf("The total time taken for sorting was %lf\n\n", time_total);
        // printf("After sorting:\n");
        // print_array(arr, ARR_LEN);

        if (TestSortCorrectness(arr, ARR_LEN)) {
            printf("The array sorted correctly.\n");
        } else {
            printf("The array did not sort correctly.\n");
        }
    }

    MPI_Finalize();
    return 0;
}
