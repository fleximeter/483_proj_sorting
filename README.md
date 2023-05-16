# Five Parallel Sorting Implementations in C and OpenMPI

### Justin Hendershot, Jeff Martin, and Bryan Nguyen
jhender2@umbc.edu, jmartin8@umbc.edu, bryann1@umbc.edu

## Introduction
This is the code for our technical report on sorting algorithms in MPI. The `makefile` is set up to make four executables, the most important of which are `svp_driver` and `svp_driver_weak_scale`.

## Running sorts
After running the `make` command, you can execute the `svp_driver` by using the command
`mpiexec -n 4 ./svp_driver`
This program can run five different parallel sorting algorithms (Bubble Sort, Insertion Sort, Quick Sort, Merge Sort, and Bucket Sort). To set which algorithm it is using, you need to edit the `sort_num` variable on line 54 and recompile the program. When run, the program tests the sorting algorithm on three different array sizes: 1000, 10000, and 100000.

## Getting weak scalability data
The program `svp_driver_weak_scale` is designed for collecting weak scalability data. It chooses an array size based on the number of MPI processes. You can execute this program by using the command
`mpiexec -n 4 ./svp_driver_weak_scale`
