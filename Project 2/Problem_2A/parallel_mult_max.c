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
        printf("USE LIKE THIS: parallel_mult_mat_mat file_A.csv n_row_A n_col_A file_B.csv n_row_B n_col_B result_matrix.csv time.csv num_threads \n");
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
        Please implement a parallel program to find the maximum value in the output matrix from a matrix-matrix multiplication. 
        To save memory, you cannot compute and store the entire output matrix in the memory and then search for the maximum from 
        the completed output matrix. Instead, you need to find the maximum as each element in the output matrix is computed
    */
    long int global_max = 0; //This will hold the global max value. It will need shared access by all threads so we initialize it outside the parallel region
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
                global_max = current; //Update the local max
            }
        }
    }

    // Record the finish time        
    double end = omp_get_wtime();
    
    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    // TODO: save the output matrix to the output csv file
    //the output file variable is outputFile - Editor's note - It might be handy to put in newlines after each row so you can actually read the file
    //Just a thought
    for(int i = 0; i < n_row1; i++)
    {
        for( int j = 0; j < n_col2; j++)
        {
            //Need to add the commas so that it's actually a csv
            fprintf(outputFile, "%d", resultMatrix[i * n_col2 + j]); //Writes each value to the file
            if (j != n_col2 - 1)
            {
                fprintf(outputFile, ","); 
            }
        }

        //Newline after each row EXCEPT the last so it stay's at 1000 lines - Will need to do the same thing with the leftover comma on each line
        if(i != n_row1 - 1)
        {
            fprintf(outputFile, "\n"); //Newline after each row
        }
    }

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

