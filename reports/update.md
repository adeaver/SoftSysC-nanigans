## Learning About Memory and Vulnerabilities with Coroutines
Andrew Deaver, Kevin Zhang

### Goals

The goal of this project was originally to explore exploiting C programs. That goal has since been updated to exploring how memory is laid out in C, and the accompanying vulnerabilities that almost always follow. We plan to create a demonstration a custom made C script using a runtime "stack" protocol, and hopefully with an exploitation presentation. The script itself should perform a simple but generalizable action that contains a security flaw using our self-built basic memory stack. Our demonstration then will be able to advantage of the flaw to show how an exploit is performed in C programs. After much research on various exploits and exploration into industrial security measures to prevent such exploits, we have decided to implement coroutines as a method of creating our own custom made C program with an exploitable stack. Our current lower bound is implementing setjmp and longjmp ourselves to create a simple stack vulnerability. Our upper bound is to then create a script/presentation that will show us exploiting that flaw to, for example, enter a previously isolated function or start a shell script.

### Learning Goals

- Become better C developers
- Understand common pitfalls that developers make that cause security flaws
- Develop a better understanding of the low level mechanisms of C and Linux that cause these vulnerabilities, specifically regarding memory
- Think critically about how memory is managed and laid out in order to understand how code execution works and implement custom stack protocols, such as coroutines

### What We Have Done

After a week of working through Exploits-Exercises' Protostar module, we decided that there wasn't enough to formulate a solid deliverable other than a list of ways to exploit C, which we didn't feel was very meaningful. We had, however, learned a lot about how memory is laid out in code execution and stack protocols, and decided to build more on that knowledge as well as some guidance provided by the instructor and pivot to implementing Coroutines in C, with a hopeful direction towards exploiting it if time permits.

- We worked through the following tutorial/module in order to better understand security flaws in C: Exploit Exercises, Protostar: https://exploit-exercises.com/protostar/
	- Solutions: https://thesprawl.org/research/exploit-exercises-protostar-stack/
	- This was a helpful resource: http://liveoverflow.com/binary_hacking/
- We read the paper "Smashing the Stack for Fun and Profit" which gave us a good idea of common exploits and a better picture of how memory is laid out.
    - http://www-inst.eecs.berkeley.edu/~cs161/fa08/papers/stack_smashing.pdf
- We worked through examples and tutorials for setjmp, jump_buf, and longjmp
    - https://stackoverflow.com/questions/33854035/gcc-modify-where-execution-continues-after-function-return/33854063
    - http://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/lecture.html
    - https://www.embecosm.com/appnotes/ean9/html/ch04s01s02.html
    - https://stackoverflow.com/questions/1823778/what-does-each-entry-in-the-jmp-buf-structure-hold
    - https://stackoverflow.com/questions/14685406/practical-usage-of-setjmp-and-longjmp-in-c

We are also very much open to suggestions and guidance from instructors about ways to implement coroutines.


### What's Next  

1. Our next step is to figure how to implement setjmp, jump_buf, and longjmp from the C library. Kevin is handling longjmp, Andrew is handling setjmp, and jump_buf is a simple data structure that will be written once we combine our code.
    - "Done" is defined as Kevin is able to produce a script that can jump to a function using his implementation of longjmp, and Andrew is able to produce a script that can store the data needed to make the jump to a specific place with his implementation of setjmp.
2. Next, we hope to combine our halves into an example program that uses our implementation of setjmp, jump_buf, and longjmp, and increase complexity with the jump_buf information to create a script that performs a simple action using our custom made "stack" protocol. This is integration and will be done together.
    - "Done" is defined as a single script that can perform arithmetic or print some information, perhaps by input, using our implementations of setjmp and longjmp to manipulate flow of execution to switch between additions or print statements.
3. Our hopeful last step is to expose any security flaw associated with this program, and demonstrate not only that we have created our own coroutine program that manipulate memory at will, but also that we can exploit the code and smash the stack for fun and profit. Should we have time to get to this part, it will be done together.
    - "Done" is defined as either another script or just a set of commands that can demonstrate a stack overflow in our coroutine program, perhaps entering an isolated function or changing the value of certain variables.
