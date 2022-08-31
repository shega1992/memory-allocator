#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#pragma pack(8)
typedef struct header
{
     size_t size;
     struct header * next;
     int free;
}Header;
#pragma pack()

Header * base = NULL;
Header * last;

void * wmalloc(size_t );
void * wcalloc(size_t , size_t );
void * wrealloc(void * , size_t );
void wfree(void * );
Header * find_block(size_t );
void split_block(Header * , size_t );
Header * request_block(size_t );
void merge_blocks(Header * );
size_t align(size_t );

#endif 
