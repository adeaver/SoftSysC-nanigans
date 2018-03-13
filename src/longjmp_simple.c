/*
This code is meant to demonstrate the basic functionality of a self-made
long_jmp using a void function pointer.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
inline assembly version of address jumping.
because it requires a string literal and doesn't work  well with types, we chose
to use the void function pointer instead. This one is meant to be a showcase
of different possiblities we looked into.
*/
void go() {
    // attempted to create a custom message to pass into asm(), but it didn't like that
    char move[100] = "jmp ";

    asm("jmp 0x1010101");
}

/*
basic jump test function, shows that you can use void function pointers that point
to address locations to jump to a specific point in memory without calling the function explicitly

this is the destination of the example long_jmp
*/
void jump(){
    printf("Hello you have jumped successfully!");
    exit(0);
    // go();
}

/*
demonstration of jumping back to the same function 10 times and then jumping to
a jump function afterwards to end using the void function pointer as a "long_jmp"

this is the source of the jumping between different addresses on the stack
*/
void sub() {
    static int i;
    if(i++ < 10) printf("Hello %d\n", i);
    else{
      void (*bar)(void) = &jump;
      bar();
    }

    void (*foo)(void) = &sub;
    foo();
}

/*
basic test main for jumping to address locations using our simplified version of the long_jmp
*/
int main() {
    sub();

}
