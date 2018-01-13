#include "huffman_coding.h"

//convert string to decimal int
int string_to_int(char* string) {
	int i, sign, value;

	value = 0;
	switch (string[0]) {
	case '+':
		i = 1;
		sign = 1;
		break;
	case '-':
		i = 1;
		sign = -1;
		break;
	default:
		i = 0;
		sign = 1;
	}
	while (string[i] != '\0') {
		value = value * 10 + string[i] - '0';
		i++;
	}
	value *= sign;
	return value;
}

//convert string to numeric with base 2, input string must have only '0' and '1'
unsigned long long string_to_binary(const char* string) {
	unsigned long long value;

	value = 0;
	for (int i = 0; string[i] != '\0'; i++) {
		value = value << 1 | (string[i] - '0');
	}
	return value;
}

//copy string
void copy_string(char* destination, char* source) {
	int i = 0;
	while (source[i] != '\0') {
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

//get time interval in milliseconds since last calling of this function
clock_t get_time_interval(void) {
	static clock_t start_time = clock();
	clock_t time_interval;
	
	time_interval = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
	start_time = clock();
	return time_interval;
}
