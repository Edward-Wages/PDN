#include <stdio.h>
#include <stdlib.h> // for strtol
#include <string.h>
#include <time.h>
#include "mpi.h"


#define MAXCHAR 25
#define BILLION  1000000000.0

//Please parallelize the dot product algorithm using MPI. Process 0 will read in the two input arrays and print out the result.

int main (int argc, char *argv[])
{
    if( argc != 6)
    {
        printf("USE LIKE THIS: dot_product_MPI n_items vec_1.csv vec_2.csv result_prob2_MPI.csv time_prob2_MPI.csv\n");
        return EXIT_FAILURE;
    }

    // Size
    int vec_size = strtol(argv[1], NULL, 10);

    // Input files
    FILE* inputFile1 = fopen(argv[2], "r");
    FILE* inputFile2 = fopen(argv[3], "r");
    if (inputFile1 == NULL) printf("Could not open file %s", argv[2]);
    if (inputFile2 == NULL) printf("Could not open file %s",argv[3]);

    // Output files
    FILE* outputFile = fopen(argv[4], "w");
    FILE* timeFile = fopen(argv[5], "w");

    //Start MPI
    int my_rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    double* vec_1 = NULL;
    double* vec_2 = NULL;

    //Process 0 reads in the input arrays & handles allocation of arrays
    if(my_rank == 0)
    {
        vec_1 = malloc(vec_size * sizeof(double));
        vec_2 = malloc(vec_size * sizeof(double));

        // Store values of vector
        int k = 0;
        char str[MAXCHAR];
        while (fgets(str, MAXCHAR, inputFile1) != NULL)
        {
            sscanf( str, "%lf", &(vec_1[k]) );
            k++;
        }
        fclose(inputFile1); 

        // Store values of vector
        k = 0;
        while (fgets(str, MAXCHAR, inputFile2) != NULL)
        {
            sscanf( str, "%lf", &(vec_2[k]) );
            k++;
        }
        fclose(inputFile2); 
    }
  
    
    // Get start time with MPI
    double local_start, local_finish, local_elapsed, elapsed;
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();


    // Get dot product 
    double global_dot_product = 0.0;
    double local_dot_product = 0.0;
    int local_n = vec_size / comm_size;
    // Scatter the two vectors to all proccesses
    double* local_vec = (double*)malloc(local_n * sizeof(double));
    MPI_Scatter(vec_1, local_n, MPI_DOUBLE, local_vec, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double* local_vec2 = (double*)malloc(local_n * sizeof(double));
    MPI_Scatter(vec_2, local_n, MPI_DOUBLE, local_vec2, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
    {
        local_dot_product += local_vec[i] * local_vec2[i];
    }

    //Reduce the local products to get the global dot product
    MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    // Get finish time
    local_finish = MPI_Wtime();
    local_elapsed = local_finish - local_start;
    MPI_Allreduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        printf("DP in C: %lf\n", global_dot_product);
        fprintf(outputFile, "%lf", global_dot_product);
        fprintf(timeFile, "%.20f", elapsed);
    }

    // Cleanup
    fclose (outputFile);
    fclose (timeFile);

    free(vec_1);
    free(vec_2);
    free(local_vec);
    free(local_vec2);

    MPI_Finalize();
    return 0;
}