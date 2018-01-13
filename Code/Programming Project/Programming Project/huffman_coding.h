#include <iostream>
#include <fstream>
#include <string>
#include <string.h>		//used in linux
#include <time.h>

using namespace std;

#define round_up_4(i) ((3 + (unsigned int)(i)) & (~3))		//round up to multiple of 4

#define NUM_OF_VALUES 1000000								//number of possible values, range: 0 ~ 999999
#define NUM_OF_DIGITS 6										//number of input value digits
#define SIZE_OF_STRING_VALUE round_up_4(NUM_OF_DIGITS + 1)	//size of string format value, example: "338827"
#define SIZE_OF_STRING_CODE 64								//size of string format code, example: "00001010111000001011"; give more space in case not enough

//code table entry
struct code_entry {
	unsigned long long bit_code;		//bit code, example: 010001; use long long to ensure buffer has 64 bits; possible for 100 million records to have bit code longer than 32 bit
	int bit_size;						//size of bit code, example: 6
};

//huffman tree node
struct huffman_node {
	int numeric;						//numeric value, used by encoder
	char string[SIZE_OF_STRING_VALUE];	//string value, used by decoder
	huffman_node* left_child;			//pointer to left child node
	huffman_node* right_child;			//pointer to right child node
};

//binary heap entry
struct binary_entry {
	int key;							//key
	void* value_pointer;				//pointer to value
};

//4-way heap entry
struct four_way_entry{
	int key;							//key
	void* value_pointer;				//pointer to value
};

//pairing heap node
struct pairing_node {
	int key;							//key
	void* value_pointer;				//pointer to value
	pairing_node* left_sibling;			//pointer to left sibling or parent
	pairing_node* right_sibling;		//pointer to right sibling
	pairing_node* child;				//pointer to first child
};

//encoder.cpp
extern int encoder(char* input_source_file_name, char* output_encoded_file_name, char* output_code_table_file_name);

//decoder.cpp
extern int decoder(char* input_encoded_file_name, char* input_code_table_file_name, char* output_decoded_file_name);

//binary_heap.cpp
extern huffman_node* build_huffman_tree_using_binary_heap(int* frequency_table, int size_of_frequency_table);

//four_way_heap.cpp
extern huffman_node* build_huffman_tree_using_four_way_heap(int* frequency_table, int size_of_frequency_table);

//pairing_heap.cpp
extern huffman_node* build_huffman_tree_using_pairing_heap(int* frequency_table, int size_of_frequency_table);

//huffman.cpp
extern void new_huffman_memory(int num_of_node);
extern void delete_huffman_memory(void);
extern huffman_node* new_huffman_node(void);

//utility.cpp
extern int string_to_int(char* s);
extern unsigned long long string_to_binary(const char* string);
extern void copy_string(char* destination, char* source);
extern clock_t get_time_interval(void);

//benchmark.cpp
extern void benchmark_huffman_tree_build_time(char* input_source_file_name);
