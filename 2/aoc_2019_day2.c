#include <stdio.h>
#include <stdlib.h>



/*
	load_program reads in a program from plain text file (Integers separated by commas).
	Parses the file and returns a array of the comma-seperated integers.
*/
int *load_program(char *pathname, int *num_integers) {
	FILE *file;
	
	// raw string with integers separated by comma
	char *program_string;
	// Array with the integers from the string
	int *program;

	long fsize;

	if((file = fopen(pathname, "r")) == NULL) {
		printf("Error open file\n");
	}

	// Find size of file
	fseek(file, 0L, SEEK_END);
	fsize = ftell(file);
	rewind(file);
	// Allocate memory for the string
	program_string = (char*)malloc((size_t) fsize + 1);

	fgets(program_string, fsize + 1,file);
	fclose(file);

	printf("Before execution:\n%s\n", program_string);


	// Get number of integers
	*num_integers = 1;
	for(char *c = program_string; *c != '\0'; c++) {
		if(*c == ',')
			*num_integers += 1;
	}

	// Allocate memory for the program array
	program = (int*)malloc(*num_integers * sizeof(int));

	// Parse program_string for integers
	// The integer will be maximum three digits
	char int_string[4];
	int_string[0] = '\0';
	int i = 0;
	int digit_index = 0;
	for(char *c = program_string; ;c++) {
		if(*c == ',' || *c == '\0') {
			// Fetched a new integer, that is stored in int_string.
			int_string[digit_index] = '\0';

			program[i++] = atoi(int_string);
			int_string[0] = '\0';
			digit_index = 0;
		} else {
			int_string[digit_index++] = *c;
		}
		if(*c == '\0')
			break;
	}


	free(program_string);
	return program;
}


struct opcode {
	int ins;
	int a;
	int b;
	int c;
};


/*
	Fetches the next opcode and increments the program counter.
*/
struct opcode fetch_opcode(int *program, int *PC) {
	struct opcode opcode;
	opcode.ins = program[*PC];
	opcode.a = program[*PC + 1];
	opcode.b = program[*PC + 2];
	opcode.c = program[*PC + 3];
	*PC += 4;
	return opcode;
};

int main() {
	
	// Load program
	int num_integers;
	int *program = load_program("./day_2_input.txt", &num_integers);
	
	// Execute program
	int PC = 0;  // Program counter
	int run_program = 1;
	// Helper variables used for the instructions
	int pos_in1, pos_in2, pos_out;
	while(run_program) {
		
		struct opcode opcode = fetch_opcode(program, &PC);
		switch(opcode.ins) {
			case 1:	
				// OPCODE 1	- ADD
				program[opcode.c]  = program[opcode.a] + program[opcode.b];
				break;
			case 2:
				// OPCODE 2 - MULTIPLY
				program[opcode.c] = program[opcode.a] * program[opcode.b];
				break;
			case 99:
				// HALT
				run_program = 0;
				break;
			default:
				break;
		}
		
	}

	// Print the result
	printf("After execution:\n");
	for(int i = 0; i < num_integers; i++) {
		printf("%d ", program[i]);
	}
	printf("\n");

	free(program);
	return -1;
}
