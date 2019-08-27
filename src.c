#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 256 

/* instructions */ 
typedef enum {
	PSH,
	ADD,
	SUB, 
	MLT,
	DIV,
	POP,
	SET,  /* sets register value */ 
	HLT  /* stops program */ 
} InstructionSet; 

typedef enum {
	A, B, C, D, E, F, SP, IP, /* A-F are general registers, sp and ip store stack pointer and instruction pointer */ 
	NUM_OF_REGISTERS
} Registers; 

int registers[NUM_OF_REGISTERS];
int stack[MAX_STACK_SIZE];  
bool running = true;  /* tracks if program is running */ 

/* test array of instructions */ 
const int test_program[] = {
	PSH, 5,
	PSH, 6, 
	ADD, /* adds 5 and 6, pops from stack, and pushes result to stack */ 
	PSH, 8, 
    ADD,  
    POP, 
    PSH, 10,
    PSH, 5,
    DIV,
    PSH, 3,
    MLT, 
    PSH, 1, 
    SUB, 
	HLT
};

void print_stack() {
	int i; 
	printf("\nStack:\n");
	for (i = 0; i <= registers[SP]; i++) {
		printf("%d\n", stack[i]);
	}
	printf("\n");
}

void print_registers() {
	int i;
	printf("\nRegisters:\n");
	for (i = 0; i < NUM_OF_REGISTERS; i++) {
		printf("%d\n", registers[i]);
	}
	printf("\n");
}

/* sets instruction pointer and stack pointer values in registers array */ 
void initialize_registers() {
	registers[SP] = -1;
	registers[IP] = 0;
}

int increment_register(int register_num) {
	registers[register_num] = registers[register_num] + 1; 
	return registers[register_num]; 
}

int decrement_register(int register_num) {
	registers[register_num] = registers[register_num] - 1; 
	return registers[register_num]; 
}

/* gets line instruction */ 
int fetch() {
	int ip = registers[IP];
	return test_program[ip];
}

/* interprets line instruction */ 
void eval(int instr) {
	switch (instr) {
		case HLT: {
			running = false; 
			break; 
		}
		case PSH: {
			int sp = increment_register(SP);
			int ip = increment_register(IP);
			stack[sp] = test_program[ip];
			break; 
		}
		case POP: {
			int sp = registers[SP];
			int value = stack[sp];
			printf("\nPopped %d\n", value);
			decrement_register(SP); 
			break; 
		}
		case ADD: {
			int sp = registers[SP]; 
			int a = stack[sp];  /* pops previous two values in stack and pushes result */ 
			int b = stack[sp - 1];
			int result = a + b;

			stack[sp - 1] = result;
			decrement_register(SP); 
			break;  
		}
		case SUB: {
			int sp = registers[SP];
			int a = stack[sp - 1];
			int b = stack[sp];
			int result = a - b;

			stack[sp - 1] = result;
			decrement_register(SP);
			break; 
		}
		case MLT: {
			int sp = registers[SP];
			int a = stack[sp];
			int b = stack[sp - 1];
			int result = a * b; 

			stack[sp - 1] = result;
			decrement_register(SP);
			break; 
		}
		case DIV: {
			int sp = registers[SP];
			int a = stack[sp - 1];
			int b = stack[sp];
			int result = a / b;

			stack[sp - 1] = result;
			decrement_register(SP);
			break; 
		}
	}
}

int vm() {
	int instr; 

	initialize_registers();
	while (running) {
		instr = fetch();
		eval(instr); 
		registers[IP] = registers[IP] + 1; 
	}
	print_stack();
	print_registers(); 
	return 0; 
}