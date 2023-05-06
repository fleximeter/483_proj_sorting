/*
File: merge_sort_parallel.h

This file implements merge sort.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, Rivest, and Stein
(chapters 2 and 4)

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef MERGE_SORT_PARALLEL
#define MERGE_SORT_PARALLEL
#include <math.h>
#include <mpi.h>
#include "../serial_sorting/merge_sort.h"

void MergeSortParallel(double *arr, int first_index, int last_index, int p, int my_rank,
    MPI_Comm *comm, MPI_Status *status);

/// @brief This function implements MergeSort in parallel. At the end of the sort, process
/// 0 will have the sorted array, and the other processes will have partially sorted arrays,
/// so you will want to retrieve the sorted array from process 0.
/// This implementation of parallel MergeSort skips the first half of the recursion graph
/// by simply giving every processor the array, and dividing it up among them all. They
/// sort their array chunks with serial MergeSort, and then these chunks are combined
/// and merged until process 0 has the final, sorted result. 
/// @param arr The array to sort
/// @param first_index The first index to sort
/// @param last_index The last index to sort
/// @param p The number of processes
/// @param my_rank My MPI rank
/// @param comm A MPI communicator
/// @param status A MPI status pointer
void MergeSortParallel(double *arr, int first_index, int last_index, int p, int my_rank, 
    MPI_Comm *comm, MPI_Status *status) {
       
    if (first_index < last_index) {
        int i;
        int indices[2];
        
        /* The range of indices that a process can sort. For example, if we have 16 items to sort
        and 4 processors, range will be 4. */
        int range = (last_index - first_index + 1) % p ? 
            (last_index - first_index + 1) / p + 1 : (last_index - first_index + 1) / p;
        
        /* Determine the indices between which the current process will sort */
        indices[0] = first_index + range * my_rank;
        indices[1] = my_rank + 1 == p ? last_index : indices[0] + range - 1;
        
        /* everyone does the mergesort on their part of the array */
        MergeSort(arr, indices[0], indices[1]);
        
        /* here's where it gets interesting. need an algorithm to bring the parallel data
        back and merge it. The idea here is that we need to iterate for log p levels, in
        order to merge the data back together. */
        int levels = (int)log2(p);
        if ((double)levels < log2(p))
            levels++;

        int my_id = 2 * my_rank;

        for (i = 0; i < levels; i++) {
            /* either I'm a sender or a receiver, or I've finished doing my work and can break */
            my_id /= 2;

            /* If my ID number is still even, I will be receiving data. */
            if (!(my_id % 2)) {
                /* where I'm receiving data from */
                int receive = my_rank + (int)pow(2, i);

                /* if the receiver node exists, we can get data from it. Otherwise we are
                done receiving data and will wait out this round. */
                if (receive < p) {
                    int receive_count, middle_index;
                    MPI_Recv(arr + indices[1] + 1, last_index - indices[1], MPI_DOUBLE, receive, i, *comm, status);
                    MPI_Get_count(status, MPI_DOUBLE, &receive_count);
                    middle_index = indices[1];
                    indices[1] += receive_count;
                    Merge(arr, indices[0], middle_index, indices[1]);
                } 
            }

            /* If my ID number is odd, I will be sending my data somewhere else, and quitting. */
            else {
                /* where I'm sending my data */
                int send = my_rank - (int)pow(2, i);
                
                MPI_Send(arr + indices[0], indices[1] - indices[0] + 1, MPI_DOUBLE, send, i, *comm);

                /* I've sent my data away, so I'm done */
                break;
            }
        }
    }
    
    /* Force all the processes to end at the same time */
    MPI_Barrier(*comm);
}

#endif
