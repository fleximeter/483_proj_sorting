/*
File: Bubble_sort_parallel.h
Author: Justin Hendershot
Date: 4/22/23

This file implements bubble sort.
Reference: https://iq.opengenus.org/parallel-bubble-sort/#:~:text=Parallel%20bubble%20sort%20is%20a,machines%20and%20computer%20programming%20generally
*/

#ifndef BUBBLE_SORT_PARALLEL
#define BUBBLE_SORT_PARALLEL
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "../serial_sorting/bubble_sort.h"

void BubbleSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status);

/// @param arr The array to sort
/// @param p The number of processes
/// @param my_rank My MPI rank
/// @param comm A MPI communicator
/// @param status A MPI status pointer
void BubbleSortParallel(double *arr, int len, int p, int my_rank, MPI_Comm *comm, MPI_Status *status) {
    
    double newArr[len];
    double tempArr[len];
    int number_received = 0;
    int x = 0; 
    int length = 0;
    
    //Calculates the range of numbers to be sent to a thread
    double low = (1 / (double)p) * (double)my_rank;
    double high = (1 / (double)p) * (double)(my_rank + 1);

    //Prints which range of numbers are going to this thread
    //printf("Thread:%d : %f, %f\n", my_rank, low, high);

    //Finds each number between the range from the main array and puts it in a temp array
    for(int i = 0; i < len; i++){
        if((arr[i] <= high) && (arr[i] > low)){
            newArr[x] = arr[i];
            x++; 
            length++;
        }
    }

    //Calls serial bubble sort of the temp array
    BubbleSort(newArr, length);

    //Combines each temp array back into the original array
    if (my_rank == 0){
        int y = 0;
        for(int x = 0; x < length; x++){
            arr[y] = tempArr[x];
            y++;
        }
        for (int i = 1; i < p; i++) {
            MPI_Recv(tempArr, len, MPI_DOUBLE, i, 1, *comm, status);
            MPI_Get_count(status, MPI_DOUBLE, &number_received);
            for(int x = 0; x < number_received; x++){
                arr[y] = tempArr[x];
                y++;
            }
        }
    } else {
        MPI_Send(newArr, length, MPI_DOUBLE, 0, 1, *comm);
    }
}

#endif
