#include <stdlib.h>
#include <mrvn.h>

void free(void *mem) {
    Chunk *chunk = (Chunk*)((char*)mem - HEADER_SIZE);
    Chunk *next = CONTAINER(Chunk, all, chunk->all.next);
    Chunk *prev = CONTAINER(Chunk, all, chunk->all.prev);
 
	//printf("%s(%p): @%p %#lx [%d]\n", __FUNCTION__, mem, chunk, memory_chunk_size(chunk), memory_chunk_slot(memory_chunk_size(chunk)));
    mem_used -= memory_chunk_size(chunk);
 
    if (next->used == 0) {
		// merge in next
		remove_free(next);
		dlist_remove(&next->all);
		//memset(next, 0xDD, sizeof(Chunk));
		mem_meta -= HEADER_SIZE;
		mem_free += HEADER_SIZE;
    }
    if (prev->used == 0) {
		// merge to prev
		remove_free(prev);
		dlist_remove(&chunk->all);
		//memset(chunk, 0xDD, sizeof(Chunk));
		push_free(prev);
		mem_meta -= HEADER_SIZE;
		mem_free += HEADER_SIZE;
    } else {
		// make chunk as free
		chunk->used = 0;
		DLIST_INIT(chunk, free);
		push_free(chunk);
    }
}
