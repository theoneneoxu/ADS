#include "huffman_coding.h"

static huffman_node* huffman_memory;
static int huffman_index;					//pointing to unused slot for next new node

void new_huffman_memory(int num_of_leaves) {
	int num_of_nodes = num_of_leaves * 2;							//actual max num_of_nodes = num_of_leaves * 2 - 1
	huffman_memory = new huffman_node[num_of_nodes];				//allocating a whole chunk of memory for huffman tree uses less memory than newing node millions of times
	memset(huffman_memory, 0, num_of_nodes * sizeof(huffman_node));
	huffman_index = 0;
}

void delete_huffman_memory(void) {
	delete[] huffman_memory;
	huffman_memory = NULL;
	huffman_index = 0;
}

huffman_node* new_huffman_node(void) {
	return huffman_memory + huffman_index++;
}
