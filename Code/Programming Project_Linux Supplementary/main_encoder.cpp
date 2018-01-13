#include "huffman_coding.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Incorrect arguments. Command must be like: ./encoder <input_file_name>" << endl;
		return 0;
	}

	//encoding
	encoder(argv[1], (char*)"encoded.bin", (char*)"code_table.txt");

	return 0;
}
