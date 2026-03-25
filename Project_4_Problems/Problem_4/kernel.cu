__global__ 
void convolution_kernel(unsigned int* A, unsigned int* K, unsigned int* B, int n_row, int n_col) 
{
    //A is the input matrix, K is the convolution filter, B is the output
    // Calculate global row and column
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < n_row && col < n_col) 
    {
        unsigned int sum = 0;

        // Filter is 5x5, so we loop from -2 to 2
        for (int i = -2; i <= 2; i++) 
        {
            for (int j = -2; j <= 2; j++)
            {
                int neighbor_row = row + i;
                int neighbor_col = col + j;

                // Zero Padding Logic: Only add to sum if neighbor is inside the image
                if (neighbor_row >= 0 && neighbor_row < n_row && neighbor_col >= 0 && neighbor_col < n_col) 
                    {
                    
                    // A is flattened: index = row * width + col
                    unsigned int pixel = A[neighbor_row * n_col + neighbor_col];
                    // K is 5x5: index = (i+2) * 5 + (j+2)
                    unsigned int filter_val = K[(i + 2) * 5 + (j + 2)];
                    
                    sum += pixel * filter_val;
                }
            }
        }
        B[row * n_col + col] = sum;
    }
}

__global__
void maxpooling_kernel(unsigned int* B, unsigned int* C, int n_row, int n_col)
{
    //B is the input matrix, C is output
    // Calculate global row and column for the output matrix C

    //We will use a max-pool window of 5x5 and a stride size of 1

    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < n_row && col < n_col)
    {
        unsigned int max_val = 0;
        // Loop through the 5x5 window
        for (int m = -2; m <= 2; m++) 
        {
            for (int n = -2; n <= 2; n++) 
            {
                int neighbor_row = row + m;
                int neighbor_col = col + n;
                // Check if the neighbor is within bounds
                if (neighbor_row >= 0 && neighbor_row < n_row && neighbor_col >= 0 && neighbor_col < n_col) 
                {
                    unsigned int val = B[neighbor_row * n_col + neighbor_col];
                    if (val > max_val) 
                    {
                        max_val = val;
                    }
                }
            }
        }
        C[row * n_col + col] = max_val;
    }
}