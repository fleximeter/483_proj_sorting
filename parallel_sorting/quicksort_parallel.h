/* 
File: quicksort_parallel.h

This file is a parallel implementation of Quicksort.
Reference: https://www.open-mpi.org/doc/v3.1/man3/MPI_Get_count.3.php
*/

#ifndef QUICKSORT_PARALLEL
#define QUICKSORT_PARALLEL
#include "../serial_sorting/quicksort.h"
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void arr_scan(double *arr, int length, int p, int my_rank);

/// @brief A parallel implementation of QuickSort.
/// @param arr The array to sort
/// @param first_index The first index of the array to sort
/// @param last_index The last index of the array to sort
/// @param p The number of processes
/// @param my_rank My rank
/// @param comm A MPI communicator
/// @param status A MPI status object
void QuickSortParallel(double *arr, int first_index, int last_index, int p, int my_rank, 
    MPI_Comm *comm, MPI_Status *status) {
    int i;
    int original_last_index = last_index;
    time_t t;
    srand((unsigned)time(&t) * 3 * (my_rank + 1));

    /* If we don't have that many numbers to sort, there is absolutely no point in
    doing a parallel sort, and we will default to a serial sort. We'll say that if we have
    less than 4 * p numbers to sort, we'll just use the serial version. */
    if (last_index - first_index + 1 < 4 * p) {
        if (my_rank == 0) {
            Quicksort(arr, first_index, last_index);
        }
    }

    else {
        /* Determine how many branching levels we have at most */
        int levels = (int)log2(p);
        int upper_limit = (int)pow(2, levels);

        /* Begin the quicksort process in parallel */
        for (i = levels - 1; i >= 0; i--){
            int cpow = (int)pow(2, i);
            
            /* If I'm a sender */
            if (!(my_rank % (int)pow(2, i+1)) && my_rank < upper_limit) {
                int middle_index;
                int right_child = my_rank + cpow;

                /* Partition the array and get the middle index. If our indices are out of whack,
                we just set the middle index to the last index. */
                if (first_index < last_index) {
                    middle_index = Partition(arr, first_index, last_index);
                } else {
                    middle_index = last_index;
                }
                
                /* Determine the array chunk we will be sending to the child process */
                int indices[2] = {middle_index + 1, last_index};
                MPI_Send(indices, 2, MPI_INT, right_child, 0, *comm);
                // printf("I am node %d and I am sending indices %d through %d to node %d\n", my_rank, indices[0], indices[1], right_child);
                MPI_Barrier(*comm);
                // arr_scan(arr, original_last_index + 1, p, my_rank);
                
                /* Send the array chunk to the child process */
                MPI_Send(arr + indices[0], last_index - middle_index, MPI_DOUBLE, right_child, i, *comm);
                last_index = middle_index;
                MPI_Barrier(*comm);
            }

            /* If I'm a receiver */
            else if (!(my_rank % cpow) && my_rank < upper_limit) {
                int parent = my_rank / cpow - 1;

                /* Get the indices we will be receiving from the parent process */
                int indices[2];
                MPI_Recv(indices, 2, MPI_INT, parent, 0, *comm, status);
                first_index = indices[0];
                last_index = indices[1];
                // printf("I am node %d and I am getting indices %d through %d from node %d\n", my_rank, indices[0], indices[1], parent);
                MPI_Barrier(*comm);
                // arr_scan(arr, original_last_index + 1, p, my_rank);

                /* Get the array chunk from the parent process */
                MPI_Recv(arr + first_index, original_last_index - first_index + 1, MPI_DOUBLE, parent, i, *comm, status);
                MPI_Barrier(*comm);
            }

            /* If I need to wait out this turn */
            else {
                MPI_Barrier(*comm);
                MPI_Barrier(*comm);
            }
        }

        /* At this point, I should know what part of the array I am sorting. */
        Quicksort(arr, first_index, last_index);

        /* Now I need to combine the data */
        if (my_rank == 0) {
            double *temp_arr = (double*)malloc((original_last_index - first_index + 1) * sizeof(double));
            int start = last_index + 1;
            int count;
            for (i = 1; i < upper_limit; i++) {
                MPI_Recv(temp_arr, original_last_index - first_index + 1, MPI_DOUBLE, i, 0, *comm, status);
                MPI_Get_count(status, MPI_DOUBLE, &count);
                if (count) {
                    int j;
                    for (j = 0; j < count; j++)
                        arr[j + start] = temp_arr[j];
                }
                //printf("I am process 0 and I got %d from process %d, starting at index %d\n", count, i, start);
                start += count;
            }
            free(temp_arr);
        } else if (my_rank < upper_limit) {
            if (first_index <= last_index) {
                //printf("I am process %d and I am sending indices %d through %d (%d total) to process 0\n", my_rank, first_index, last_index, last_index - first_index + 1);
                MPI_Send(arr + first_index, last_index - first_index + 1, MPI_DOUBLE, 0, 0, *comm);
            } else {
                //printf("I am process %d and I am sending indices %d through %d (%d total) to process 0\n", my_rank, first_index, last_index, 0);
                MPI_Send(arr + first_index, 0, MPI_DOUBLE, 0, 0, *comm);
            }
        }
    }

    MPI_Barrier(*comm);
}

void arr_scan(double *arr, int length, int p, int my_rank) {
    for (int i = 0; i < length; i++){
        if (arr[i] == 0) {
            printf("I am process %d and index %d of my array is 0\n", my_rank, i);
        }
    }
}

#endif