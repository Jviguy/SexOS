#include <stdlib.h>
#include <mrvn.h>

void *malloc(size_t size) {
    //printf("%s(%#lx)\n", __FUNCTION__, size);
    size = (size + ALIGN - 1) & (~(ALIGN - 1));
 
	if (size < MIN_SIZE) size = MIN_SIZE;
 
	int n = memory_chunk_slot(size - 1) + 1;
 
	if (n >= NUM_SIZES) return NULL;
 
	while(!free_chunk[n]) {
		++n;
		if (n >= NUM_SIZES) return NULL;
    }
 
	Chunk *chunk = DLIST_POP(&free_chunk[n], free);
    size_t size2 = memory_chunk_size(chunk);
	//printf("@ %p [%#lx]\n", chunk, size2);
    size_t len = 0;
 
	if (size + sizeof(Chunk) <= size2) {
		Chunk *chunk2 = (Chunk*)(((char*)chunk) + HEADER_SIZE + size);
		memory_chunk_init(chunk2);
		dlist_insert_after(&chunk->all, &chunk2->all);
		len = memory_chunk_size(chunk2);
		int n = memory_chunk_slot(len);
		//printf("  adding chunk @ %p %#lx [%d]\n", chunk2, len, n);
		DLIST_PUSH(&free_chunk[n], chunk2, free);
		mem_meta += HEADER_SIZE;
		mem_free += len - HEADER_SIZE;
    }
 
	chunk->used = 1;
    //memset(chunk->data, 0xAA, size);
	//printf("AAAA\n");
    mem_free -= size2;
    mem_used += size2 - len - HEADER_SIZE;
    //printf("  = %p [%p]\n", chunk->data, chunk);
    return chunk->data;
}
