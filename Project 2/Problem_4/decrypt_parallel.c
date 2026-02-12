#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
/*
Please write a multi-threaded parallel algorithm to crack an encrypted English text using brute-force computation. 
There are 256 possible keys for the Caesar Encryption algorithm. Among all these possible keys, 
let us assume that the correct key should yield a text that contains the most occurrence of the 
word "The" or "the", because the word "the" is the most frequent word in English.
*/

//It should be run in this way: decrpyt_parallel input_text.txt key.txt time.txt num_threads

int main (int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("USE LIKE THIS: decrpyt_parallel input_text.txt key.txt time.txt num_threads\n");
        return EXIT_FAILURE;
    }

    // Open the encrypted text file
    FILE* inputFile = fopen(argv[1], "r"); //Should I be writing to the file with the unecrypted text?

    //Open the key file
    FILE* keyFile = fopen(argv[2], "w");
    //We are looking for the key that yields the most occurence of "the", so we're writing to this file

    FILE* timeFile = fopen(argv[3], "w");

    int num_threads = atoi(argv[4]);
    int key = 0; //This will get updated as we loop through possible keys

    //Allocating the buffer for input
    fseek(inputFile, 0l, SEEK_END);
    long lSize = ftell(inputFile);
    rewind(inputFile);
    unsigned char* buffer = calloc(1, lSize + 1);
    if (!buffer)
    {
        fclose(inputFile),
        fclose(keyFile),
        fclose(timeFile),
        free(buffer),
        fputs("Memory allocation failed\n", stderr),
        exit(1);
    }


        //Need to read in the input text into the buffer

    if(1 != fread(buffer, lSize, 1, inputFile))
    {
        fclose(inputFile),
        fclose(keyFile),
        // fclose(timeFile),
        free(buffer),
        fputs("Failed reading into the input buffer!\n", stderr),
        exit(2);
    }
    
    //Do I need a buffer for the decrypted text?


    //Begin Decryption
    double start_time = omp_get_wtime();

    #pragma omp parallel for num_threads(num_threads)
    //Decryption Happens here
    //High level steps:
    //for each potential key, alter the text using the key & count the number of times "the" appears
    //I think each thread will need a private count variable for which key has the most occurences as well as the actual max count
    //Utilize join by reduction to find the key that yields the most occurences

    double end_time = omp_get_wtime(); - start_time;

    //Print the total time to the file
    fprintf(timeFile, "%f", end_time);

    //Write the key to the key file
    fprintf(keyFile, "%d", key);

     // Cleanup
    fclose(inputFile);
    fclose(timeFile);
    fclose(keyFile);
    free(buffer);

    return 0;
}