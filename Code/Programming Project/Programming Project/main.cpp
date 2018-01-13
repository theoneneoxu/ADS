#include "huffman_coding.h"

int main(void) {
	//benchmark
	switch (1) {
	case 1:
		benchmark_huffman_tree_build_time((char*)"sample_input_large.txt");
		break;
	case 2:
		benchmark_huffman_tree_build_time(NULL);
		break;
	default:
		break;
	}

	//encoding
	switch (1) {
	case 1:
		encoder((char*)"sample_input_large.txt", (char*)"encoded.bin", (char*)"code_table.txt");
		break;
	case 2:
		encoder((char*)"sample_input_small.txt", (char*)"encoded.bin", (char*)"code_table.txt");
		break;
	default:
		break;
	}

	//decoding
	switch (1) {
	case 1:
		decoder((char*)"encoded.bin", (char*)"code_table.txt", (char*)"decoded.txt");
		break;
	default:
		break;
	}

	cout << "Processing completed." << endl;
	getchar();
	return 0;
}
