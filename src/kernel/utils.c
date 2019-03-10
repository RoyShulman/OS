#include "utils.h"

int memcpy(char* source, char* dest, int num_bytes) {
	if (source == NULL || dest == NULL)	{
		return -1;
	}

	for (int i = 0; i < num_bytes; i++) {
		*(dest + i) = *(source + i);
	}
	return 0;
}

int itoa(int num, char* str) {
	if (str == NULL) {
		return -1;
	}

	int i = 0;
	char is_negative = 0;

	if (num == 0) {
		str[i] = '0';
		str[++i] = 0;
		return 0;
	}

	if (num < 0) {
		is_negative = 1;
		num = -num;
	}

	// Process individual digits
	while (num != 0) {
		int dig = num % 10;
		str[i++] = dig + '0';
		num /= 10;
	}

	if (is_negative) {
		str[i++] = '-';
	}

	// Add the null terminator
	str[i] = 0;

	if (reverse(str) != 0) {
		return -1;
	}

	return 0;
}

int reverse(char* string) {
	if (string == NULL) {
		return -1;
	}
	int len = strlen(string);
	for (int i = 0; i < (len / 2); i++) {
		char temp;
		temp = string[i];
		string[i] = string[len - i - 1];
		string[len - 1 - i] = temp;
	}
	return 0;
}

int strlen(char* string) {
	if (string == NULL) {
		return -1;
	}
	int len = 0;
	int i = 0;
	while(string[i++] != 0) {
		len++;
	}
	return len;
}
