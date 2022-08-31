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
**size** parameter has size_t type. size_t is unsigned integer type. Therefore **size** cannot take values less than 0, which results incorrect condition **size <= 0**. 

2) 
