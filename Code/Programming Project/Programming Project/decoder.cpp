#include "huffman_coding.h"

static ifstream encoded_file;
static ifstream code_table_file;
static ofstream output_file;
static huffman_node** starting_node_table;		//starting node table
static int min_bit_size;						//min size of valid bit code

//read bit code from encoded file
//return: 0 - success; -1 - eof
static int read_bit(unsigned long long* bit_code, int bit_size) {
	static unsigned long long bit_buffer = 0;
	static int buffer_count = 0;
	static int int_bit_size = sizeof(bit_buffer) * 8;
	unsigned char input_byte;

	while (buffer_count < bit_size) {
		encoded_file.read((char *)&input_byte, 1);
		if (encoded_file.eof()) {	//must read first and then check if eof
			return -1;
		}
		bit_buffer = bit_buffer << 8 | input_byte;
		buffer_count += 8;
	}
	*bit_code = (bit_buffer << (int_bit_size - buffer_count)) >> (int_bit_size - bit_size);
	buffer_count -= bit_size;
	return 0;
}

//traverse huffman node to set starting node table
static void traverse_node(huffman_node* node, string string_code) {
	if (string_code.size() == min_bit_size) {
		starting_node_table[string_to_binary(string_code.c_str())] = node;
	} else {
		traverse_node(node->left_child, string_code + "0");
		traverse_node(node->right_child, string_code + "1");
	}
}

//traverse huffman tree to initialize starting node table
static void traverse_tree(huffman_node* root) {
	if (root == NULL) {
		return;
	}
	if (root->left_child != NULL) {
		traverse_node(root->left_child, "0");
	}
	if (root->right_child != NULL) {
		traverse_node(root->right_child, "1");
	}
}

//decode input file
//return: 0 - success; -1 - error
int decoder(char* input_encoded_file_name, char* input_code_table_file_name, char* output_decoded_file_name) {
	huffman_node* huffman_tree;
	huffman_node* current_node;
	unsigned long long bit_code;
	char string_value[SIZE_OF_STRING_VALUE];
	char string_code[SIZE_OF_STRING_CODE];
	int i;

	get_time_interval();
	if (input_encoded_file_name == NULL || input_code_table_file_name == NULL || output_decoded_file_name == NULL) {
		return -1;
	}
	//open input file and check existence
	encoded_file.open(input_encoded_file_name, ios::binary);
	if (!encoded_file) {
		cout << "File \"" << input_encoded_file_name << "\" does not exist. Decoder exits." << endl;
		return -1;
	}
	code_table_file.open(input_code_table_file_name, ios::binary);
	if (!code_table_file) {
		cout << "File \"" << input_code_table_file_name << "\" does not exist. Decoder exits." << endl;
		return -1;
	}

	cout << "Decoding started." << endl;
	min_bit_size = SIZE_OF_STRING_CODE;
	new_huffman_memory(NUM_OF_VALUES);

	//build huffman tree from code table file
	huffman_tree = new_huffman_node();
	while (code_table_file >> string_value >> string_code) {
		current_node = huffman_tree;
		for (i = 0; string_code[i] != '\0'; i++) {
			if (string_code[i] == '0') {
				if (current_node->left_child == NULL) {
					current_node->left_child = new_huffman_node();
				}
				current_node = current_node->left_child;
			} else {
				if (current_node->right_child == NULL) {
					current_node->right_child = new_huffman_node();
				}
				current_node = current_node->right_child;
			}
		}
		copy_string(current_node->string, string_value);
		if (strlen(string_code) < min_bit_size) {
			min_bit_size = strlen(string_code);
		}
	}
	code_table_file.close();

	//build starting node table to provide direct access to possible closest node
	starting_node_table = new huffman_node*[1 << min_bit_size];
	memset(starting_node_table, 0 , (1 << min_bit_size) * sizeof(huffman_node*));
	traverse_tree(huffman_tree);

	//decode and output file
	output_file.open(output_decoded_file_name, ios::binary);
	while (read_bit(&bit_code, min_bit_size) == 0) {	//performance is 30% better than starting at root and reading 1 bit each time
		current_node = starting_node_table[bit_code];	//direct access to node very close to leaf
		if (current_node->left_child != NULL) {
			while (read_bit(&bit_code, 1) == 0) {
				current_node = bit_code == 0 ? current_node->left_child : current_node->right_child;
				if (current_node->left_child == NULL) {
					break;
				}
			}
		}
		output_file << current_node->string << "\x0A";
	}
	/*
	//decode file with huffman tree root as starting point for every time
	current_node = huffman_tree;
	while (read_bit(&bit_code, 1) == 0) {
		current_node = bit_code == 0 ? current_node->left_child : current_node->right_child;
		if (current_node->left_child == NULL) {
			output_file << current_node->string << "\x0A";
			current_node = huffman_tree;
		}
	}
	*/

	output_file.close();
	encoded_file.close();
	delete_huffman_memory();
	delete[] starting_node_table;
	starting_node_table = NULL;
	cout << "Decoding completed. Decoding time (ms): " << get_time_interval() << endl;
	return 0;
}
