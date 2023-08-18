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
	char int_string[5];
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

/*
	Changed name of struct from day2. 
	The struct is a instruction, that has a opcode and operands.
*/
struct Instruction {
	int opcode;
	int a;
	int b;
	int c;
	int mode_a;
	int mode_b;
	int mode_c;
};


/*
	Fetches the next insrtruction and increments the program counter.
*/
struct Instruction fetch_instruction(int *program, int *PC) {
	struct Instruction instruction;

	// n is all digits of instruction, must be parsed.
	int  n = program[*PC];
	// The two rightmost digits is the opcode
	instruction.opcode = n % 100;
	n = n / 100;
	// Store mode for all operands
	int modes[3];

	for(int i=2; i >= 0; i--) {
		modes[i] = n % 10;
		n = n / 10;
	}

	// Add modes to the struct
	instruction.mode_a = modes[0];
	instruction.mode_b = modes[1];
	instruction.mode_c = modes[2];

	if(instruction.opcode == 1 || instruction.opcode == 2) {
		// Instruction 1 and 2 has three operands
		instruction.a = program[*PC + 1];
		instruction.b = program[*PC + 2];
		instruction.c = program[*PC + 3];
		// Increment program counter
		*PC += 4;
	} else if(instruction.opcode == 3 || instruction.opcode == 4) {
		// Instruction 3 and 4 has one operand
		instruction.a = program[*PC + 1];
		*PC += 2;
	}

	printf("opcode: %d a: %d b: %d c: %d\n", instruction.opcode,instruction.mode_a,instruction.mode_b,instruction.mode_c);


	return instruction;
}

/*
	Function that sets a operand based on a paramter value
	parameter = 0 is position mode
	paramater = 1 is immediate mode
*/
void set_operand_value(int *program, int *op_value, int op, int mode) {

	if(mode == 0) {
		// position mode
		*op_value = program[op];
	} else if(mode == 1) {
		*op_value = op;
	}
}

int main() {
	
	// Load program
	int num_integers;
	int *program = load_program("./test_program.txt", &num_integers);
	
	// Execute program
	int PC = 0;  // Program counter
	int run_program = 1;
	// Helper variables used for the instructions
	int a,b;


	while(run_program) {
		
		struct Instruction instruction = fetch_instruction(program, &PC);
		switch(instruction.opcode) {
			case 1:	
				// OPCODE 1	- ADD
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				program[instruction.c]  = a + b;
				break;
			case 2:
				// OPCODE 2 - MULTIPLY
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				printf("a: %d, b: %d\n", a,b);
				program[instruction.c] = a * b;
				break;
			case 3:
				// OPCODE 3 - INPUT
				char input[10];
				int input_integer;
				fgets(input, 10,stdin);
				program[instruction.a] = atoi(input);
				break;
			case 4:
				// OPCODE 4 - OUTPUT
				printf("OUT: %d\n", program[instruction.a]);
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
