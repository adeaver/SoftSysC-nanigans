#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
	int *data;
	uint64_t sp;
} jump_buf_simple;

jump_buf_simple *buffer;

int setjmp_simple(jump_buf_simple *buffer) {
	uint64_t sp;
    asm("movq %%rsp, %0"
			:"=r" (sp));
	if (buffer == NULL) {
		buffer = malloc(sizeof(jump_buf_simple));
		buffer->data = malloc(sizeof(int));
		*(buffer->data) = 0;
	}
	buffer->sp = sp;
	int data = *(buffer->data);
	*(buffer->data) = 0;
	return data;	
}

int main() {
	int ret = setjmp_simple(buffer);
	printf("%d\n", ret);
}
