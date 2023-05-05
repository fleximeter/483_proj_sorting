/*
File: insertion_sort_parallel.h
Author: Bryan Nguyen
*/

#ifndef INSERTION_SORT_PARALLEL
#define INSERTION_SORT_PARALLEL
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "../serial_sorting/insertion_sort.h"

void InsertionSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status);

/// @param arr The array to sort
/// @param p The number of processes
/// @param my_rank My MPI rank
/// @param comm A MPI communicator
/// @param status A MPI status pointer
void InsertionSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status) {
    
    double* newArr;

    newArr = (double*)malloc(len * sizeof(double));
    int x = 0; 
    int length = 0;
    double tempArr[len];

    double low = (1 / (double)p) * (double)my_rank;
    double high = (1 / (double)p) * (double)(my_rank + 1);

    printf("Thread:%d : %f, %f\n", my_rank, low, high);

    for(int i = 0; i < len; i++){
        if((arr[i] <= high) && (arr[i] > low)){
            newArr[x] = arr[i];
            x++; 
            length++;
        }
    }

    InsertionSort(newArr, length);
    
    if (TestSortCorrectness(newArr, length)) {
        printf("The array sorted correctly.\n");
    } else {
        printf("The array did not sort correctly.\n");
    }

    MPI_Send(newArr, length, MPI_DOUBLE, 0, 1, *comm);

    if(my_rank == 0){
        for(int i = 0; i < p; i++){
            MPI_Recv(tempArr, len, MPI_DOUBLE, i, 1, *comm, status);
            print_array(tempArr, len);

        }
    }
}

#endif
