
__global__
void convolution_kernel(unsigned int* d_mat_input, unsigned int* d_mat_kernel, unsigned int* d_mat_output, unsigned int n_row, unsigned int n_col) {

    // Calculate thread index
    unsigned int row = blockDim.y * blockIdx.y + threadIdx.y;
    unsigned int col = blockDim.x * blockIdx.x + threadIdx.x;

    if(row < n_row && col < n_col)
    {
        unsigned int sum = 0;

        for(int i = 0; i < 5; i ++)
        {
            for(int j = 0; j < 5; j++)
            {
                int r = (int)row + i - 2; //Centering the kernel on the current pixel
                int c = (int)col + j - 2;

                //boundary check
                if(r >= 0 && r < n_row && c >= 0 && c < n_col)
                {
                    unsigned int pixel_value = d_mat_input[r * n_col + c];
                    unsigned int kernel_value = d_mat_kernel[i * 5 + j];
                    sum += pixel_value * kernel_value;
                }
            }
        }
        d_mat_output[row * n_col + col] = sum;
    }
}