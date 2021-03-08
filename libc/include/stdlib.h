#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);

void memory_init(void *mem, size_t size);

void *malloc(size_t size);

void free(void *mem);

#ifdef __cplusplus
}
#endif

#endif
