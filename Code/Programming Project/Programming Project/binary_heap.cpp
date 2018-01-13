#include "huffman_coding.h"

#define get_parent_index(i) (i >> 1)
#define get_left_child_index(i) (i << 1)
#define get_right_child_index(i) ((i << 1) + 1)

#define STARTING_INDEX 1					//starting index is set to 1 to make child index easy to calculate

static binary_entry* binary_heap;			//min binary heap
static int heap_index = STARTING_INDEX;		//pointing to unused slot for next entry into heap

//insert into binary heap
//return: 0 - success
static int insert(int key, void* value_pointer) {
	int current_index, parent_index;

	current_index = heap_index++;
	binary_heap[current_index].key = key;
	binary_heap[current_index].value_pointer = value_pointer;
	//compare with parent key and swap
	parent_index = get_parent_index(current_index);
	while (parent_index >= STARTING_INDEX && key < binary_heap[parent_index].key) {
		//swap current and parent
		binary_heap[current_index] = binary_heap[parent_index];
		binary_heap[parent_index].key = key;
		binary_heap[parent_index].value_pointer = value_pointer;
		//calculate new index
		current_index = parent_index;
		parent_index = get_parent_index(current_index);
	}
	return 0;
}

//remove and get min from binary heap
//return: 0 - success; -1 - heap is empty
static int remove_min(binary_entry* output) {
	binary_entry temp;
	int current_index, left_child_index, right_child_index;
	int result;

	result = -1;
	if (heap_index > STARTING_INDEX) {		//heap is not empty
		current_index = STARTING_INDEX;
		//output min
		*output = binary_heap[current_index];
		result = 0;
		//process new root
		binary_heap[current_index] = binary_heap[--heap_index];		//put last node at root
		left_child_index = get_left_child_index(current_index);
		right_child_index = get_right_child_index(current_index);
		while (left_child_index < heap_index && (binary_heap[left_child_index].key < binary_heap[current_index].key || (right_child_index < heap_index && binary_heap[right_child_index].key < binary_heap[current_index].key))) {	//if left child does not exist then right child does not exist
			if (right_child_index < heap_index && binary_heap[right_child_index].key < binary_heap[left_child_index].key) {		//if right child exists and its key is less than left child's key, then swap with right child
				temp = binary_heap[current_index];
				binary_heap[current_index] = binary_heap[right_child_index];
				binary_heap[right_child_index] = temp;
				//calculate new index
				current_index = right_child_index;
			} else {	//swap with left child
				temp = binary_heap[current_index];
				binary_heap[current_index] = binary_heap[left_child_index];
				binary_heap[left_child_index] = temp;
				//calculate new index
				current_index = left_child_index;
			}
			left_child_index = get_left_child_index(current_index);
			right_child_index = get_right_child_index(current_index);
		}
	}
	return result;
}

//build huffman tree using binary heap
huffman_node* build_huffman_tree_using_binary_heap(int* frequency_table, int size_of_frequency_table) {
	huffman_node* node;
	binary_entry entry1, entry2;
	int result;

	if (frequency_table == NULL || size_of_frequency_table < 0) {
		return NULL;
	}
	//initialize binary heap
	binary_heap = new binary_entry[size_of_frequency_table + STARTING_INDEX];	//max size is more than size_of_frequency_table due to starting index is not 0
	for (int i = 0; i < size_of_frequency_table; i++) {
		if (frequency_table[i] > 0) {
			node = new_huffman_node();
			node->numeric = i;
			insert(frequency_table[i], node);
		}
	}
	//build huffman tree using greedy algorithm
	result = remove_min(&entry1);
	while (remove_min(&entry2) == 0) {	//combine while two entries exist
		node = new_huffman_node();
		node->left_child = (huffman_node*)(entry1.value_pointer);
		node->right_child = (huffman_node*)(entry2.value_pointer);
		insert(entry1.key + entry2.key, node);
		remove_min(&entry1);
	}
	node = result == 0 ? (huffman_node*)(entry1.value_pointer) : NULL;
	delete[] binary_heap;
	binary_heap = NULL;
	return node;
}
