#include "huffman_coding.h"

void benchmark_huffman_tree_build_time(char* input_source_file_name) {
	ifstream input_file;
	char input_string[SIZE_OF_STRING_VALUE];
	int* frequency_table;
	int num_of_values, loop_times;
	bool use_input_file;

	if (input_source_file_name == NULL) {
		use_input_file = false;
	} else {
		input_file.open(input_source_file_name, ios::binary);
		if (!input_file) {
			cout << "File \"" << input_source_file_name << "\" does not exist. Benchmark exits." << endl;
			return;
		}
		use_input_file = true;
	}

	cout << "Benchmark started." << endl;
	loop_times = 10;
	num_of_values = NUM_OF_VALUES;
	frequency_table = new int[num_of_values];
	memset(frequency_table, 0, num_of_values * sizeof(int));

	if (use_input_file) {
		while (input_file >> input_string) {
			frequency_table[string_to_int(input_string)]++;
		}
	} else {
		cout << "Generating random frequency data." << endl;
		for (int i = 0; i < num_of_values; i++) {
			frequency_table[i] = (i * 31) % 83;
		}
	}
	
	get_time_interval();
	for (int i = 0; i < loop_times; i++) {
		new_huffman_memory(num_of_values);
		build_huffman_tree_using_binary_heap(frequency_table, num_of_values);
		delete_huffman_memory();
	}
	cout << "Huffman tree average construction time using binary heap (ms): " << get_time_interval() / loop_times << endl;

	for (int i = 0; i < loop_times; i++) {
		new_huffman_memory(num_of_values);
		build_huffman_tree_using_four_way_heap(frequency_table, num_of_values);
		delete_huffman_memory();
	}
	cout << "Huffman tree average construction time using four way heap (ms): " << get_time_interval() / loop_times << endl;

	for (int i = 0; i < loop_times; i++) {
		new_huffman_memory(num_of_values);
		build_huffman_tree_using_pairing_heap(frequency_table, num_of_values);
		delete_huffman_memory();
	}
	cout << "Huffman tree average construction time using pairing heap (ms): " << get_time_interval() / loop_times << endl;

	delete[] frequency_table;
	frequency_table = NULL;
	cout << "Benchmark completed." << endl;
}
