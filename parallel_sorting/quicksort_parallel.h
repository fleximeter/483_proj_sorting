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
#include <stdlib.h>

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
                MPI_Barrier(*comm);
                
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
                MPI_Barrier(*comm);

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
                start += count;
            }
            free(temp_arr);
        } else if (my_rank < upper_limit) {
            if (first_index <= last_index) {
                MPI_Send(arr + first_index, last_index - first_index + 1, MPI_DOUBLE, 0, 0, *comm);
            } else {
                MPI_Send(arr + first_index, 0, MPI_DOUBLE, 0, 0, *comm);
            }
        }
    }

    /* Force all the processes to end at the same time */
    MPI_Barrier(*comm);
}

#endif
