#include "luz/luz.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
	struct debug_allocator *debug = 0;
	void *p = 0;
	debug = ctx;
	fprintf(stdout, "%s:%u: libc malloc(%lu)", file, line, size);
	p = libc_malloc(size, align, ctx, file, line);
	fprintf(stdout, " -> %p -- %s\n", p, debug->ref);
	return p;
}

static void
libc_free_debug(void *p, usize size, void *ctx, const char *file, u32 line)
{
	struct debug_allocator *debug = 0;
	debug = ctx;
	fprintf(stdout, "%s:%u: libc free(%p) -- %s\n", file, line, p, debug->ref);
	libc_free(p, size, ctx, file, line);
}

static void *
libc_realloc_debug(void *p, usize old, usize new, usize align, void *ctx, const char *file, u32 line)
{
	struct debug_allocator *debug = 0;
	void *p_new = 0;
	debug = ctx;
	fprintf(stdout, "%s:%u: libc realloc(%p, %lu)", file, line, p, new);
	p_new = libc_realloc(p, old, new, align, ctx, file, line);
	fprintf(stdout, " -> %p -- %s\n", p_new, debug->ref);
	return p_new;
}

struct debug_allocator *
allocator_libc_debug(struct allocator *allocator)
{
	struct debug_allocator *ctx = NULL;
	ctx = malloc(sizeof *ctx);
	if (ctx == NULL)
	{
		return NULL;
	}
	allocator->alloc = libc_malloc_debug;
	allocator->free = libc_free_debug;
	allocator->realloc = libc_realloc_debug;
	allocator->ctx = ctx;
	return ctx;
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
	arena = ctx;
	padding = -arena->offset & (align - 1);
	if (arena->offset + padding + size > arena->size)
	{
		p = NULL;
	}
	else
	{
		p = &arena->buffer[arena->offset + padding];
		arena->offset += padding + size;
	}
	ASSERT_DEBUG(p != NULL);
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
arena_realloc(void *_p, usize _old, usize new, usize align, void *ctx, const char *file, u32 line)
{
	(void)_p;
	(void)_old;
	return arena_alloc(new, align, ctx, file, line);
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

void
assert(const char *file, u32 line, const char *check_str, int check)
{
	if (!check)
	{
		fprintf(stderr, "%s:%u: (assert) %s\n", file, line, check_str);
		fflush(stderr);
		exit(1);
	}
}

u64
fs_timestamp(const char *path)
{
	int rc = 0;
	struct stat file_stat = { 0 };
	ASSERT_DEBUG(path != NULL);
	rc = stat(path, &file_stat);
	ASSERT_DEBUG(rc == 0);
	return file_stat.st_mtime;
}

void
fs_delete(const char *path)
{
	int rc = 0;
	ASSERT_DEBUG(path != NULL);
	rc = unlink(path);
	ASSERT_DEBUG(rc == 0);
}

void
fs_mkdir(const char *path)
{
	int rc = 0;
	rc = mkdir(path, 0700);
	ASSERT_DEBUG(rc == 0 || errno == EEXIST);
}

void
str_copy_cstr(char *dst, struct string *src)
{
	ASSERT_DEBUG(dst != NULL);
	ASSERT_DEBUG(src != NULL);
	memcpy(dst, src->start, src->size);
	dst[src->size] = 0;
}

struct string *
str_alloc_view(struct allocator *allocator, struct string *src)
{
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(src != NULL);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	s->start = src->start;
	s->is_owned = 0;
	s->size = src->size;
	return s;
}

struct string *
str_alloc_dup(struct allocator *allocator, struct string *src)
{
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(src != NULL);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	s->start = ALLOC(allocator, char, src->size);
	if (s->start == NULL)
	{
		FREE_STRUCT(allocator, s, string, 1);
		return NULL;
	}
	s->is_owned = 0;
	s->size = src->size;
	memcpy(s->start, src->start, src->size);
	return s;
}

struct string *
str_alloc_cstr_view(struct allocator *allocator, const char *src)
{
	usize length = 0;
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	length = strlen(src);
	s = str_alloc_cstr_view_slice(allocator, src, length);
	return s;
}

struct string *
str_alloc_cstr_view_slice(struct allocator *allocator, const char *src, usize length)
{
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	s->start = (char *)src;
	s->is_owned = 0;
	s->size = length;
	return s;
}

struct string *
str_alloc_cstr_dup(struct allocator *allocator, const char *src)
{
	usize length = 0;
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	length = strlen(src);
	s = str_alloc_cstr_dup_slice(allocator, src, length);
	return s;
}

struct string *
str_alloc_cstr_dup_slice(struct allocator *allocator, const char *src, usize length)
{
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	if (length > 0)
	{
		s->start = ALLOC(allocator, char, length);
		if (s->start == NULL)
		{
			FREE_STRUCT(allocator, s, string, 1);
			return NULL;
		}
		s->is_owned = !0;
		/* We do not copy the '\0'. */
		memcpy(s->start, src, length);
	}
	else
	{
		s->start = NULL;
		s->is_owned = 0;
	}
	s->size = length;
	return s;
}

struct string *
str_alloc_from_strl(struct allocator *allocator, struct string_list *strl)
{
	usize length = 0;
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(strl != NULL);
	length = strl_str_length(strl);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	s->start = ALLOC(allocator, char, length);
	if (s->start == NULL)
	{
		FREE_STRUCT(allocator, s, string, 1);
		return NULL;
	}
	s->is_owned = !0;
	s->size = length;
	strl_copy(s->start, strl);
	return s;
}

void
str_free(struct allocator *allocator, struct string *s)
{
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(s != NULL);
	if (s->is_owned)
	{
		FREE(allocator, s->start, char, s->size);
	}
	FREE_STRUCT(allocator, s, string, 1);
}

struct string_list *
strl_alloc(struct allocator *allocator)
{
	struct string_list *strl = 0;
	ASSERT_DEBUG(allocator != NULL);
	strl = ALLOC_STRUCT(allocator, string_list, 1);
	if (strl == NULL)
	{
		return NULL;
	}
	strl->allocator = allocator;
	strl->head = NULL;
	strl->last = NULL;
	return strl;
}

int
strl_append_view(struct string_list *strl, struct string *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return 0;
	}
	node->string = str_alloc_view(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return 0;
	}
	node->next = NULL;
	if (strl->last == NULL)
	{
		strl->head = node;
	}
	else
	{
		strl->last->next = node;
	}
	strl->last = node;
	return !0;
}

int
strl_append_dup(struct string_list *strl, struct string *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return 0;
	}
	node->string = str_alloc_dup(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return 0;
	}
	node->next = NULL;
	if (strl->last == NULL)
	{
		strl->head = node;
	}
	else
	{
		strl->last->next = node;
	}
	strl->last = node;
	return !0;
}

