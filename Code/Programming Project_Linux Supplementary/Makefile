CC = g++
CFLAGS = -std=c++11
LFLAGS = #not used
BASIC_OBJECTS = utility.o huffman.o
HEAP_OBJECTS = binary_heap.o four_way_heap.o pairing_heap.o
ENCODER_OBJECTS = $(BASIC_OBJECTS) $(HEAP_OBJECTS) encoder.o main_encoder.o
DECODER_OBJECTS = $(BASIC_OBJECTS) decoder.o main_decoder.o
BENCHMARK_OBJECTS = $(BASIC_OBJECTS) $(HEAP_OBJECTS) benchmark.o main_benchmark.o
AUTORUN_OBJECTS = $(BASIC_OBJECTS) $(HEAP_OBJECTS) encoder.o decoder.o benchmark.o main.o

make: compile encoder decoder benchmark autorun

encoder: $(ENCODER_OBJECTS)
	$(CC) $(LFLAGS) $(ENCODER_OBJECTS) -o encoder

decoder: $(DECODER_OBJECTS)
	$(CC) $(LFLAGS) $(DECODER_OBJECTS) -o decoder

benchmark: $(BENCHMARK_OBJECTS)
	$(CC) $(LFLAGS) $(BENCHMARK_OBJECTS) -o benchmark

autorun: $(AUTORUN_OBJECTS)
	$(CC) $(LFLAGS) $(AUTORUN_OBJECTS) -o autorun

compile: *.cpp
	$(CC) $(CFLAGS) -c *.cpp

clean:
	-@rm *.o decoder encoder benchmark autorun
