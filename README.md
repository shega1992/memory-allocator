# memory-allocator
This repository holds the source code for memory allocator. The idea for the project is taken from OSTEP https://github.com/remzi-arpacidusseau/ostep-projects. However, OSTEP does not contain a description for this project. There are several tutorials I can recommend:

1) http://danluu.com/malloc-tutorial/ . This is a simple version of the memory allocator that does not include memory alignment, block splitting, and block merges. Be careful, the code is not quite correct while the ideas themselves are still correct. For example
```
void *malloc(size_t size) {
  struct block_meta *block;
  // TODO: align size?

  if (size <= 0) {
    return NULL;
  }
  ...
```
**size** parameter has **size_t** type. **size_t** is unsigned integer type. Therefore **size** cannot take values less than 0, which results incorrect condition **size <= 0**. 

2) https://wiki-prog.infoprepa.epita.fr/images/0/04/Malloc_tutorial.pdf . This is a good tutorial, although the presentation of individual ideas seemed a bit abstruse to me.
## What is in this allocator?
Conceptually, my memory allocator is similar to the two presented above, but there are differences:
1) I forcibly align headers( where the information about the allocated block is stored) and memory blocks along an 8-byte boundary. Thus, if the user requests for example 3 bytes, the allocator will return 8 (although the user will not know about it). In my system, header alignment is optional, the **gcc** aligns the header to an 8-byte alignment by default. Alignment is mainly intended for compatibility with 32-bit systems, where the **size_t** type and pointer are taken 4 bytes.
2) I used **best fit** strategy instead of **first fit** when searching for a free block, but I added one feature: if we find a block exactly the right size, we immediately return it.  Thus, with a certain probability, we will not have to go through the entire list. The splitting and merging of blocks are available in this allocator.
3) Allocator is presumably thread-safe (some additional checks may be required).
## How to use this allocator?
I suggest the following procedure:
**cpp -v /dev/null -o /dev/null** (this trick is taken from https://gcc.gnu.org/onlinedocs/cpp/Search-Path.html#Search-Path) to know search path in your system.
We are interested in the following lines(the output may differ in your system):
```
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
 ```
I suggest using **/usr/local/include** to keep header file(**wallocator.h**). You will need to be a superuser to move the file to this directory. After that, you can
```
#include <wallocator.h>
```
in your programs.
But the work is not finished yet. The next step is to move liballoc.so. Similar to the previous step I suggest using /usr/local/lib to keep the library.
The following actions are necessary( I do not know the exact details of each linux distribution, so I am only giving an assumed set of actions ):
1) Check /etc/ld.so.conf for include /etc/ld.so.conf.d/*.conf line. If line exist then go to the next step. If no then put one line in it: /usr/local/lib and skip steps 2-4.
2) Check /etc/ld.so.conf.d/libc.conf. If file contains /usr/local/lib line then no additional work is required. Skip steps 3-4.
3) If libc.conf file is not exist or not contain /usr/local/lib line then check other files in this directory for /usr/local/lib. if successful, skip next step.
4) If no file in the directory contains the searched line, then create a file with the .conf extension and put one line in it: /usr/local/lib.
5) Type sudo ldconfig command.
Now we are ready to use the library in our programs. For example, let's take the Ex1.c program from the Tests directory. To compile: gcc -o Ex1 Ex1.c -lalloc.
What is -lalloc? Basically it's -llibrary linker option. -l is an option followed by the name of our library without a lib prefix and .so extension. 
For more details type man gcc then /-llibrary.
Let's make sure we are using liballoc.so in our program. Type ldd ./Ex1.( ldd prints the shared objects (shared libraries) required by each program or shared object specified on the command line).
The output will be something like this:
    linux-vdso.so.1 (0x00007ffd615d3000)
	liballoc.so => /usr/local/lib/liballoc.so (0x00007f0f131a2000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f0f12fb0000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f0f131c3000)
Good, now let's run the program. Type ./Ex1
Here's how the program works on my system:
Enter the number of elements: 5
Enter the initialization value: 3

3 3 3 3 3
Enter the number of elements (<1 to quit): 0
Done.
