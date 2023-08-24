#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lexicographical_permutations.h"


/*
	Prints the current program,
	used for debugging
*/
void print_program(int *program, size_t length) {

	for(int i = 0; i < length; i++) {
		printf("%d,", program[i]);
	}
	printf("\n");
}


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
	
	// Get number of integers
	*num_integers = 1;
	for(char *c = program_string; *c != '\0'; c++) {
		if(*c == ',')
			*num_integers += 1;
	}

	// Allocate memory for the program array
	program = (int*)malloc(*num_integers * sizeof(int));

	// Parse program_string for integers
	char int_string[10];
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
	int instruction;
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
	instruction.instruction = n;

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
	instruction.mode_a = modes[2];
	instruction.mode_b = modes[1];
	instruction.mode_c = modes[0];

	if(instruction.opcode == 1 || instruction.opcode == 2 || instruction.opcode == 7 || instruction.opcode == 8) {
		// Instruction 1,2,7,8 has three operands
		instruction.a = program[*PC + 1];
		instruction.b = program[*PC + 2];
		instruction.c = program[*PC + 3];
		// Increment program counter
		*PC += 4;
	} else if(instruction.opcode == 3 || instruction.opcode == 4) {
		// Instruction 3 and 4 has one operand
		instruction.a = program[*PC + 1];
		*PC += 2;
	} else if(instruction.opcode == 5 || instruction.opcode == 6) {
		// Instruction 5 and 6 has 2 operands
		instruction.a = program[*PC + 1];
		instruction.b = program[*PC + 2];
		*PC += 3;
	}

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


struct Amp {
	int program_len;
	int *program;
	int pc;
	int has_halted;
	int amp_nmbr;
};

void execute_amp(struct Amp* amp, int *input, int input_length, int *output) {

	int *program = (*amp).program;
	int program_len = (*amp).program_len;
	int *PC = &(*amp).pc;
	int a,b,c;

	// There will be two inputs, given in the input array.
	// input_count keep track of the first and second input.
	int input_count = 0;

	while(1) {
		struct Instruction instruction = fetch_instruction(program, PC);
		
		//printf("amp: %d\n", amp.amp_nmbr);
		//printf("instruction: %d opcode: %d PC: %d\n", instruction.instruction, instruction.opcode, *PC);
		//print_program(program,program_len);
	
		switch(instruction.opcode) {

			case 0:
				printf("Invalid opcode %d at PC = %d\n", instruction.opcode, *PC);
				exit(-1);
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
				program[instruction.c] = a * b;
				break;
			case 3:
				// OPCODE 3 - INPUT
				//char input[10];
				//int input_integer;
				//fgets(input, 10,stdin);
				if(input_length == 2) {
					if(input_count > 1) {
					printf("Error: It should be max two input values\n");
					exit(-1);
					}
					program[instruction.a] = input[input_count++];
				}
				else if(input_length == 1) {
					program[instruction.a] = input[0];
				}
				
				break;
			case 4:
				// OPCODE 4 - OUTPUT
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				// Set output value and return
				*output = a;
				return;
				//printf("%d\n", a);
				break;
			case 5:
				// OPCODE 5 - jump-if-true
				// if the first parameter is non-zero, it sets the instruction pointer 
				// to the value from the second parameter. Otherwise, it does nothing.
				
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				if(a != 0) {
					*PC = b;
				}
				
				break;
			case 6:
				// OPCODE 6 - jump-if-false
				// if the first parameter is zero, it sets the instruction pointer 
				// to the value from the second parameter. Otherwise, it does nothing.
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				if(a == 0) {
					*PC = b;
				}
				
				break;
			
			case 7:
				// OPCODE 7 - less-than
				// if the first parameter is less than the second parameter, 
				// it stores 1 in the position given by the third parameter. Otherwise, it stores 0.

				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				set_operand_value(program, &c,instruction.c, instruction.mode_c);

				if(a < b) {
					program[instruction.c] = 1;
				
				} else {
					program[instruction.c] = 0;

				}
				break;
			case 8:
				// OPCODE 8 - equals
				// f the first parameter is equal to the second parameter, 
				// it stores 1 in the position given by the third parameter. Otherwise, it stores 0.
				set_operand_value(program, &a,instruction.a, instruction.mode_a);
				set_operand_value(program, &b,instruction.b, instruction.mode_b);
				set_operand_value(program, &c,instruction.c, instruction.mode_c);
				
				if(a == b) {
					program[instruction.c] = 1;

				} else {
					program[instruction.c] = 0;

				}
				break;
			case 99:
				(*amp).has_halted = 1;
				return;
			default:
				break;
		}
		
	}
}


int main() {

	#define NUM_AMPS 5
	// Load program
	int num_integers;
	int *program = load_program("./day_7_input.txt", &num_integers);
	int phase_sequence[5] = {5,6,7,8,9};
	
	// Keep track of the max output among all permutations.
	int max_output = 0;

	// Loop over all permutations of phase_sequence
	// The loop will break after last permutation
	while(1) {
		
		print_sequence(phase_sequence, 5);
		int output = 0; 
		// Array of amplifiers
		struct Amp *amps = (struct Amp*)malloc(NUM_AMPS * sizeof(struct Amp) + (num_integers - 1) * sizeof(int));

		// Set up each amp
		for(int i = 0; i <  NUM_AMPS; i++) {
			struct Amp amp;
			// Allocate memory for program
			amp.program = (int*)malloc(num_integers * sizeof(int));
			// Copy program to the amp
			memcpy(amp.program, program, num_integers * sizeof(int));
			amp.program_len = num_integers;
			amp.pc = 0;
			amp.has_halted = 0;
			amp.amp_nmbr = i;

			amps[i] = amp;
		}
		
		// First iteration of the feedback loop, wil give both phase and signal as input.
		for(int i = 0; i < NUM_AMPS; i++) {
			int input[2] = {phase_sequence[i], output};
			execute_amp(&amps[i], input, 2, &output);
		}
		// Keep the feedback loop going until all amps has halted.
		while(!amps[4].has_halted) {
			// Loop over each amp
			for(int i = 0; i < NUM_AMPS; i++) {
				int input[1] = {output};
				execute_amp(&amps[i], input, 1, &output);
			}	
		}

		max_output = output > max_output ? output : max_output;
		for(int i = 0; i < NUM_AMPS; i++) {
			free(amps[i].program);
		}
		free(amps);

		if(!next_lexicographical_permutaion(phase_sequence, 5)) break;
	}
	
	printf("max output: %d\n", max_output);
	free(program);

	return -1;
}
