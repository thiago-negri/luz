#include "luz/luz.h"
#include <stdio.h>
#include <stdlib.h>

void *libc_malloc(usize size, void *_ctx, const char *file, u32 line)
{
	void *p = 0;
	ASSERT_DEBUG(size > 0);
	p = malloc(size);
	ASSERT_DEBUG(p != NULL);
	return p;
}

void libc_free(void *p, usize _size, void *_ctx, const char *file, u32 line)
{
	ASSERT_DEBUG(p != NULL);
	free(p);
}

void *libc_realloc(void *p, usize _old, usize new, void *_ctx, const char *file, u32 line)
{
	void *p_new = 0;
	ASSERT_DEBUG(p != NULL);
	ASSERT_DEBUG(new > 0);
	p_new = realloc(p, new);
	ASSERT_DEBUG(p_new != NULL);
	return p_new;
}

void allocator_libc(struct allocator *allocator)
{
	allocator->alloc = libc_malloc;
	allocator->free = libc_free;
	allocator->realloc = libc_realloc;
	allocator->ctx = NULL;
}

void *libc_malloc_debug(usize size, void *ctx, const char *file, u32 line)
{
	void *p = 0;
	fprintf(stdout, "%s:%u: libc malloc(%lu)", file, line, size);
	p = libc_malloc(size, ctx, file, line);
	fprintf(stdout, " -> %p\n", p);
	return p;
}

void libc_free_debug(void *p, usize size, void *ctx, const char *file, u32 line)
{
	fprintf(stdout, "%s:%u: libc free(%p)\n", file, line, p);
	libc_free(p, size, ctx, file, line);
}

void *libc_realloc_debug(void *p, usize old, usize new, void *ctx, const char *file, u32 line)
{
	void *p_new = 0;
	fprintf(stdout, "%s:%u: libc realloc(%p, %lu)", file, line, p, new);
	p_new = libc_realloc(p, old, new, ctx, file, line);
	fprintf(stdout, " -> %p\n", p_new);
	return p_new;
}

void allocator_libc_debug(struct allocator *allocator)
{
	allocator->alloc = libc_malloc_debug;
	allocator->free = libc_free_debug;
	allocator->realloc = libc_realloc_debug;
	allocator->ctx = NULL;
}
