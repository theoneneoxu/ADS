#include "huffman_coding.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Incorrect arguments. Command must be like: ./decoder <encoded_file_name> <code_table_file_name>" << endl;
		return 0;
	}

	//decoding
	decoder(argv[1], argv[2], (char*)"decoded.txt");

	return 0;
}
