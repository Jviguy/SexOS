#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
 
typedef struct DList DList;
struct DList {
    DList *next;
    DList *prev;
};
 
// initialize a one element DList
static inline void dlist_init(DList *dlist) {
	//printf("%s(%p)\n", __FUNCTION__, dlist);
    dlist->next = dlist;
    dlist->prev = dlist;
}
 
// insert d2 after d1
static inline void dlist_insert_after(DList *d1, DList *d2) {
	//printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *n1 = d1->next;
    DList *e2 = d2->prev;
 
    d1->next = d2;
    d2->prev = d1;
    e2->next = n1;
    n1->prev = e2;
}
 
// insert d2 before d1
static inline void dlist_insert_before(DList *d1, DList *d2) {
	//printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *e1 = d1->prev;
    DList *e2 = d2->prev;
 
    e1->next = d2;
    d2->prev = e1;
    e2->next = d1;
    d1->prev = e2;
}
 
// remove d from the list
static inline void dlist_remove(DList *d) {
	//printf("%s(%p)\n", __FUNCTION__, d);
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->next = d;
    d->prev = d;    
}
 
// push d2 to the front of the d1p list
static inline void dlist_push(DList **d1p, DList *d2) {
	//printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p != NULL) {
	dlist_insert_before(*d1p, d2);
    }
    *d1p = d2;
}
 
// pop the front of the dp list
static inline DList * dlist_pop(DList **dp) {
	//printf("%s(%p)\n", __FUNCTION__, dp);
    DList *d1 = *dp;
    DList *d2 = d1->next;
    dlist_remove(d1);
    if (d1 == d2) {
	*dp = NULL;
    } else {
	*dp = d2;
    }
    return d1;
}
 
// remove d2 from the list, advancing d1p if needed
static inline void dlist_remove_from(DList **d1p, DList *d2) {
	//printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p == d2) {
	dlist_pop(d1p);
    } else {
	dlist_remove(d2);
    }
}
 
#define CONTAINER(C, l, v) ((C*)(((char*)v) - (intptr_t)&(((C*)0)->l)))
#define OFFSETOF(TYPE, MEMBER)  __builtin_offsetof (TYPE, MEMBER)
 
#define DLIST_INIT(v, l) dlist_init(&v->l)
 
#define DLIST_REMOVE_FROM(h, d, l)					\
    {									\
	typeof(**h) **h_ = h, *d_ = d;					\
	DList *head = &(*h_)->l;					\
	dlist_remove_from(&head, &d_->l);					\
	if (head == NULL) {						\
	    *h_ = NULL;							\
	} else {							\
	    *h_ = CONTAINER(typeof(**h), l, head);			\
	}								\
    }
 
#define DLIST_PUSH(h, v, l)						\
    {									\
	typeof(*v) **h_ = h, *v_ = v;					\
	DList *head = &(*h_)->l;					\
	if (*h_ == NULL) head = NULL;					\
	dlist_push(&head, &v_->l);					\
	*h_ = CONTAINER(typeof(*v), l, head);				\
    }
 
#define DLIST_POP(h, l)							\
    ({									\
	typeof(**h) **h_ = h;						\
	DList *head = &(*h_)->l;					\
	DList *res = dlist_pop(&head);					\
	if (head == NULL) {						\
	    *h_ = NULL;							\
	} else {							\
	    *h_ = CONTAINER(typeof(**h), l, head);			\
	}								\
	CONTAINER(typeof(**h), l, res);					\
    })
 
#define DLIST_ITERATOR_BEGIN(h, l, it)					\
    {									\
        typeof(*h) *h_ = h;						\
	DList *last_##it = h_->l.prev, *iter_##it = &h_->l, *next_##it;	\
	do {								\
	    if (iter_##it == last_##it) {				\
		next_##it = NULL;					\
	    } else {							\
		next_##it = iter_##it->next;				\
	    }								\
	    typeof(*h)* it = CONTAINER(typeof(*h), l, iter_##it);
 
#define DLIST_ITERATOR_END(it)						\
	} while((iter_##it = next_##it));				\
    }
 
#define DLIST_ITERATOR_REMOVE_FROM(h, it, l) DLIST_REMOVE_FROM(h, iter_##it, l)
 
typedef struct Chunk Chunk;
struct Chunk {
    DList all;
    int used;
    union {
	char data[0];
	DList free;
    };
};
 
enum {
    NUM_SIZES = 32,
    ALIGN = 4,
    MIN_SIZE = sizeof(DList),
    HEADER_SIZE = OFFSETOF(Chunk, data),
};
 
Chunk *free_chunk[NUM_SIZES] = { NULL };
size_t mem_free = 0;
size_t mem_used = 0;
size_t mem_meta = 0;
Chunk *first = NULL;
Chunk *last = NULL;
 
static void memory_chunk_init(Chunk *chunk) {
	//printf("%s(%p)\n", __FUNCTION__, chunk);
    DLIST_INIT(chunk, all);
    chunk->used = 0;
    DLIST_INIT(chunk, free);
}
 
static size_t memory_chunk_size(const Chunk *chunk) {
	//printf("%s(%p)\n", __FUNCTION__, chunk);
    char *end = (char*)(chunk->all.next);
    char *start = (char*)(&chunk->all);
    return (end - start) - HEADER_SIZE;
}
 
static int memory_chunk_slot(size_t size) {
    int n = -1;
    while(size > 0) {
	++n;
	size /= 2;
    }
    return n;
}

static void remove_free(Chunk *chunk) {
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : removing chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_REMOVE_FROM(&free_chunk[n], chunk, free);
    mem_free -= len - HEADER_SIZE;
}
 
static void push_free(Chunk *chunk) {
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : adding chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_PUSH(&free_chunk[n], chunk, free);
    mem_free += len - HEADER_SIZE;
}
 
#define MEM_SIZE (1024*1024*256)
char MEM[MEM_SIZE] = { 0 };
 
#define MAX_BLOCK (1024*1024)
#define NUM_SLOTS 1024
void *slot[NUM_SLOTS] = { NULL };
 
void check(void) {
	int	i;
    Chunk *t = last;
 
	DLIST_ITERATOR_BEGIN(first, all, it) {
		assert(CONTAINER(Chunk, all, it->all.prev) == t);
		t = it;
    } DLIST_ITERATOR_END(it);
 
    for(i = 0; i < NUM_SIZES; ++i) {
		if (free_chunk[i]) {
			t = CONTAINER(Chunk, free, free_chunk[i]->free.prev);
			DLIST_ITERATOR_BEGIN(free_chunk[i], free, it) {
			assert(CONTAINER(Chunk, free, it->free.prev) == t);
			t = it;
			} DLIST_ITERATOR_END(it);
		}
    }
}
