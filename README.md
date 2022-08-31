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
