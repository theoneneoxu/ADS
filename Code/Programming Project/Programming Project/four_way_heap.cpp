#include "huffman_coding.h"

#define get_parent_index(i) ((i >> 2) + 2)
#define get_child_base_index(i) ((i - 2) << 2)
/*
#define get_child_index(i, c) (((i - 2) << 2) + c)
#define get_child_1_index(i) ((i - 2) << 2)
#define get_child_2_index(i) (((i - 2) << 2) + 1)
#define get_child_3_index(i) (((i - 2) << 2) + 2)
#define get_child_4_index(i) (((i - 2) << 2) + 3)
*/

#define STARTING_INDEX 3					//starting index is set to 3 to make all children of a specific node lying at the same cache line

static four_way_entry* four_way_heap;		//min four way heap
static int heap_index = STARTING_INDEX;		//pointing to unused slot for next entry into heap

//insert into four way heap
//return: 0 - success
static int insert(int key, void* value_pointer) {
	int current_index, parent_index;

	current_index = heap_index++;
	four_way_heap[current_index].key = key;
	four_way_heap[current_index].value_pointer = value_pointer;
	//compare with parent key and swap
	parent_index = get_parent_index(current_index);
	while (parent_index >= STARTING_INDEX && key < four_way_heap[parent_index].key) {
		//swap current and parent
		four_way_heap[current_index] = four_way_heap[parent_index];
		four_way_heap[parent_index].key = key;
		four_way_heap[parent_index].value_pointer = value_pointer;
		//calculate new index
		current_index = parent_index;
		parent_index = get_parent_index(current_index);
	}
	return 0;
}

