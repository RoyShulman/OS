#include <drivers/screen.h>

#include "string.h"

#include "utils.h"

void panic(char* msg, char* file, uint32_t line) {
	print(msg);
	print("\n");
	print("Thrown from file: ");
	print(file);
	print(" from line: ");
	char str_line[256];
	itoa(line, str_line);
	print(str_line);
	print("\n");

	while(1) {}
}
