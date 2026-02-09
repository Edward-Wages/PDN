#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

#define DEBUG 0

/* ----------- Project 2 - Problem 1 - Matrix Mult -----------

    This file will multiply two matricies.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //

//Simple helper function to read the input files & fill the matrices
void parse_csv(FILE* file, long int* matrix, int n_row, int n_col)
{
    for(int i = 0; i < n_row; i++)
    {
        for( int j = 0; j < n_col; j++)
        {
            fscanf(file, "%d,", &matrix[i * n_col + j]); //Reads the matrix file & inserts into it's allocated space
        }

    }
}

int main(int argc, char* argv[])
{
    // Catch console errors
    if (argc != 10)
    {
        printf("USE LIKE THIS: parallel_mult_mat_mat file_A.csv n_row_A n_col_A file_B.csv n_row_B n_col_B result_second_largest.csv time.csv num_threads \n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE* inputMatrix1 = fopen(argv[1], "r");
    FILE* inputMatrix2 = fopen(argv[4], "r");

    char* p1;
    char* p2;

    // Get matrix 1's dims
    int n_row1 = strtol(argv[2], &p1, 10);
    int n_col1 = strtol(argv[3], &p2, 10);

    // Get matrix 2's dims
    int n_row2 = strtol(argv[5], &p1, 10);
    int n_col2 = strtol(argv[6], &p2, 10);

    // Get num threads
    int thread_count = strtol(argv[9], NULL, 10);

    // Get output files
    FILE* outputFile = fopen(argv[7], "w");
    FILE* outputTime = fopen(argv[8], "w");


    // TODO: malloc the two input matrices and the output matrix
    // Please use long int as the variable type
    long int *matrix1 = malloc(n_row1 * n_col1 * sizeof(long int));
    long int *matrix2 = malloc(n_row2 * n_col2 * sizeof(long int));
    long int *resultMatrix = malloc(n_row1 * n_col2 * sizeof(long int));
    //Resulting matrix needs to be of size n_row1 x n_col2

    // TODO: Parse the input csv files and fill in the input matrices
    parse_csv(inputMatrix1, matrix1, n_row1, n_col1);
    parse_csv(inputMatrix2, matrix2, n_row2, n_col2);

    // We are interesting in timing the matrix-matrix multiplication only
    // Record the start time
    double start = omp_get_wtime();
    
    // TODO: Parallelize the matrix-matrix multiplication
    //We'll have 3 loops. the outer most will be parallelized. Each thread wil be responsible for calculating a number of rows in the result matrix.
    // Check to see if this actually works

    /*NEW TODO: 
       Please implement a parallel program to find the second largest value in the output matrix from a matrix-matrix multiplication. 
       Similar to the problem step in Problem 2A, you cannot compute and store the entire output matrix in the memory and 
       then search for the second largest value from the completed output matrix.
    */
    long int global_max = 0; //This will hold the global max value. It will need shared access by all threads so we initialize it outside the parallel region
    long int global_second_max = 0; 
    #pragma omp parallel for num_threads(thread_count) reduction(max: global_max) 
    for(int i = 0; i < n_row1; i++)
    {
        for(int j = 0; j < n_col2; j++)
        {
            long int current = 0;
            resultMatrix[i * n_col2 + j] = 0; //Initializing the space
            for(int k = 0; k < n_col1; k++)
            {
                current += matrix1[i * n_col1 + k] * matrix2[k * n_col2 + j]; //Calculate the result early so we can check if it's the max
            }

            resultMatrix[i * n_col2 + j] += current; //Assign the value

            if(current > global_max)
            {
                global_second_max = global_max;
                global_max = current;
            }
            else if(current > global_second_max)
            {
                global_second_max = current;
            }
            /*
            if (x > largest)
			secondLargest = largest
               largest = x
          else if (x > secondLargest)
               secondLargest = x
            */
        }
    }

    // Record the finish time        
    double end = omp_get_wtime();
    
    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    //TODO: Print the global max to the output file.
    fprintf(outputFile, "%d", global_second_max);

    // Cleanup
    fclose(inputMatrix1);
    fclose(inputMatrix2);
    fclose(outputFile);
    fclose(outputTime);
    // Remember to free your buffers!
    free(matrix1);
    free(matrix2);
    free(resultMatrix);

    return 0;
}

