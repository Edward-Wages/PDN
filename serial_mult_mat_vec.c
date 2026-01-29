#include <stdio.h>
#include <stdlib.h> 
// Use more libraries as necessary

#define DEBUG 0

/* ---------- Project 1 - Problem 2 - Mat-Vec Mult ----------
    This file will multiply a matrix and vector.
    Complete the TODOs left in this file.
*/ // ------------------------------------------------------ //


int main (int argc, char *argv[])
{
    // Catch console errors
    if( argc != 7)
    {
        printf("USE LIKE THIS: serial_mult_mat_vec in_mat.csv n_row_1 n_col_1 in_vec.csv n_row_2 output_file.csv \n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE *matFile = fopen(argv[1], "r"); // Matrix File
    FILE *vecFile = fopen(argv[4], "r"); //Vector File

    // Get dim of the matrix
    char* p1;
    char* p2;
    int n_row1 = strtol(argv[2], &p1, 10 );
    int n_col1 = strtol(argv[3], &p2, 10 );

    // Get dim of the vector
    char* p3;
    int n_row2 = strtol(argv[5], &p3, 10 );

    // Get the output file
    FILE *outputFile = fopen(argv[6], "w");


    // TODO: Use malloc to allocate memory for the matrices - Done?
    //Matrix space will be n_row times n_col
    //Vector space will be n_row2

    int *matrix = malloc(n_row1 * n_col1 * sizeof(int)); //Creates space for the matrix
    int *vector = malloc(n_row2 * sizeof(int)); //Creates space for the vector

    if(matrix == NULL || vector == NULL)
    {
        printf("You messed soemthing up");
        return EXIT_FAILURE;
    }

    // TODO: Parse the input CSV files - Done?
    //Should be simple enough. Just need to learn how to read CSV files in C.

    /* Temp comment for visualizing data layout:
     Matrix:          Vector:
     94,88,97           10582
     78,56,74           8064
     38,84,51           5930
    
    */
    //Scan matrix file
    for(int i = 0; i < n_row1; i++)
    {
        for( int j = 0; j < n_col1; j++)
        {
            fscanf(matFile, "%d,", &matrix[i * n_col1 + j]); //Reads the matrix file & inserts into it's allocated space
        }

    }
    //By now the matrix should be fully read in

    //Scanning vector file
    for (int i = 0; i < n_row2; i++)
    {
        fscanf(vecFile, "%d,", &vector[i]); //Reads the vecFile & inserts the values
    }


    // TODO: Perform the matrix-vector multiplication - done?
    //We'll accomplish this by doing the following:
    // For each row in the matrix we'll compute the dot product of that row with the vector 
    //(I.E. matrix [0][0] * vector [0] & add for num columns)

    int *result = malloc(n_row1 * sizeof(int)); //Result vector will have n_row1 elements
    if(result == NULL)
    {
        printf("Back to the drawing board");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < n_row1; i++) //For each row in matrix
    {
        result[i] = 0; //Init
        for(int j = 0; j < n_col1; j++) //For each column in that row
        {
            result[i] += matrix[i * n_col1 + j] * vector[j]; //Execute dot product calculation
        }
    }

    // TODO: Write the output CSV file - the file variable is named output_file - done?
    /* Temp comment for visualizing output layout:
    out Vector:
    10582
    8064
    5930
    */
   for(int i = 0; i < n_row1; i++)//For each vector row
   {
        fprintf(outputFile, "%d\n", result[i]); //Writes the result in this row to the file
   }


    // TODO: Free memory
    free(matrix);
    free(vector);
    free(result);

    // Cleanup
    fclose (matFile);
    fclose (vecFile);
    fclose (outputFile);
    // Free buffers here as well!

    return 0;
}