//huffman tree construction time is 230ms when using this version
//remove and get min from four way heap
//return: 0 - success; -1 - heap is empty
static int remove_min(four_way_entry* output) {
	four_way_entry temp;
	int current_index, child_base_index;
	int child_min_key, child_id;
	int i, result;

	result = -1;
	if (heap_index > STARTING_INDEX) {		//heap is not empty
		current_index = STARTING_INDEX;
		//output min
		*output = four_way_heap[current_index];
		result = 0;
		//process new root
		four_way_heap[current_index] = four_way_heap[--heap_index];		//put last node at root
		child_base_index = get_child_base_index(current_index);
		child_min_key = four_way_heap[current_index].key;
		for (i = 0; i < 4; i++) {
			if (child_base_index + i >= heap_index) {
				break;
			}
			if (four_way_heap[child_base_index + i].key < child_min_key) {
				child_min_key = four_way_heap[child_base_index + i].key;
				child_id = i;
			}
		}
		while (child_min_key < four_way_heap[current_index].key) {
			//swap
			temp = four_way_heap[current_index];
			four_way_heap[current_index] = four_way_heap[child_base_index + child_id];
			four_way_heap[child_base_index + child_id] = temp;
			//calculate index and key
			current_index = child_base_index + child_id;
			child_base_index = get_child_base_index(current_index);
			child_min_key = four_way_heap[current_index].key;
			for (i = 0; i < 4; i++) {
				if (child_base_index + i >= heap_index) {
					break;
				}
				if (four_way_heap[child_base_index + i].key < child_min_key) {
					child_min_key = four_way_heap[child_base_index + i].key;
					child_id = i;
				}
			}
		}
	}
	return result;
}
/*
//this version is not the fastest probably due to writing value to local variable is slow
//huffman tree construction time is 280ms when using this version
static int remove_min2(four_way_entry* output) {
	four_way_entry temp;
	int current_index, child_base_index, child_index;
	int current_key, child_min_key, child_key, child_id;
	int i, result;

	result = -1;
	if (heap_index > STARTING_INDEX) {		//heap is not empty
		current_index = STARTING_INDEX;
		//output min
		*output = four_way_heap[current_index];
		result = 0;
		//process new root
		four_way_heap[current_index] = four_way_heap[--heap_index];		//put last node at root
		child_base_index = get_child_base_index(current_index);
		current_key = four_way_heap[current_index].key;
		child_min_key = current_key;
		for (i = 0; i < 4; i++) {
			child_index = child_base_index + i;
			if (child_index >= heap_index) {
				break;
			}
			child_key = four_way_heap[child_index].key;
			if (child_key < child_min_key) {
				child_min_key = child_key;
				child_id = i;
			}
		}
		while (child_min_key < current_key) {
			child_index = child_base_index + child_id;
			//swap
			temp = four_way_heap[current_index];
			four_way_heap[current_index] = four_way_heap[child_index];
			four_way_heap[child_index] = temp;
			//calculate index and key
			current_index = child_index;
			child_base_index = get_child_base_index(current_index);
			current_key = four_way_heap[current_index].key;
			child_min_key = current_key;
			for (i = 0; i < 4; i++) {
				child_index = child_base_index + i;
				if (child_index >= heap_index) {
					break;
				}
				child_key = four_way_heap[child_index].key;
				if (child_key < child_min_key) {
					child_min_key = child_key;
					child_id = i;
				}
			}
		}
	}
	return result;
}

//this version is not the fastest probably due to too many calculations of each child index and too many access to child_index array
//huffman tree construction time is 330ms when using this version
static int remove_min1(four_way_entry* output) {
	four_way_entry temp;
	int current_index, child_1_index, child_2_index, child_3_index, child_4_index, child_index[4];
	int child_min_key, child_id;
	int i, result;

	result = -1;
	if (heap_index > STARTING_INDEX) {		//heap is not empty
		current_index = STARTING_INDEX;
		//output min
		*output = four_way_heap[current_index];
		result = 0;
		//process new root
		four_way_heap[current_index] = four_way_heap[--heap_index];
		for (i = 0; i < 4; i++) {
			child_index[i] = get_child_index(current_index, i);
		}
		//calculate child min key
		child_min_key = four_way_heap[current_index].key;
		for (i = 0; i < 4; i++) {
			if (child_index[i] >= heap_index) {
				break;
			}
			if (four_way_heap[child_index[i]].key < child_min_key) {
				child_min_key = four_way_heap[child_index[i]].key;
				child_id = i;
			}
		}
		while (child_min_key < four_way_heap[current_index].key) {
			//swap
			temp = four_way_heap[current_index];
			four_way_heap[current_index] = four_way_heap[child_index[child_id]];
			four_way_heap[child_index[child_id]] = temp;
			//calculate index
			current_index = child_index[child_id];
			for (i = 0; i < 4; i++) {
				child_index[i] = get_child_index(current_index, i);
			}
			//calculate child min key
			child_min_key = four_way_heap[current_index].key;
			for (i = 0; i < 4; i++) {
				if (child_index[i] >= heap_index) {
					break;
				}
				if (four_way_heap[child_index[i]].key < child_min_key) {
					child_min_key = four_way_heap[child_index[i]].key;
					child_id = i;
				}
			}
		}
	}
	return result;
}

//this version is not the fastest probably due to too many logic in while loop
//huffman tree construction time is 480ms when using this version
static int remove_min0(four_way_entry* output) {
	four_way_entry temp;
	int current_index, child_index[4];
	int i, result;

	result = -1;
	if (heap_index > STARTING_INDEX) {		//heap is not empty
		current_index = STARTING_INDEX;
		//output min
		*output = four_way_heap[current_index];
		result = 0;
		//process new root
		four_way_heap[current_index] = four_way_heap[--heap_index];
		for (i = 0; i < 4; i++) {
			child_index[i] = get_child_index(current_index, i);
		}
		while ((child_index[0] < heap_index && four_way_heap[child_index[0]].key < four_way_heap[current_index].key) || 
			   (child_index[1] < heap_index && four_way_heap[child_index[1]].key < four_way_heap[current_index].key) || 
			   (child_index[2] < heap_index && four_way_heap[child_index[2]].key < four_way_heap[current_index].key) || 
			   (child_index[3] < heap_index && four_way_heap[child_index[3]].key < four_way_heap[current_index].key)) {
			int min_key = four_way_heap[child_index[0]].key;
			int child_id = 0;
			for (i = 1; i < 4; i++) {
				if (child_index[i] < heap_index && four_way_heap[child_index[i]].key < min_key) {
					min_key = four_way_heap[child_index[i]].key;
					child_id = i;
				}
			}
			temp = four_way_heap[current_index];
			four_way_heap[current_index] = four_way_heap[child_index[child_id]];
			four_way_heap[child_index[child_id]] = temp;
			//calculate new index
			current_index = child_index[child_id];
			for (i = 0; i < 4; i++) {
				child_index[i] = get_child_index(current_index, i);
			}
		}
	}
	return result;
}
*/

//build huffman tree using four way heap
huffman_node* build_huffman_tree_using_four_way_heap(int* frequency_table, int size_of_frequency_table) {
	huffman_node* node;
	four_way_entry entry1, entry2;
	int result;

	if (frequency_table == NULL || size_of_frequency_table < 0) {
		return NULL;
	}
	//initialize four way heap
	four_way_heap = new four_way_entry[size_of_frequency_table + STARTING_INDEX];	//max size is more than size_of_frequency_table due to starting index is not 0
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
	delete[] four_way_heap;
	four_way_heap = NULL;
	return node;
}
