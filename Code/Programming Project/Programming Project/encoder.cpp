#include "huffman_coding.h"

static ifstream input_file;
static ofstream output_file;
static code_entry* code_table;

//write encoded bit to output file
static void write_bit(unsigned long long bit_code, int bit_size) {
	static unsigned long long bit_buffer = 0;		
	static int buffer_count = 0;
	unsigned char output_byte;

	bit_buffer = bit_buffer << bit_size | bit_code;
	buffer_count += bit_size;
	while (buffer_count >= 8) {
		output_byte = (unsigned char)(bit_buffer >> (buffer_count - 8));
		output_file << output_byte;
		buffer_count -= 8;
	}
}

//traverse huffman node to set and output code
static void traverse_node(huffman_node* node, string string_code) {
	if (node->left_child == NULL) {		//current node is leaf
		//set code table
		code_table[node->numeric].bit_code = string_to_binary(string_code.c_str());
		code_table[node->numeric].bit_size = string_code.size();
		//output to file
		output_file << to_string(node->numeric) + " " + string_code + "\x0A";
	} else {	//traverse two children
		traverse_node(node->left_child, string_code + "0");
		traverse_node(node->right_child, string_code + "1");
	}
}

//traverse huffman tree to initialize and output code table
static void traverse_tree(huffman_node* root) {
	if (root == NULL) {
		return;
	}
	if (root->left_child == NULL) {
		traverse_node(root, "0");
	} else {
		traverse_node(root->left_child, "0");
		traverse_node(root->right_child, "1");
	}
}

//build huffman tree
static huffman_node* build_huffman_tree(int* frequency_table, int size_of_frequency_table) {
	switch (1) {
	case 1:
		cout << "Building huffman tree using binary heap." << endl;
		//huffman tree construction time is 220ms (not including time used to allocate memory for huffman nodes)
		return build_huffman_tree_using_binary_heap(frequency_table, size_of_frequency_table);
	case 2:
		cout << "Building huffman tree using four way heap." << endl;
		//huffman tree construction time is 230ms (not including time used to allocate memory for huffman nodes)
		return build_huffman_tree_using_four_way_heap(frequency_table, size_of_frequency_table);
	case 3:
		cout << "Building huffman tree using pairing heap." << endl;
		//huffman tree construction time is 470ms (not including time used to allocate memory for huffman nodes)
		return build_huffman_tree_using_pairing_heap(frequency_table, size_of_frequency_table);
	default:
		return NULL;
	}
}

//encode input file
//return: 0 - success; -1 - error
int encoder(char* input_source_file_name, char* output_encoded_file_name, char* output_code_table_file_name) {
	huffman_node* huffman_tree;
	char input_string[SIZE_OF_STRING_VALUE];
	int* frequency_table;
	int num_of_values, code_index;

	get_time_interval();
	if (input_source_file_name == NULL || output_encoded_file_name == NULL || output_code_table_file_name == NULL) {
		return -1;
	}
	//open input file and check existence
	input_file.open(input_source_file_name, ios::binary);
	if (!input_file) {
		cout << "File \"" << input_source_file_name << "\" does not exist. Encoder exits." << endl;
		return -1;
	}

	cout << "Encoding started." << endl;
	num_of_values = NUM_OF_VALUES;
	frequency_table = new int[num_of_values];
	memset(frequency_table, 0, num_of_values * sizeof(int));
	//calculate frequency
	while (input_file >> input_string) {
		frequency_table[string_to_int(input_string)]++;		//self-made string_to_int is faster than stoi or fscanf with %d
	}
	/*
	FILE* input_file2;
	int value;
	input_file2 = fopen(input_file_name, "rb");
	while (fscanf(input_file2, "%d", &value) == 1) {
		frequency_table[value]++;
	}
	*/

	//build huffman tree
	new_huffman_memory(num_of_values);		//allocating a whole chunk of memory for huffman tree uses less memory than newing node millions of times
	huffman_tree = build_huffman_tree(frequency_table, num_of_values);
	delete[] frequency_table;
	frequency_table = NULL;

	//build and output code table
	code_table = new code_entry[num_of_values];
	output_file.open(output_code_table_file_name, ios::binary);	//writing in binary mode to ensure output '\x0A' is exactly the same
	traverse_tree(huffman_tree);						//traverse huffman tree to initialize and output code table
	output_file.close();
	delete_huffman_memory();

	//encode and output source file
	input_file.clear();						//two lines work together to set input file stream to beginning
	input_file.seekg(0, ios::beg);
	output_file.open(output_encoded_file_name, ios::binary);
	while (input_file >> input_string) {
		code_index = string_to_int(input_string);
		write_bit(code_table[code_index].bit_code, code_table[code_index].bit_size);
	}

	input_file.close();
	output_file.close();
	delete[] code_table;
	code_table = NULL;
	cout << "Encoding completed. Encoding time (ms): " << get_time_interval() << endl;
	return 0;
}
