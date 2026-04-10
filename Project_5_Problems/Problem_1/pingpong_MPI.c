#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char* argv[]) 
{

    MPI_Init(&argc, &argv);
    int my_rank, comm_sz;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    
    // Catch console errors
    if (argc != 3)
    {
        printf("USE LIKE THIS: pingpong_MPI n_items time_prob1_MPI.csv\n");
        return EXIT_FAILURE;
        MPI_Finalize();
    }

    int n_items = strtol(argv[1], NULL, 10);
    int* ping_array = malloc(n_items * sizeof(int));

    if(my_rank == 0)
    {
        for(int i = 0; i < n_items; i++)
        {
            ping_array[i] = i;
        }

        FILE* outputFile = fopen(argv[2], "w");
    }

    // Start time
    MPI_Barrier(MPI_COMM_WORLD); // Ensure all processes start at the same time
    double starttime = MPI_Wtime();


    for(int i = 0; i < 1000; i++)
    {
        if (my_rank == 0)
        {
            MPI_Send(ping_array, n_items, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(ping_array, n_items, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else if (my_rank == 1)
        {
            MPI_Recv(ping_array, n_items, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(ping_array, n_items, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if(my_rank == 0)
    {
        double endtime = MPI_Wtime();
        double total_time = endtime - starttime;
        fprintf(outputFile, "%d, %f\n", n_items, total_time);
    }

    free(ping_array);
    fclose(outputFile);

    MPI_Finalize();
    return 0;
} /* main */