int
strl_append_cstr_dup(struct string_list *strl, const char *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return 0;
	}
	node->string = str_alloc_cstr_dup(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return 0;
	}
	node->next = NULL;
	if (strl->last == NULL)
	{
		strl->head = node;
	}
	else
	{
		strl->last->next = node;
	}
	strl->last = node;
	return !0;
}

int
strl_append_cstr_view(struct string_list *strl, const char *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return 0;
	}
	node->string = str_alloc_cstr_view(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return 0;
	}
	node->next = NULL;
	if (strl->last == NULL)
	{
		strl->head = node;
	}
	else
	{
		strl->last->next = node;
	}
	strl->last = node;
	return !0;
}

void
strl_free(struct string_list *strl)
{
	struct string_list_node *node = 0;
	struct string_list_node *next = 0;
	ASSERT_DEBUG(strl != NULL);
	node = strl->head;
	while (node != NULL)
	{
		next = node->next;
		str_free(strl->allocator, node->string);
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		node = next;
	}
	FREE_STRUCT(strl->allocator, strl, string_list, 1);
}

usize
strl_str_length(struct string_list *strl)
{
	struct string_list_node *node = 0;
	usize length = 0;
	ASSERT_DEBUG(strl != NULL);
	node = strl->head;
	while (node != NULL)
	{
		length += node->string->size;
		node = node->next;
	}
	return length;
}

void
strl_copy(char *dst, struct string_list *src)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(src != NULL);
	node = src->head;
	while (node != NULL)
	{
		memcpy(dst, node->string->start, node->string->size);
		dst += node->string->size;
		node = node->next;
	}
}

int
test_assert(int test, const char *test_cstr, const char *file, u32 line)
{
	if (test)
	{
		fprintf(stderr, "TEST OK   %s:%u: %s\n", file, line, test_cstr);
	}
	else
	{
		fprintf(stderr, "TEST FAIL %s:%u: %s\n", file, line, test_cstr);
		exit(1);
	}
	return test;
}
