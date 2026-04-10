#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define MAXLINE 25
#define DEBUG   0

// to read in file
float* read_input(FILE* inputFile, int n_items);
int cmpfloat(const void* a, const void* b);
void merge(float* arr1, int n1, float* arr2, int n2, float* result);

/* Main Program -------------- */
int main (int argc, char *argv[])
{
    if( argc != 5)
    {
        printf("USE LIKE THIS: merge_sort_MPI n_items input.csv output.csv time.csv\n");
        return EXIT_FAILURE;
    }

    // input file and size
    FILE* inputFile = fopen(argv[2], "r");
    int  n_items = strtol(argv[1], NULL, 10);

    // Start MPI
    int my_rank, comm_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // arrays to use
    // TODO: initialize your arrays here

    // get start time
    double local_start, local_finish, local_elapsed, elapsed;
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();



    // TODO: implement solution here
    /* MPI Merge Sort Pseudocode:
    Read a global array from an input file using process 0 - Done
    Scatter the global array across all processes - Done
    Sort the local arrays by every process using the build-in quick sort function (qsort) - Done
    Reduce the local sorted arrays on all processes to a global sorted array on process 0 - Done? I think I actually have to use tree reduction instead
    Write the global sorted array to an output file using process 0 - Done
    */

    float* global_arr;

    if(my_rank == 0)
    {
        global_arr = read_input(inputFile, n_items);
        fclose(inputFile);
    }

    int local_n = n_items / comm_size;
    float *local_arr = (float*)malloc(local_n * sizeof(float));

    MPI_Scatter(
        global_arr, 
        local_n, 
        MPI_FLOAT, 
        local_arr, 
        local_n, 
        MPI_FLOAT,
        0, 
        MPI_COMM_WORLD
    );

    qsort(local_arr, local_n, sizeof(float), cmpfloat);

    //Gather the sorted subarrays to process 0
    float *sorted_arr = NULL;
    if(my_rank == 0)
    {
        sorted_arr = (float*)malloc(n_items * sizeof(float));
    }

    //Implementing tree reduction for gathering sorted subarrays
    int divisor = 2;
    int core_difference = 1;

    while(divisor <= comm_size)
    {
        //Sender proccesses
        if(my_rank % divisor == 0)
        {
            int sender = my_rank + core_difference;

            if(sender < comm_size)
            {
                int recv_size = local_n * core_difference;
                float* recv_buffer = (float*)malloc(recv_size * sizeof(float));

                MPI_Recv(recv_buffer, recv_size, MPI_FLOAT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                float* temp_merge = (float*)malloc((local_n + recv_size) * sizeof(float));
                merge(local_arr, local_n, recv_buffer, recv_size, temp_merge);

                free(local_arr);
                free(recv_buffer);
                local_arr = temp_merge;
                local_n += recv_size;
            }
        }
        else
        {
            int receiver = my_rank - core_difference;
            MPI_Send(local_arr, local_n, MPI_FLOAT, receiver, 0, MPI_COMM_WORLD);
            break;
        }

        divisor *= 2;
        core_difference *= 2;
    }

    if(my_rank == 0)
    {
        sorted_arr = local_arr;
    }




    // get elapsed time
    local_finish  = MPI_Wtime();
    local_elapsed = local_finish-local_start;

    // send time to main process
    MPI_Reduce(
        &local_elapsed, 
        &elapsed, 
        1, 
        MPI_DOUBLE,
        MPI_MAX, 
        0, 
        MPI_COMM_WORLD
    );

    // Write output (Step 5)
    if (my_rank == 0) {
        FILE* outputFile = fopen(argv[3], "w");
        FILE* timeFile = fopen(argv[4], "w");

        // TODO: output - the test output has each number on a new line
        for(int i = 0; i < n_items; i++)
        {
            fprintf(outputFile, "%f\n", sorted_arr[i]);
        }

        fprintf(timeFile, "%.20f", elapsed);

        fclose(outputFile);
        fclose(timeFile);
    }


    MPI_Finalize();

    if(DEBUG) printf("Finished!\n");
    return 0;
} // End Main //



/* Read Input -------------------- */
float* read_input(FILE* inputFile, int n_items) {
    float* arr = (float*)malloc(n_items * sizeof(float));
    char line[MAXLINE] = {0};
    int i = 0;
    char* ptr;
    while (fgets(line, MAXLINE, inputFile)) {
        sscanf(line, "%f", &(arr[i]));
        ++i;
    }
    return arr;
} // Read Input //



/* Cmp Int ----------------------------- */
// use this for qsort
// source: https://stackoverflow.com/questions/3886446/problem-trying-to-use-the-c-qsort-function
int cmpfloat(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
} // Cmp Int //

//Helper method for merging the sorted arrays
void merge(float* arr1, int n1, float* arr2, int n2, float* result) 
{
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) result[k++] = arr1[i++];
        else result[k++] = arr2[j++];
    }
    while (i < n1) result[k++] = arr1[i++];
    while (j < n2) result[k++] = arr2[j++];
}