
__global__
void convolution_kernel(unsigned int* d_mat_input, unsigned int* d_mat_kernel, unsigned int* d_mat_output, unsigned int n_row, unsigned int n_col) 
{
    //Redo - math was bad
    /* See this example of doing it serially:
    // Performing convolution
	// Take a look at slides about the blurring example - Blur size = 2, it's a 5x5 filter
    for(int i=0; i<n_row; i++)
        for (int j=0; j<n_col; j++)
        {
            int sum_val = 0;

            for (int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE +1; ++blurRow)
            {
                for(int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE+1; ++blurCol)
                {
                    int curRow = i + blurRow;
                    int curCol = j + blurCol;

                    int i_row = blurRow + BLUR_SIZE;
                    int i_col = blurCol + BLUR_SIZE;

                    if( curRow > -1 && curRow < n_row && curCol > -1 && curCol < n_col)
                    {
                        sum_val += inputMatrix_h[curRow*n_col + curCol]*filterMatrix_h[i_row*5 + i_col]; 
                    }
                }
            }

            outputMatrix_h[i*n_col+j] = sum_val;

        }
    */




}