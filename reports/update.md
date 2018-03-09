## Learning About Memory with Coroutines 
Andrew Deaver, Kevin Zhang

### Goals

The goal of this project was originally to explore exploiting C. After a week of working through Exploits-Exercises' Protostar module, we decided that there wasn't enough to formulate a solid deliverable other than a list of ways to exploit C, which we didn't feel was very meaningful. We had, however, learned a lot about how memory is laid out, and decided to build more on that knowledge as well as some guidance provided by the instructor and pivot to implementing Coroutines in C. We want to implement coroutines while also not exposing huge security flaws (or at least being aware of the security flaws that we expose). Our lower bound is implementing setjmp and longjmp (without assembly). Our upper bound is to expose any security flaws with this code.

### Learning Goals

- Become better C developers
- Understand common pitfalls that developers make that cause security flaws
- Develop a better understanding of the low level mechanisms of C and Linux that cause these vulnerabilities
- Think critically about how memory is managed and laid out in order to implement coroutines

### What We Have Done 

- We worked through the following tutorial/module in order to better understand security flaws in C: Exploit Exercises, Protostar: https://exploit-exercises.com/protostar/ 
	- Solutions: https://thesprawl.org/research/exploit-exercises-protostar-stack/
	- This could help: http://liveoverflow.com/binary_hacking/
- We read the paper "Smashing the Stack for Fun and Profit" which gave us a good idea of common exploits and a better picture of how memory is laid out.
- We worked through examples and tutorials for setjmp, jump_buf, and longjmp 

### What's Next  

1. Implementation of setjmp, jump_buf, and longjmp
2. Write an example program that uses our implementation of setjmp, jump_buf, and longjmp
3. (Optional) Expose any security flaws associated with this program 
