/*
File: counting_sort_parallel.h
Author: Bryan Nguyen
*/

#ifndef COUNTING_SORT_PARALLEL
#define COUNTING_SORT_PARALLEL
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "../serial_sorting/counting_sort.h"

void CountingSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status);

/// @param arr The array to sort
/// @param p The number of processes
/// @param my_rank My MPI rank
/// @param comm A MPI communicator
/// @param status A MPI status pointer
void CountingSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status){
    // find length of sub array to divide original array evenly based on number of threads
    int count_array_size = (int)(ceil((double)(len) / (double)(p)));
    double* newArr;
    

    CountingSort(arr1, arr2, length, max_val);
    
    if (TestSortCorrectness(newArr, length)) {
        printf("The array sorted correctly.\n");
    } else {
        printf("The array did not sort correctly.\n");
    }

/*    MPI_Send(newArr, length, MPI_DOUBLE, 0, 1, *comm);*/
    if(my_rank == 0){
        /*for(int i = 0; i < p; i++){
            MPI_Recv(tempArr, len, MPI_DOUBLE, i, 1, *comm, status);
            print_array(tempArr, len);
        }*/
    }
}

#endif
