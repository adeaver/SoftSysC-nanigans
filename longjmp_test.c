#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
inline assembly version of address jumping.
because it requires a string literal, this one is highly experimental
*/
void go() {
    char move[100] = "jmp ";

    asm("jmp 0x1010101");
}

/*
basic jump test function, shows that you can use void function pointers that point
to address locations to jump to a specific point in memory without calling the function explicitly
*/
void jump(){
    printf("Hello you have jumped successfully!");
    exit(0);
    // go();
}

/*
demonstration of jumping back to the same function 10 times and then jumping to
a jump function afterwards to end
*/
void sub() {
    static int i;
    if(i++ < 10) printf("Hello %d\n", i);
    else{
      void (*foob)(void) = &jump;
      foob();
    }

    void (*foo)(void) = &sub;
    foo();
}

/*
basic test main fo jumping to address locations
*/
int main() {
    sub();

}
