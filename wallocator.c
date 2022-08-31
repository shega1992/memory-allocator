#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "wallocator.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * wmalloc(size_t size)
{
     Header * block;
     int rc;
     
     if(!size)
	  return NULL;
     
     size = align(size);
     rc = pthread_mutex_lock(&lock);
     assert(rc == 0);
     if((block = find_block(size))) 
	  ;
     else
	  block = request_block(size);
     rc = pthread_mutex_unlock(&lock);
     assert(rc == 0);

     return (block)? block + 1 : NULL;
}

void * wcalloc(size_t nmem, size_t size)
{
     void * ptr;
     
     if(!nmem)
	  return NULL;
     else if((ptr = wmalloc(nmem * size)))
	  memset(ptr, 0 , size);

     return ptr;
}

void * wrealloc(void * ptr, size_t size)
{
     Header * block = base;
     void * new;
     int rc;
     
     if(!ptr)
	  return wmalloc(size);
     else if(!size && ptr)
     {
	  wfree(ptr);
	  return NULL;
     }
     else
     {
	  while(block && block + 1 != ptr)
	       block = block->next;
	  assert(block + 1 == ptr);
	  assert(block->free == 0);

	  if(block->size >= size)
	  {
	       size = align(size);
	       rc = pthread_mutex_lock(&lock);
	       assert(rc == 0);
	       if(block->size - size > sizeof(Header))
		    split_block(block, size);
	       rc = pthread_mutex_unlock(&lock);
	       assert(rc == 0);
	       return block + 1;
	  }
	  else if((new = wmalloc(size)))
	  {
	       memcpy(new, ptr, block->size);
	       rc = pthread_mutex_lock(&lock);
	       assert(rc == 0);
	       block->free = 1;
	       merge_blocks(base);
	       rc = pthread_mutex_unlock(&lock);
	       assert(rc == 0);
	       return new;
	  }
	  else
	       return NULL;
     }
}
	  
void wfree(void * ptr)
{
     Header * p = base;
     int rc;
     
     if(!ptr)
	  ;
     else
     {
	  while(p && p + 1 != ptr)
	       p = p->next;
	  assert(p + 1 == ptr);
	  
	  rc = pthread_mutex_lock(&lock);
	  assert(rc == 0);
	  assert(p->free == 0);
	  p->free = 1; 
	  merge_blocks(base);
	  rc = pthread_mutex_unlock(&lock);
	  assert(rc == 0);
     }
}
     
Header * find_block(size_t size)
{
     Header * p = base;
     Header * best_chunk = NULL;
     size_t best_size = SIZE_MAX;

     while(p)
     {
	  if(!p->free)
	       ;
	  else if(p->size == size)
	  {
	       p->free = 0;
	       return p;
	  }
	  else if(p->size > size && p->size < best_size)
	  {
	       best_chunk = p;
	       best_size = p->size;
	  }
	  p = p->next;
     }

     if(best_chunk)
     {
	  best_chunk->free = 0;
	  if(best_chunk->size - size > sizeof(Header))
	       split_block(best_chunk, size);
     }

     return best_chunk;
}
	
void split_block(Header * block, size_t size)
{
     Header * new;
     
     new = (Header *)((char *)(block + 1) + size);
     new->size = block->size - sizeof(Header) - size;
     new->free = 1;
     new->next = block->next;
     if(!new->next)
	  last = new;
     
     block->size = size;
     block->next = new;
}

Header * request_block(size_t size)
{
     Header * block  = sbrk(0);
     if(sbrk(sizeof(Header) + size) == (void *) -1)
	  return NULL;
	  
     block->size = size;
     block->free = 0;
     if(!base)
	  base = last = block;
     else
     {
	  last->next = block;
	  last = block;
     }
     block->next = NULL;

     return block;
}

void merge_blocks(Header * block)
{
     if(!block->next)
	  last = block;
     else if(!block->free || !block->next->free)
	  merge_blocks(block->next);
     else
     {
	  block->size += sizeof(Header) + block->next->size;
	  block->next = block->next->next;
	  merge_blocks(block);
     }
}

size_t align(size_t size)
{
     while(size % 8 != 0)
	  size++;

     return size;
}
     
