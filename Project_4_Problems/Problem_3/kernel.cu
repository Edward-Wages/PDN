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