## Learning About Memory and Vulnerabilities with Coroutines
Andrew Deaver, Kevin Zhang

#### Project Goals

Our goal for the project was to obtain a deeper understanding of memory management and control flow in C to implement coroutines. We planned to make our own simplified version of the coroutine mechanism to demonstrate what we had learned. It should be able to essentially jump between two different functions or “routines” for preemptive multitasking, each time saving its current state in a given function before jumping to the other one so that it can return to its “saved” point later to continue.

#### Learning Goals

For this project, our learning goals were to generally gain a better understanding for how memory is laid out in C. We originally wanted to use this understanding to be able to exploit memory vulnerabilities in C; however, in the end, we decided to use our powers for good and not evil by shifting our focus to learning how the way that C manages memory and control flow could be used to implement coroutines. A coroutine is basically a multitasking mechanism in C that utilizes in depth knowledge of memory in C to perform its function, so we thought learning more about this mechanism would be a useful and beneficial endeavor. Overall, with this project, we were seeking to become better C developers through achieving a better understanding of the low level mechanisms that C uses to handle control flow and memory management.

#### Useful Resources

- We worked through the following tutorial/module in order to better understand security flaws in C: Exploit Exercises, [Protostar](https://exploit-exercises.com/protostar/) and [Solutions](https://thesprawl.org/research/exploit-exercises-protostar-stack/).
- [Binary Hacking](http://liveoverflow.com/binary_hacking/) was a helpful resource.
- [Other Exploit Tutorials](https://github.com/FabioBaroni/awesome-exploit-development)
- We read the paper ["Smashing the Stack for Fun and Profit"](http://www-inst.eecs.berkeley.edu/~cs161/fa08/papers/stack_smashing.pdf) which gave us a good idea of common exploits and a better picture of how memory is laid out.
- We worked through examples and tutorials for setjmp, jump_buf, and longjmp:
    - [stack overflow](https://stackoverflow.com/questions/33854035/gcc-modify-where-execution-continues-after-function-return/33854063)
    - [lecture](http://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/lecture.html)
    - [documentation](https://www.embecosm.com/appnotes/ean9/html/ch04s01s02.html)
    - [about the jmp_buffer struct](https://stackoverflow.com/questions/1823778/what-does-each-entry-in-the-jmp-buf-structure-hold)
    - [practical example of coroutines](https://stackoverflow.com/questions/14685406/practical-usage-of-setjmp-and-longjmp-in-c)

#### End Product

We made significant progress in working through exploit tutorials on Exploit Exercises, namely Protostar (linked above). Individually, we each completed all of the Stack and Format tutorials as well as some of the Heap tutorials, with periodic meetings to come together and discuss what we had learned. We supplemented our tutorials with reading some papers discussing stack overflows. By the end of all these tutorials, we were becoming familiar with memory layout and low-level C execution flow.

At some point in the middle of the Heap tutorials, we both came to the same conclusion that we were learning a great deal, but with the way we were headed there wasn’t really any sort of tangible deliverable or practical use for where our learning was taking us. Stack overflows and Heap overflows can teach one a lot about how memory works in C, but given their pervasive nature even the most basic programs come with a level of encryption and protection against their implementation that exceeds the scope of this project. After some deliberation, we decided that it was better to pivot slightly to focus more on how memory worked in C code execution rather figuring out how to use this knowledge to exploit vulnerabilities. It was suggested to us that coroutines were a practical and helpful application of memory management in code execution. Therefore we channeled our efforts into learning more about coroutines and memory management in the stack/heap. After understanding how the multitasking process in coroutines functioned, we decided to try and implement our own coroutine on a simplified scale.

We were able to successfully implement a coroutine. The basic structure of the program that we created looked like this:
![General Program Flow](../images/Structure.png?raw=True)

To accomplish this, we had to implement two functions that were inspired by the setjmp library in C. The first function was called setjmp. The point of setjmp is to read a the value for a buffer (while resetting it) and then capture the return address, so the program knows where to jump back to. In the C library, setjmp is architecture dependent and is actually implemented as a macro, not a function. Below is our implementation of setjmp (called setjmp_simple):

```
void __setjmp_simple(jump_buf_simple *buffer) {
	uint64_t sp;
	// It's 8(rbp) because we just allocated an 8 byte int
    asm("movq 8(%%rbp), %0"
			:"=r" (sp));
	buffer->sp = sp;
	//buffer->sp = __builtin_return_address(0); // Alternate solution
}

// We made our set_jmp functionality into a macro to better mimic the real mechanism
#define setjmp_simple(buffer, output) __setjmp_simple(buffer); output = buffer->data; buffer->data = 0;
```

You can see that the function __setjmp_simple takes in a buffer, which will be talked about later, and grabs the base pointer for the current function, which is known as rbp. We get the return address by moving 8 bytes up from the location of rbp because we just allocated an 8 byte integer.  This required us to use a bit of embedded assembly. The macro calls this function and assigns the current value of the buffer to an output variable and resets it. This is also how the setjmp macro works in the library.

The other part of our implementation required jumping back to this return pointer. This was actually fairly easy to do. In the code above, you can see that we assigned the pointer to a void function type inside of the buffer struct, which is shown below. This generates a warning because we’re manually setting the address of this function by assigning the integer value directly to the pointer. But once we do that, we can jump back to the return address by simply calling the function later on in the program.

```
typedef struct {
	int data;
	void (*sp)(void);
} jump_buf_simple;

.
.
.

setjmp_simple(bufferA, val);
	if(!val) bufferB->sp();
```

Given the two halves of the code shown and the flow of execution chart above, put together into a comprehensive script called coroutines_simple.c results in the output below:

![Program Output](../images/Output.png?raw=True)

As you can see, our coroutines script can successfully jump between two different functions or “routines”, much like the originally created set_jmp and long_jmp can. It is important to note that our coroutine is a simplified version of the real one. Namely, we only keep track and restore based on the rbp, whereas the real set_jmp stores information about the entirety of the stack, including all the registers, not just rbp. Thus our functionality is a proof of concept meant to show that this is possible and we now have the ability do this, but it isn't necessarily the full implementation of a set_jmp and long_jmp. We decided that given our timeline and our small pivot that a proof of concept was enough to show the outcome of our project, and any further implementation was an unnecessary complication that doesn't demonstrate any substantial knowledge we haven't already shown here.

A really interesting problem that we encountered while working on this project was that we’d occasionally “lose” the return pointer. That is, sometimes we had complicated the stack so much that the compiler was unable to return to the correct location. We hypothesize this most likely occurs because we only move around in our stack frames based on the rbp, and don't completely save or restore all parts of the state of the stack, thus confusing the instruction pointer and making it unable to find the proper return address. For this reason, we often had to explicitly tell the compiler where to return. This resulted in code that looked like this:

```
void (*foo)(void) = __builtin_return_address(2);
foo();
```

Where we were utilizing a variation of our long_jmp functionality to jump to the physical return address of the function so it could complete its execution.

The most interesting case of this was at the end of main. We hypothesize we had essentially created a copy of main onto the stack, which had no idea where it was supposed to end. Using a return at the end of main caused a segmentation fault because its return pointer was pointing to inaccessible memory. We believe that the stack looked somewhat like this when we finished:

![Stack](../images/Stackframe.png?raw=True)

#### Reflection

Because we pivoted on our project, we didn’t really end up between the upper and lower bounds identified in our original proposal. However, as per the bounds identified in our updated proposal, we ended up between the lower and upper bounds. We were able to implement setjmp and longjmp, which was our lower bound, but we also managed to integrate them into a demonstrable coroutine. We did not achieve our upper bound goals of being able to exploit our own version of setjmp and longjmp. Given our mid-way pivot and our timeline, we believe that we achieved a substantial end product with meaningful takeaways.
We both agree that we achieved our learning goals as evidence by a lot of the tricks we were able to pull in our implementation of coroutines. For example, understanding the lower level mechanics of C lead us to the thought process of assigning the pointer that we got from setjmp to a void function pointer to be able to call it again. Also, our understanding of return pointers helped in our final implementation when we needed to actually exit the program. We believe that we have learned a lot from this project, more than we originally expected, and plan to take these findings about low level memory layout and stack/heap execution flow with us moving forward into Project 2 and beyond.
