/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */

#pragma once

#ifndef _malloc_H_
#define _malloc_H_

 /* Define the block size since the sizeof will be wrong */
#define BLOCK_SIZE 40

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

void* mm_malloc(size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);


typedef struct s_block *s_block_ptr;

/* block struct */
struct s_block {
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;
    /* A pointer to the allocated block */
    char data [0];
 };

/* Split block according to size, b must exist */
void split_block (s_block_ptr b, size_t s){

 s_block_ptr new;
new = (s_block_ptr)(b->data + s);
new->size = b->size - s - BLOCK_SIZE;
new->next = b->next;
new->prev = b;
new->free = 1;
new->ptr = new->data;
b->size = s;
b->next = new;
if(new->next) new->next->prev = new;

}

/* Try fusing block with neighbors */
s_block_ptr fusion(s_block_ptr b){
	 if(b->next && b->next->free){
	b->size += BLOCK_SIZE + b->next->size;
	b->next = b->next->next;
	if (b->next) b->next->prev = b;
	}
	return b;
}

/* Get the block from addr */
s_block_ptr get_block (void *p){
char *tmp;
tmp = p;
return (p = tmp -= BLOCK_SIZE);
}

/* Add a new block at the of heap,
 * return NULL if things go wrong
 */





s_block_ptr extend_heap (s_block_ptr last , size_t s){
	 size_t sb;
	s_block_ptr b;
	b = sbrk(0);
	sb = (size_t)sbrk(BLOCK_SIZE + s);
	if(sb < 0) return NULL;
	b->size = s;
	b->next = NULL;
	b->prev = last;
	b->ptr = b->data;
	if(last) last->next = b;
	b->free = 0;
	return b;

}

/*
void set_block_content(s_block_ptr b,size_t s,struct s_block *next,struct s_block *prev, int free, void *ptr){
	b->size = s;
	b->next = next;
	b->prev = prev;
	b->free = free;
	b->ptr = ptr;
}
*/

#ifdef __cplusplus
}

#endif
#endif
