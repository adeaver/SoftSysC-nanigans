/*
This code is meant to showcase an example setjmp functionality combined with longjmp from
the other example script. It shows a simple jump enabling a loop in the main function
using the custom implemented version of set_jmp and long_jmp
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
	int data;
	void (*sp)(void);
} jump_buf_simple;

/*
This function allocates a simple jump buffer

returns a pointer to a freshly allocated jump_buf_simple
*/
jump_buf_simple *make_jump_buffer() {
	jump_buf_simple *buffer = malloc(sizeof(jump_buf_simple));
	buffer->data = 0;
	buffer->sp = 0;
	buffer->sp_int = 0;
	return buffer;
}

/*
This function captures the return address to refer to
as a point to come back to. Unlike the real setjmp, it does not return the value
for the data in the buffer. We implemented it like this because when returning
a value, the return address gets overwritten, so we were getting a segmentation fault.
*/
void setjmp_simple(jump_buf_simple *buffer) {
	uint64_t sp;
    asm("movq 8(%%rbp), %0"
			:"=r" (sp));
	puts("Goodbye, World");
	//buffer->sp = __builtin_return_address(0); // Alternate solution
	buffer->sp = sp;
	buffer->sp_int = sp;
}

/*
main function that performs a simple loop to show the jump
*/
int main() {
	jump_buf_simple *buffer = make_jump_buffer();
	setjmp_simple(buffer);
	uint64_t buf = 1;
	puts("Hello, World");
	if(buffer->data < 10) {
		printf("Not found\n");
		buffer->data++;
		buffer->sp();
	} else {
		printf("Found\n");
	}

	// We messed up the state of the stack by only manipulating the instruction pointer
    //so to return,
	// we have to explicitly tell the main function where to go
	void (*foo)(void) = __builtin_return_address(0);
	foo();
}
