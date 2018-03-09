#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
	int *data;
	void (*sp)();
} jump_buf_simple;

jump_buf_simple *make_jump_buffer() {
	jump_buf_simple *buffer = malloc(sizeof(jump_buf_simple));
	buffer->data = malloc(sizeof(int));
	*(buffer->data) = 0;
	buffer->sp = 0;
	return buffer;
}

int setjmp_simple(jump_buf_simple *buffer) {
	unsigned long sp;
    asm("movq 8(%%rsp), %0"
			:"=r" (sp));
	printf("Stack pointer: %lu\n", sp);
	buffer->sp = (void (*)()) sp;
	int data = *(buffer->data);
	*(buffer->data) = 0;
	return data;
}

int main() {
	jump_buf_simple *buffer = make_jump_buffer();
	int ret = setjmp_simple(buffer);
	printf("Starting\n");
	if(!ret) {
		printf("Not found\n");
		*(buffer->data) = 4;
		buffer->sp();
	} else {
		printf("Found");
	}
	return 0;
		
}
