#include <cuda_runtime_api.h>
#include <curand_kernel.h>
#include <driver_types.h>
#include <curand.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <cstdio>
#include <cuda.h>

#include "support.h"
#include "kernel.cu"


void read_file(char* file, unsigned int* transactions, unsigned int n_transactions);

int main(int argc, char* argv[])
{
    /*
    Your program should be run like this:
    convolution_CUDA_parallel n_row n_col mat_input.csv mat_output_prob3.csv time_prob3_CUDA.csv

    Where:
    o	n_row, n_col: number of rows and columns, respectively
    o	mat_input.csv: matrix input in csv format
    o	mat_output_prob3.csv: matrix output in csv format
    o	time_prob3_CUDA.csv: You would output 3 wall-time times in csv format (1 column, 3 rows) to fill in the time table in the report

    */

    // Catch console errors
    if (argc != 6)
    {
        printf("USE LIKE THIS: convolution_CUDA n_row n_col mat_input.csv mat_output_prob3.csv time_prob3_CUDA.csv\n");
        return EXIT_FAILURE;
    }

    //output files
    FILE* output_file = fopen(argv[4], "w");
    FILE* time_file   = fopen(argv[5], "w");

    // Read in the input matrix
    unsigned int n_row = strtoul(argv[1], NULL, 10);
    unsigned int n_col = strtoul(argv[2], NULL, 10);
    unsigned int* mat_input = (unsigned int*)calloc(n_row * n_col, sizeof(unsigned int));
    read_file(argv[3], mat_input, n_row * n_col);

    // Set timer and cuda error return
    Timer timer;
    startTime(&timer);

    //1.	Transfer the input image (the A matrix) to the device memory
    unsigned int* d_mat_input;
    cudaMalloc((void**)&d_mat_input, n_row * n_col * sizeof(unsigned int));
    cudaMemcpy(d_mat_input, mat_input, n_row * n_col * sizeof(unsigned int), cudaMemcpyHostToDevice);


    //2.	Transfer the convolution filter (the K matrix) to the device memory
    //The convolution filter is a 5x5 matrix with the following values:
    // 1 0 0 0 1
    // 0 1 0 1 0
    // 0 0 1 0 0
    // 0 1 0 1 0
    // 1 0 0 0 1
    unsigned int* mat_kernel = (unsigned int*)calloc(5 * 5, sizeof(unsigned int));
    mat_kernel[0] = 1; mat_kernel[1] = 0; mat_kernel[2] = 0; mat_kernel[3] = 0; mat_kernel[4] = 1;
    mat_kernel[5] = 0; mat_kernel[6] = 1; mat_kernel[7] = 0; mat_kernel[8] = 1; mat_kernel[9] = 0;
    mat_kernel[10] = 0; mat_kernel[11] = 0; mat_kernel[12] = 1; mat_kernel[13] = 0; mat_kernel[14] = 0;
    mat_kernel[15] = 0; mat_kernel[16] = 1; mat_kernel[17] = 0; mat_kernel[18] = 1; mat_kernel[19] = 0;
    mat_kernel[20] = 1; mat_kernel[21] = 0; mat_kernel[22] = 0; mat_kernel[23] = 0; mat_kernel[24] = 1;

    unsigned int* d_mat_kernel;
    cudaMalloc((void**)&d_mat_kernel, 5 * 5 * sizeof(unsigned int));
    cudaMemcpy(d_mat_kernel, mat_kernel, 5 * 5 * sizeof(unsigned int), cudaMemcpyHostToDevice);

    stopTime(&timer);

    float time_H2D = elapsedTime(timer); //Getting time for copying data from host to device

    //3.	Launch the convolution kernel to compute the filter map (the B matrix) by applying the convolution to every pixel in the input image.
    unsigned int* d_mat_output;
    cudaMalloc((void**)&d_mat_output, n_row * n_col * sizeof(unsigned int));

    // Define how many threads are in one block
    dim3 blockSize(16, 16);

    // Calculate how many blocks we need to cover the whole image
    dim3 gridSize((n_col + blockSize.x - 1) / blockSize.x, (n_row + blockSize.y - 1) / blockSize.y);

    startTime(&timer); //Restart timer for kernel execution

    convolution_kernel<<<gridSize, blockSize>>> (
        d_mat_input,
        d_mat_kernel,
        d_mat_output,
        n_row,
        n_col
    );
    cudaDeviceSynchronize();
    stopTime(&timer);
    float time_Kernel = elapsedTime(timer); //Getting time for kernel execution


    //Perform max=pooling on the output of convolution
    unsigned int* d_mat_maxpooling_output;
    cudaMalloc((void**)&d_mat_maxpooling_output, n_row * n_col * sizeof(unsigned int));

    startTime(&timer); //Restart timer for copying data from device to host


    maxpooling_kernel<<<gridSize, blockSize>>> (
        d_mat_output,
        d_mat_maxpooling_output,
        n_row,
        n_col
    );

    cudaDeviceSynchronize();
    stopTime(&timer);
    float time_maxPooling = elapsedTime(timer); 


    startTime(&timer); //Restart timer for copying data from device to host
    //Transfer maxpooling output back to host
    unsigned int* h_mat_maxpooling_output = (unsigned int*)calloc(n_row * n_col, sizeof(unsigned int));
    cudaMemcpy(h_mat_maxpooling_output, d_mat_maxpooling_output, n_row * n_col * sizeof(unsigned int), cudaMemcpyDeviceToHost);

    stopTime(&timer);
    float time_D2H = elapsedTime(timer);

    //Write the output matrix & timing to the output files in csv format
    //Needs to be changed to fprintf
    for (unsigned int i = 0; i < n_row; i++) 
    {
        for (unsigned int j = 0; j < n_col; j++) 
        {
            fprintf(output_file, "%u", h_mat_maxpooling_output[i * n_col + j]);
            if (j < n_col - 1) 
            {
                fprintf(output_file, ",");
            }
        }
        fprintf(output_file, "\n");
    }



    fprintf(time_file, "%f\n%f\n%f\n%f\n", time_H2D, time_Kernel, time_maxPooling, time_D2H);


    //Freeing up memory
    cudaFree(d_mat_input);
    cudaFree(d_mat_kernel);
    cudaFree(d_mat_output);
    cudaFree(d_mat_maxpooling_output);
    
    free(mat_input);
    free(mat_kernel);
    free(h_mat_maxpooling_output);

    fclose(output_file);
    fclose(time_file);


    return 0;
}

/* Read File -------------------- //
*   Reads in a file of transactions. 
*/
void read_file(char* file, unsigned int* transactions, unsigned int n_transactions) {

    FILE* f = fopen(file, "r");
    if (!f) exit(-1);

    char line[65536]; // Big enough for a long CSV row
    unsigned int i = 0;
    while (fgets(line, sizeof(line), f) && i < n_transactions) {
        char* token = strtok(line, ",");
        while (token != NULL && i < n_transactions) {
            transactions[i++] = (unsigned int)strtoul(token, NULL, 10);
            token = strtok(NULL, ",");
        }
    }
    fclose(f);

} // End Read File ------------- //
