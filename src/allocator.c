#include "luz/allocator.h"
#include "luz/assert.h"
#include <stdio.h>
#include <stdlib.h>

static void *
libc_malloc(usize size, usize _align, void *_ctx, const char *_file, u32 _line)
{
	void *p = 0;
	(void)_ctx;
	(void)_file;
	(void)_line;
	(void)_align; /* libc must return an address that has correct alignment to everything */
	ASSERT_DEBUG(size > 0);
	p = malloc(size);
	ASSERT_DEBUG(p != NULL);
	return p;
}

static void
libc_free(void *p, usize _size, void *_ctx, const char *_file, u32 _line)
{
	(void)_size;
	(void)_ctx;
	(void)_file;
	(void)_line;
	ASSERT_DEBUG(p != NULL);
	free(p);
}

static void *
libc_realloc(void *p, usize _old, usize new, usize _align, void *_ctx, const char *_file, u32 _line)
{
	void *p_new = 0;
	(void)_old;
	(void)_ctx;
	(void)_file;
	(void)_line;
	(void)_align; /* libc must return an address that has correct alignment to everything */
	ASSERT_DEBUG(p != NULL);
	ASSERT_DEBUG(new > 0);
	p_new = realloc(p, new);
	ASSERT_DEBUG(p_new != NULL);
	return p_new;
}

void
allocator_libc(struct allocator *allocator)
{
	allocator->alloc = libc_malloc;
	allocator->free = libc_free;
	allocator->realloc = libc_realloc;
	allocator->ctx = NULL;
}

static void *
libc_malloc_debug(usize size, usize align, void *ctx, const char *file, u32 line)
{
	void *p = 0;
	fprintf(stdout, "%s:%u: libc malloc(%lu)", file, line, size);
	p = libc_malloc(size, align, ctx, file, line);
	fprintf(stdout, " -> %p\n", p);
	return p;
}

static void
libc_free_debug(void *p, usize size, void *ctx, const char *file, u32 line)
{
	fprintf(stdout, "%s:%u: libc free(%p)\n", file, line, p);
	libc_free(p, size, ctx, file, line);
}

static void *
libc_realloc_debug(void *p, usize old, usize new, usize align, void *ctx, const char *file, u32 line)
{
	void *p_new = 0;
	fprintf(stdout, "%s:%u: libc realloc(%p, %lu)", file, line, p, new);
	p_new = libc_realloc(p, old, new, align, ctx, file, line);
	fprintf(stdout, " -> %p\n", p_new);
	return p_new;
}

void
allocator_libc_debug(struct allocator *allocator)
{
	allocator->alloc = libc_malloc_debug;
	allocator->free = libc_free_debug;
	allocator->realloc = libc_realloc_debug;
	allocator->ctx = NULL;
}

static void *
arena_alloc(usize size, usize align, void *ctx, const char *_file, u32 _line)
{
	struct arena_allocator *arena = 0;
	usize padding = 0;
	void *p = 0;
	(void)_file;
	(void)_line;
	ASSERT_DEBUG(ctx != NULL);
	arena = (struct arena_allocator *)ctx;
	padding = -arena->offset & (align - 1);
	if (arena->offset + padding + size > arena->size)
	{
		ASSERT_DEBUG(FALSE);
		return NULL;
	}
	p = &arena->buffer[arena->offset + padding];
	arena->offset += padding + size;
	return p;
}

static void
arena_free(void *_p, usize _size, void *_ctx, const char *_file, u32 _line)
{
	(void)_p;
	(void)_size;
	(void)_ctx;
	(void)_file;
	(void)_line;
}

static void *
arena_realloc(void *_p, usize _old, usize _new, usize _align, void *_ctx, const char *_file, u32 _line)
{
	(void)_p;
	(void)_old;
	(void)_new;
	(void)_align;
	(void)_ctx;
	(void)_file;
	(void)_line;
	ASSERT_DEBUG(FALSE);
	return NULL;
}

void
allocator_arena(struct allocator *allocator, struct arena_allocator *arena, u8 *buffer, usize size)
{
	allocator->alloc = arena_alloc;
	allocator->free = arena_free;
	allocator->realloc = arena_realloc;
	allocator->ctx = arena;
	arena->buffer = buffer;
	arena->size = size;
	arena->offset = 0;
}

void
arena_reset(struct arena_allocator *arena)
{
	arena->offset = 0;
}
