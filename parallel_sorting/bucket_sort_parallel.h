/*
File: bucket_sort_parallel.h

This file implements bucket sort in parallel.
Reference: "Introduction to Algorithms, 3rd Edition," by Cormen, Leiserson, Rivest, and Stein
(chapter 2 for insertion sort, chapter 8 for bucket sort)

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef BUCKET_SORT_PARALLEL
#define BUCKET_SORT_PARALLEL
#include <stdlib.h>
#include <mpi.h>
#include "../serial_sorting/bucket_sort.h"
#include "../data_structures/vector.h"

void BucketSortParallel(double *arr, int length, double a, double b, int p, int my_rank, MPI_Comm *comm, MPI_Status *status);
static void _bucketSortAux(double *arr, int first_index, int last_index, double a, double b);

/// @brief Implements BucketSort as described on p. 201 of CLRS edition 3. We assume that
/// all the numbers are from a to b in a uniform random distribution. They do not need
/// to be in a uniform random distribution for the sort to work, but it would be less 
/// efficient otherwise.
/// @param arr An array to sort
/// @param length The length of the array
/// @param a The lowest number in the range to be sorted
/// @param b The highest number in the range to be sorted
void BucketSortParallel(double *arr, int length, double a, double b, int p, int my_rank, MPI_Comm *comm, MPI_Status *status) {
    int h, i, j, k, bucket_idx;
    
    /* Get the subarray for this processor to bucketize. Each subarray is 
    approximately of size length / p. */
    int my_share = length % p ? length / p + 1 : length / p;
    int my_idx_first = my_share * my_rank;
    int my_idx_last = my_rank + 1 == p ? length - 1 : my_idx_first + my_share - 1;

    /* Get the range width of numbers that can be allocated to one process */
    double range = (b - a + 0.0001) / p;
    
    /* We will put our buckets here. Note that the first p indices of the array hold
    the starting index of the bucket for the corresponding process rank. The p + 1th
    index holds the last index used (which might not be the final index in the array). */
    int length_my_buckets = my_share + p + 1;
    double *my_buckets = (double*)malloc(length_my_buckets * p * sizeof(double));
    double *my_final_arrs = (double*)malloc(length_my_buckets * p * sizeof(double));

    /* This will hold our buckets until we combine them into the array */
    struct vector_d *my_temp_buckets = (struct vector_d*)malloc(p * sizeof(struct vector_d));
    for (i = 0; i < p; i++)
        my_temp_buckets[i] = VectorDConstruct(my_share);
    
    /* Bucketize the numbers */
    for (i = my_idx_first; i <= my_idx_last; i++) {
        bucket_idx = (int)(arr[i] / range);
        VectorDPushBack(&(my_temp_buckets[bucket_idx]), arr[i]);
    }

    /* Combine the buckets for sending. This is a bit complicated because we are
    using an all-to-all broadcast. Our combined array will have to have p copies
    of all of our local process's buckets, in sequence, one copy for each process.
    This is controlled by the variable h.
    The first p indices (of each copy of the buckets) contain the starting indices
    for the respective processors. The p+1th bucket (in each copy of the buckets)
    contains the last index used in the array. */
    for (h = 0; h < p; h++) {
        /* setup the starting index for process 0 */
        k = p + 1;
        my_buckets[0 + length_my_buckets * h] = (double)(p + 1);

        /* copy the data for process 0 into its bucket */
        for (j = 0; j < my_temp_buckets[0].size; j++) {
            my_buckets[k + length_my_buckets * h] = my_temp_buckets[0].data[j];
            k++;
        }

        /* do the same for the other processors' buckets */
        for (i = 1; i < p; i++) {
            /* get the starting index for process i */
            my_buckets[i + length_my_buckets * h] = my_buckets[i - 1 + length_my_buckets * h] + (double)my_temp_buckets[i-1].size;
            
            /* copy the data for process i into its bucket */
            for (j = 0; j < my_temp_buckets[i].size; j++) {
                my_buckets[k + length_my_buckets * h] = my_temp_buckets[i].data[j];
                k++;
            }
        }

        /* set the final position in the array + 1 */
        my_buckets[p + length_my_buckets * h] = k;
    }

    /* Dispatch the buckets */
    MPI_Alltoall(my_buckets, length_my_buckets, MPI_DOUBLE, 
        my_final_arrs, length_my_buckets, MPI_DOUBLE, *comm);

    /* Make a bucket for this processor that only contains the values it will sort */    
    struct vector_d my_bucket = VectorDConstruct(length / p);
    for (i = 0; i < p; i++) {
        int offset_start = (int)my_final_arrs[i * length_my_buckets + my_rank] + i * length_my_buckets;
        int offset_end = (int)my_final_arrs[i * length_my_buckets + my_rank + 1] + i * length_my_buckets;
        for (j = offset_start; j < offset_end; j++)
            VectorDPushBack(&my_bucket, my_final_arrs[j]);
    }

    /* Sort the contents of my bucket */
    _bucketSortAux(my_bucket.data, 0, my_bucket.size - 1, my_rank * range, my_rank * range + range);
    
    if (my_rank == 0) {
        int recv, next_index;
        for (i = 0; i < my_bucket.size; i++)
            arr[i] = my_bucket.data[i];
        next_index = my_bucket.size;
        for (i = 1; i < p; i++) {
            MPI_Recv(arr + next_index, length, MPI_DOUBLE, i, i, *comm, status);
            MPI_Get_count(status, MPI_DOUBLE, &recv);
            next_index += recv;
        }
    } else {
        MPI_Send(my_bucket.data, my_bucket.size, MPI_DOUBLE, 0, my_rank, *comm);
    }

    free(my_buckets);
    free(my_final_arrs);
    for (i = 0; i < p; i++)
        VectorDDestruct(&(my_temp_buckets[i]));
    free(my_temp_buckets);
    VectorDDestruct(&my_bucket);
    
    /* Force all the processes to end at the same time */
    MPI_Barrier(*comm);
}

/// @brief Implements BucketSort as described on p. 201 of CLRS edition 3. We assume that
/// all the numbers are from a to b in a uniform random distribution. They do not need
/// to be in a uniform random distribution for the sort to work, but it would be less 
/// efficient otherwise.
/// @param arr An array to sort
/// @param first_index The first index to sort
/// @param last_index The last index to sort
/// @param a The lowest number in the range to be sorted
/// @param b The highest number in the range to be sorted
static void _bucketSortAux(double *arr, int first_index, int last_index, double a, double b) {  
    int length = last_index - first_index + 1;
    struct vector_d *buckets = (struct vector_d*)malloc(length * sizeof(struct vector_d));
    int i, j;
    int k = 0;

    for (i = 0; i < length; i++)
        buckets[i] = VectorDConstruct(2);
    
    for (i = 0; i < length; i++) {
        int bucket_id = (int)(length * arr[i]);
        VectorDPushBack(&(buckets[bucket_id]), arr[i]);
    }

    for (i = 0; i < length; i++)
        _insertionSortAux(buckets[i].data, buckets[i].size);

    for (i = 0; i < length; i++) {
        for (j = 0; j < buckets[i].size; j++) {
            arr[k] = buckets[i].data[j];
            k++;
        }
        VectorDDestruct(&(buckets[i]));
    }
    free(buckets);
}

#endif
