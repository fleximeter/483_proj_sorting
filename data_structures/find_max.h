/*
File: find_max.h
Author: Bryan Nguyen
*/

#ifndef FIND_MAX
#define FIND_MAX
#include <stdio.h>

int MaxElementI(int* arr, int n);
double MaxElementD(double* arr, int n);

int MaxElementI(int* arr, int n){
    int i;
    int max = arr[0];
    for (i = 1; i < n; i++){
        if (arr[i] > max)
            max = arr[i];
    }
  
    return max;
}

double MaxElementD(double* arr, int n){
    int i;
    double max = arr[0];
    for (i = 1; i < n; i++){
        if (arr[i] > max)
            max = arr[i];
    }
  
    return max;
}
#endif
