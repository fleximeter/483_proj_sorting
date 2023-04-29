/*
File: vector.h
Author: Jeff Martin
Date: 4/14/23

This file implements a simple vector. To create a vector, you call the
VectorDConstruct() function. To free a vector, you call the VectorDDestruct() 
function. To insert at the end, you call the PushBack() function.

Note: This is from Jeff Martin's Homework 4.
*/

#ifndef VECTOR
#define VECTOR
#define MIN_VEC_SIZE 2
#include <stdlib.h>

struct vector_d VectorDConstruct();
void VectorDDestruct(struct vector_d *vec);
double VectorDGet(struct vector_d *vec, int index);
double VectorDPopBack(struct vector_d *vec);
void VectorDPushBack(struct vector_d *vec, double data);
void VectorDSet(struct vector_d *vec, int index, double data);

/// @brief A simple vector of doubles
typedef struct vector_d {
    double *data;
    int size;
    int _capacity;
} vector_d;

/// @brief Creates a vector of doubles
/// @return A vector of doubles
struct vector_d VectorDConstruct() {
    struct vector_d vec;
    vec.data = (double*)malloc(MIN_VEC_SIZE * sizeof(double));
    vec.size = 0;
    vec._capacity = MIN_VEC_SIZE;
    return vec;
}

/// @brief Destructs a vector of doubles
/// @param vec The vector
void VectorDDestruct(struct vector_d *vec) {
    free(vec->data);
    vec->size = 0;
    vec->_capacity = 0;
}

/// @brief Gets the item at a specific index in a vector
/// @param vec The vector
/// @param index The index
/// @return The value at that index
double VectorDGet(struct vector_d *vec, int index) {
    return vec->data[index];
}

/// @brief Removes the last item from the vector and returns it
/// @param vec The vector
/// @return The item
double VectorDPopBack(struct vector_d *vec) {
    vec->size--;
    return vec->data[vec->size];
}

/// @brief Adds a new item to the end of a vector
/// @param vec The vector
/// @param data The new item
void VectorDPushBack(struct vector_d *vec, double data) {
    if (vec->size >= vec->_capacity) {
        double *new_vec = (double*)malloc(2 * vec->_capacity * sizeof(double));
        int i;
        for (i = 0; i < vec->size; i++)
            new_vec[i] = vec->data[i];
        free(vec->data);
        vec->data = new_vec;
        vec->_capacity *= 2;
    }
    vec->data[vec->size] = data;
    vec->size++;
}

/// @brief Sets the value of an index in a vector
/// @param vec The vector
/// @param index The index
/// @param data The new value
void VectorDSet(struct vector_d *vec, int index, double data) {
    vec->data[index] = data;
}

#endif
