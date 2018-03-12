#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
	int data;
	void (*sp)(void);
} jump_buf_simple;

jump_buf_simple *bufferA;
jump_buf_simple *bufferB;

void routineA();
void routineB();

/* This function allocates a simple jump buffer
 *  
 * returns a pointer to a freshly allocated jump_buf_simple
 */
jump_buf_simple *make_jump_buffer() {
	jump_buf_simple *buffer = malloc(sizeof(jump_buf_simple));
	buffer->data = 0;
	buffer->sp = 0;
	return buffer;
}

/* This function captures the return address to refer to 
 * as a point to come back to. Unlike the real setjmp, it does not return the value
 * for the data in the buffer. We implemented it like this because when returning
 * a value, the return address gets overwritten, so we were getting a segmentation fault. 
 */
void setjmp_simple(jump_buf_simple *buffer) {
	uint64_t sp;
    asm("movq 8(%%rbp), %0"
			:"=r" (sp));
	puts("Inside setjmp_simple");
	buffer->sp = sp;
	//buffer->sp = __builtin_return_address(0); // Alternate solution
}

// The following functions are our routines
void routineA() {
	puts("Starting A");
	bufferA = make_jump_buffer(); 
	setjmp_simple(bufferA);

	if(!bufferA->data) routineB();

	puts("Setting B");
	bufferB->data=1;
	setjmp_simple(bufferA);
	if(bufferA->data) bufferB->sp();

	puts("Finished A");
	
	void (*foo)(void) = __builtin_return_address(1);
	foo();	
}

void routineB() {
	puts("Starting B, Setting A");
	bufferB = make_jump_buffer();
	bufferA->data = 1;

	setjmp_simple(bufferB);
	if(!bufferB->data) bufferA->sp();

	puts("Finished B, Setting A");
	bufferA->data = 0;
	bufferA->sp();
	
	puts("This should never print");	
}

int main() {
	puts("Starting Main");
	routineA();
	puts("Finished Main");

	/* The actual return address of main is two functions up
	 * because by the time we get here, routineA has essentially
	 * placed a copy of main onto the stack frame which has a return pointer that goes to A
	 * which has been cleared and everything is f**ked up. So we have to go to the original main to
	 * find the actual return address. In short, we have discovered a new paddlin'
	 */
	void (*foo)(void) = __builtin_return_address(2);
	foo();	
}
