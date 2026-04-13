#include <stdio.h>
#include <stdlib.h> // for strtol
#include <string.h>
#include <time.h>
#include "mpi.h"

/*
Write an MPI program using Monte Carlo method to estimate the value of π. 

For this purpose, we randomly generate a large number of points (x, y), where 0 ≤ x, y ≤ 1. 
All these points lie in the square with each of its sides of length 1 with its south-west corner at origin.
*/

int main(int argc, char *argv[])
{
    if( argc != 3)
    {
        printf("USE LIKE THIS: pi_MPI result_prob4.csv time_prob4_MPI.csv\n");
        return EXIT_FAILURE;
    }

    //Start MPI
    int my_rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    srand(time(NULL) + my_rank); // Seed the random number generator differently for each process

    int total_points = 65536;
    int local_points = total_points / comm_size;
    int local_count = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double local_start = MPI_Wtime();

    /*
    Each process generates a subset of the random points and determines the number of points that lie within the circular part of the first quadrant. 
    For different processes to generate a sequence of different random numbers, choose a different seed value for generating random numbers for each process. 
    This seed value can be a very simple function of the process id. 
    */
   for(int i = 0; i < local_points; i++)
    {
        double x = (double)rand() / RAND_MAX; // Random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX; // Random y coordinate between 0 and 1

        if (x*x + y*y <= 1.0) // Check if the point is inside the quarter circle
        {
            local_count++;
        }
    }

     /*
    Next, the process 0 should sum up the counts of all the points that lie in the sector (red points) calculated by all the processes. 
    Finally, the process 0 should compute π and print out the value of π. 
    The value of π is 4 times of the ratio of (the number of points within circular part)/(the total number of points generated). 
    */

    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - local_start;
    double max_elapsed;
    MPI_Reduce(&elapsed_time, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        double pi_estimate = 4.0 * global_count / total_points;

        FILE* outputFile = fopen(argv[1], "w");
        FILE* timeFile = fopen(argv[2], "w");
        fprintf(outputFile, "%lf\n", pi_estimate);
        fprintf(timeFile, "%lf\n", max_elapsed);

        fclose(outputFile);
        fclose(timeFile);
    }

    MPI_Finalize();
    return 0;
}