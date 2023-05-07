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
#include "parallel_sorting/insertion_sort_parallel.h"
#include "parallel_sorting/bucket_sort_parallel.h"
#include "parallel_sorting/merge_sort_parallel.h"
#include "parallel_sorting/quicksort_parallel.h"
#include "serial_sorting/bubble_sort.h"
#include "serial_sorting/insertion_sort.h"
#include "serial_sorting/bucket_sort.h"
#include "serial_sorting/merge_sort.h"
#include "serial_sorting/quicksort.h"
#include "tests/tests.h"
#include "tests/timer.h"
#include "data_structures/array_helpers.h"
#include <stdio.h>
#include <time.h>
#include <mpi.h>

int main(void) {
    int p, my_rank, arr_len;
    MPI_Init(NULL, NULL);
    MPI_Comm comm;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    comm = MPI_COMM_WORLD;

    int num_tests = 10;
    int num_arrays = 3;
    double s_time_start, s_time_end, s_time_total;
    double p_time_start, p_time_end, p_time_total;
    double serial_total, parallel_total, speedup_total, efficiency_total;
    double serial_time_arr[num_arrays];
    double parallel_time_arr[num_arrays];
    double speedup_arr[num_arrays];
    double efficency_arr[num_arrays];
    /* Change sort_num to chose which sort to use 1. Bubble 2. Insertion 3. Bucket 4. Merge 5. Quick*/
    int sort_num = 2; 
    

    for(int x = 3; x < num_arrays + 3; x++) {
        arr_len = pow(10, x);
        double *s_arr = (double*)malloc(arr_len * sizeof(double));
        double *p_arr = (double*)malloc(arr_len * sizeof(double));

        /* Prints the array size being used and resets the totals */
        if (my_rank == 0){
            printf("\n\nArray Size: %d\n", arr_len);
            serial_total = 0;
            parallel_total = 0;
            speedup_total = 0;
            efficiency_total = 0;
        }

        for(int i = 0; i < num_tests; i++){
            if (my_rank == 0) {
                printf("Test #%d\n", i);
                time_t t;
                fill_array_rand(s_arr, arr_len, (unsigned)time(&t));
                copy_array(s_arr, p_arr, arr_len);
                
                /* Times serial sort */
                GET_TIME(s_time_start);
                switch (sort_num){
                    case 1:
                        BubbleSort(s_arr, arr_len);
                        break;
                    case 2:
                        InsertionSortD(s_arr, arr_len);
                        break;
                    case 3:
                        BucketSort(s_arr, arr_len);
                        break;
                    case 4:
                        MergeSort(s_arr, 0, arr_len - 1);
                        break;
                    case 5:
                        QuickSort(s_arr, 0, arr_len - 1);
                        break;
                }
                GET_TIME(s_time_end);

                /* Calculates the time taken for serial sorting*/
                s_time_total = s_time_end - s_time_start;
                printf("The total time taken for serial sorting was %lf\n", s_time_total);
                serial_total = serial_total + s_time_total;
                MPI_Barrier(comm);

                GET_TIME(p_time_start);
            } else {
                MPI_Barrier(comm);
            }


            /* Broadcast the randomly generated array to all ranks */
            MPI_Bcast(p_arr, arr_len, MPI_DOUBLE, 0, comm);

            /* Calls the chosen parallel sort */
            switch (sort_num){
                case 1:
                    BubbleSortParallel(p_arr, arr_len, p, my_rank, &comm, &status);
                    break;
                case 2:
                    InsertionSortParallel(p_arr, arr_len, p, my_rank, &comm, &status);
                    break;
                case 3:
                    BucketSortParallel(p_arr, arr_len, 0, 1, p, my_rank, &comm, &status);
                    break;
                case 4:
                    MergeSortParallel(p_arr, 0, arr_len - 1, p, my_rank, &comm, &status);
                    break;
                case 5:
                    QuickSortParallel(p_arr, 0, arr_len - 1, p, my_rank, &comm, &status);
                    break;
            }

            if (my_rank == 0) {
                /* Calculates the time taken for parallel sorting*/
                GET_TIME(p_time_end);
                MPI_Barrier(comm);
                p_time_total = p_time_end - p_time_start;
                printf("The total time taken for parallel sorting was %lf\n", p_time_total);
                parallel_total = parallel_total + p_time_total;

                /* Prints if the sorts were correct, if not says which one was incorrect */
                if (TestSortCorrectness(s_arr, arr_len) && TestSortCorrectness(p_arr, arr_len)){
                    printf("The arrays sorted correctly.\n");
                    /* Calculates Efficiency and Speedup*/
                    printf("Efficiency: %f\n", Efficiency(s_time_total, p_time_total, p));
                    printf("Speedup: %f\n", Speedup(s_time_total, p_time_total));
                    efficiency_total = efficiency_total + Efficiency(s_time_total, p_time_total, p);
                    speedup_total = speedup_total + Speedup(s_time_total, p_time_total);
                } else if (TestSortCorrectness(p_arr, arr_len)) {
                    printf("The serial sort did not sort correctly.\n");
                } else if (TestSortCorrectness(s_arr, arr_len)) {
                    printf("The parallel sort did not sort correctly.\n");
                } else {
                    printf("Both serial and parallel sorts did not sort correctly.\n");
                } 
            } else {
                MPI_Barrier(comm);
            }
        }

        /*Finds the average efficiency and speedup*/
        if (my_rank == 0){
            printf("Average Serial Time over %d runs: %f\n", num_tests, (serial_total / (double)num_tests));
            serial_time_arr[x - 3] = (serial_total / (double)num_tests);
            printf("Average Parallel Time over %d runs: %f\n", num_tests, (parallel_total / (double)num_tests));
            parallel_time_arr[x - 3] = (parallel_total / (double)num_tests);
            printf("Average Efficiency over %d runs: %f\n", num_tests, (efficiency_total / (double)num_tests));
            efficency_arr[x - 3] = (efficiency_total / (double)num_tests);
            printf("Average Speedup over %d runs: %f\n", num_tests, (speedup_total / (double)num_tests));
            speedup_arr[x - 3] = (speedup_total / (double)num_tests);
        }

        free(s_arr);
        free(p_arr);
    }

    /* Prints out the averages for each array size */
    if(my_rank == 0){
        switch (sort_num){
            case 1:
                printf("\n\nBubble Sort\n");
                break;
            case 2:
                printf("\n\nInsertion Sort\n");
                break;
            case 3:
                printf("\n\nBucket Sort\n");
                break;
            case 4:
                printf("\n\nMerge Sort\n");
                break;
            case 5:
                printf("\n\nQuick Sort\n");
                break;
        }
        printf("Number of Threads: %d\n", p);
        for(int x = 0; x < num_arrays; x++)
            printf("Serial average time for size %d array: %f\n", (int)pow(10, (x + 3)), serial_time_arr[x]);
        for(int x = 0; x < num_arrays; x++)
            printf("Parallel average time for size %d array: %f\n", (int)pow(10, (x + 3)), parallel_time_arr[x]);
        for(int x = 0; x < num_arrays; x++)
            printf("Efficency average for size %d array: %f\n", (int)pow(10, (x + 3)), efficency_arr[x]);
        for(int x = 0; x < num_arrays; x++)
            printf("Speedup average for size %d array: %f\n", (int)pow(10, (x + 3)), speedup_arr[x]);
    }

    MPI_Finalize();
    return 0;
}
