#include "huffman_coding.h"

int main(int argc, char* argv[]) {
	if (argc > 2) {
		cout << "Incorrect arguments. Command must be like: ./benchmark [input_file_name]" << endl;
		return 0;
	}

	//benchmark
	if (argc == 2) {
		benchmark_huffman_tree_build_time(argv[1]);
	}
	else {
		benchmark_huffman_tree_build_time(NULL);
	}

	return 0;
}
