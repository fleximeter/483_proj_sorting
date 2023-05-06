/*
File: svp_driver.c
Authors: Jeff Martin, Justin Hendershot
Date: 4/14/23

This file is set up for running parallel MPI sorts.
Consulted https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
for random seeding and use.

This is based on a previous version of driver.c in Jeff Martin's Homework 4,
as well as Jeff Martin's final Homework 4 driver.c.
*/

#include "parallel_sorting/bubble_sort_parallel.h"
#include "serial_sorting/bubble_sort.h"
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

    double s_arr[ARR_LEN];
    double p_arr[ARR_LEN];
    double s_time_start, s_time_end, s_time_total;
    double p_time_start, p_time_end, p_time_total;
    double speedup_total, efficiency_total;
    int num_tests = 10;

    for(int i = 0; i < num_tests; i++){
        if (my_rank == 0) {
            printf("Test #%d\n", i);
            time_t t;
            fill_array_rand(s_arr, ARR_LEN, (unsigned)time(&t));
            copy_array(s_arr, p_arr, ARR_LEN);
            
            /* Times serial sort */
            GET_TIME(s_time_start);
            BubbleSort(s_arr, ARR_LEN);
            GET_TIME(s_time_end);

            /* Calculates the time taken for parallel sorting*/
            s_time_total = s_time_end - s_time_start;
            printf("The total time taken for serial sorting was %lf\n", s_time_total);

            GET_TIME(p_time_start);
        }

        /* Broadcast the randomly generated array to all ranks */
        MPI_Bcast(p_arr, ARR_LEN, MPI_DOUBLE, 0, comm);

        BubbleSortParallel(p_arr, ARR_LEN, p, my_rank, &comm, &status);

        if (my_rank == 0) {
            /* Calculates the time taken for parallel sorting*/
            GET_TIME(p_time_end);
            p_time_total = p_time_end - p_time_start;
            printf("The total time taken for parallel sorting was %lf\n", p_time_total);

            if (TestSortCorrectness(s_arr, ARR_LEN) && TestSortCorrectness(p_arr, ARR_LEN)){
                printf("The arrays sorted correctly.\n");
                /* Calculates Efficiency and Speedup*/
                printf("Efficiency: %f\n", Efficiency(s_time_total, p_time_total, p));
                printf("Speedup: %f\n", Speedup(s_time_total, p_time_total));
                efficiency_total += Efficiency(s_time_total, p_time_total, p);
                speedup_total += Speedup(s_time_total, p_time_total);
            } else {
                printf("The arrays did not sort correctly.\n");
            }
        }
    }

    /*Finds the average efficiency and speedup*/
    if (my_rank == 0){
        printf("Average Efficiency over %d runs: %f\n", num_tests, (efficiency_total / (double)num_tests));
        printf("Average Speedup over %d runs: %f\n", num_tests, (speedup_total / (double)num_tests));
    }

    MPI_Finalize();
    return 0;
}
