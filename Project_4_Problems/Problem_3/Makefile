NVCC        = nvcc
NVCC_FLAGS  = -O3 -I/usr/local/cuda/include
LD_FLAGS    = -lcudart -L/usr/local/cuda/lib64
EXE         = convolution_CUDA
OBJ         = convolution_CUDA.o support.o

# --- Problem 3 Parameters ---
# Change these values based on your assignment requirements
ROWS        = 2048
COLS        = 2048
INPUT_CSV   = mat_input.csv
OUTPUT_CSV  = mat_output_prob3.csv
TIME_CSV    = time_prob3_CUDA.csv

default: $(EXE)

# Note: Ensure kernel.cu is the correct filename for Problem 3
convolution_CUDA.o: convolution_CUDA.cu kernel.cu support.h
	$(NVCC) -c -o $@ convolution_CUDA.cu $(NVCC_FLAGS)

support.o: support.cu support.h
	$(NVCC) -c -o $@ support.cu $(NVCC_FLAGS)

$(EXE): $(OBJ)
	$(NVCC) $(OBJ) -o $(EXE) $(LD_FLAGS)


run: $(EXE)
	./$(EXE) $(ROWS) $(COLS) $(INPUT_CSV) $(OUTPUT_CSV) $(TIME_CSV)

clean:
	rm -rf *.o $(EXE)