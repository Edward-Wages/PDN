
// To generate random value
__device__
unsigned int random_kernel(unsigned int seed, unsigned int index);


/* Nonce Kernel ----------------------------------
*       Generates an array of random nonce values.
*/
__global__
void nonce_kernel(unsigned int* nonce_array, unsigned int array_size, unsigned int mod, unsigned int seed) {

    // Calculate thread rank
    unsigned int index = blockDim.x * blockIdx.x + threadIdx.x;

    // Generate random nonce values for every item in the array
    if (index < array_size) {
        unsigned int rand = random_kernel(seed, index);
        nonce_array[index] = rand % mod;
    }

} // End Nonce Kernel //



/* Random Kernel ----------------
*       Generates a random value.
*/
__device__
unsigned int random_kernel(unsigned int seed, unsigned int index) {

    curandState_t state;
    curand_init(
        seed,  // the seed can be the same for every thread and is set to be the time
        index, // the sequence number should be different for every thread
        0,     // an offset into the random number sequence at which to begin sampling
        &state // the random state object
    );

    // generate a random number
    return (unsigned int)(curand(&state));

} // End Random Kernel //

__global__ void min_nonce_kernel(unsigned int* g_hash, unsigned int* g_nonce, unsigned int* res_hash, unsigned int* res_nonce, unsigned int n)
{
    __shared__ unsigned int s_hash[1024];
    __shared__ unsigned int s_nonce[1024];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) 
    {
        s_hash[tid] = g_hash[i];
        s_nonce[tid] = g_nonce[i];
    } 
    else 
    {
        s_hash[tid] = 0xFFFFFFFF; // Set to max value
        s_nonce[tid] = 0;
    }
    __syncthreads();

    for(unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s) 
        {
            if (s_hash[tid + s] < s_hash[tid]) 
            {
                s_hash[tid] = s_hash[tid + s];
                s_nonce[tid] = s_nonce[tid + s];
            }
        }
        __syncthreads();
    }

    if(tid == 0)
    {
        unsigned int old_hash = atomicMin(res_hash, s_hash[0]);
        if (old_hash > s_hash[0])
        {
            *res_nonce = s_nonce[0];
        }
    }
}
