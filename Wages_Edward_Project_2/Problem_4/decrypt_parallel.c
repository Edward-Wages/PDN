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

int the_count(unsigned char* buffer, long size)
{
    int count = 0;
    for (long i = 0; i <= size - 3; i++) 
    {
        if ((buffer[i] == 't' || buffer[i] == 'T') &&
            (buffer[i+1] == 'h' || buffer[i+1] == 'H') &&
            (buffer[i+2] == 'e' || buffer[i+2] == 'E')) 
        {
            count++;
            i += 2; // Jump ahead to avoid overlapping checks
        }
    }
    return count;
}

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


    //Begin Decryption
    double start_time = omp_get_wtime();

    int maxCount = -1; //The max count of "the"

    //Should each thread have it's own decrypted buffer?
    #pragma omp parallel for num_threads(num_threads)
    for(int potential_key = 0; potential_key < 256; potential_key++)
    {
        unsigned char* decryped_buffer = calloc(1, lSize + 1);
        


        //Decrypt the text using the potential key
        for(long i = 0; i < lSize; i++)
        {
            decryped_buffer[i] = (buffer[i] - potential_key + 256 ) % 256; //We add 256 to ensure we're not getting negative values before the mod operation
        }
        decryped_buffer[lSize] = '\0'; // Null-terminate

        //Call our helper function
        int count = the_count(decryped_buffer, lSize);

        #pragma omp critical
        if(count > maxCount)
        {
            maxCount = count;
            key = potential_key;
        }
        free(decryped_buffer); //Free before the end of the loop to avoid memory leaks
    }

    double end_time = omp_get_wtime() - start_time;

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