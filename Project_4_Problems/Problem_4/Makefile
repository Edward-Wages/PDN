
NVCC        = nvcc
NVCC_FLAGS  = -O3 -I/usr/local/cuda/include
LD_FLAGS    = -lcudart -L/usr/local/cuda/lib64
EXE	        = convolution_maxpooling_CUDA
OBJ	        = convolution_maxpooling_CUDA.o support.o

ROWS = 2048
COLS = 2048
INPUT_CSV = mat_input.csv
OUTPUT_CSV = mat_output_prob4.csv
TIME_CSV = time_prob4_CUDA.csv

default: $(EXE)

convolution_maxpooling_CUDA.o: convolution_maxpooling_CUDA.cu kernel.cu support.h
	$(NVCC) -c -o $@ convolution_maxpooling_CUDA.cu $(NVCC_FLAGS)

support.o: support.cu support.h
	$(NVCC) -c -o $@ support.cu $(NVCC_FLAGS)

$(EXE): $(OBJ)
	$(NVCC) $(OBJ) -o $(EXE) $(LD_FLAGS)

run: $(EXE)
	./$(EXE) $(ROWS) $(COLS) $(INPUT_CSV) $(OUTPUT_CSV) $(TIME_CSV)

clean:
	rm -rf *.o $(EXE)

