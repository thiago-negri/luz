#include "luz/string.h"
#include "luz/assert.h"
#include <string.h>

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
	s->is_owned = FALSE;
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
	s->start = ALLOC(allocator, src->size, ALIGNOF(char));
	if (s->start == NULL)
	{
		FREE_STRUCT(allocator, s, string, 1);
		return NULL;
	}
	s->is_owned = TRUE;
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
	s->is_owned = FALSE;
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
		s->start = ALLOC(allocator, length, ALIGNOF(char));
		if (s->start == NULL)
		{
			FREE_STRUCT(allocator, s, string, 1);
			return NULL;
		}
		s->is_owned = TRUE;
		/* We do not copy the '\0'. */
		memcpy(s->start, src, length);
	}
	else
	{
		s->start = NULL;
		s->is_owned = FALSE;
	}
	s->size = length;
	return s;
}

struct string *
str_alloc_from_sb(struct allocator *allocator, struct string_builder *sb)
{
	usize length = 0;
	struct string *s = 0;
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(sb != NULL);
	length = sb_length(sb);
	s = ALLOC_STRUCT(allocator, string, 1);
	if (s == NULL)
	{
		return NULL;
	}
	s->start = ALLOC(allocator, length, ALIGNOF(char));
	if (s->start == NULL)
	{
		FREE_STRUCT(allocator, s, string, 1);
		return NULL;
	}
	s->is_owned = TRUE;
	s->size = length;
	sb_copy(s->start, sb);
	return s;
}

void
str_free(struct allocator *allocator, struct string *s)
{
	ASSERT_DEBUG(allocator != NULL);
	ASSERT_DEBUG(s != NULL);
	if (s->is_owned)
	{
		FREE(allocator, s->start, s->size);
	}
	FREE_STRUCT(allocator, s, string, 1);
}

struct string_builder *
sb_alloc(struct allocator *allocator)
{
	struct string_builder *sb = 0;
	ASSERT_DEBUG(allocator != NULL);
	sb = ALLOC_STRUCT(allocator, string_builder, 1);
	if (sb == NULL)
	{
		return NULL;
	}
	sb->allocator = allocator;
	sb->head = NULL;
	sb->last = NULL;
	return sb;
}

bool
sb_append_view(struct string_builder *sb, struct string *s)
{
	struct string_list *node = 0;
	ASSERT_DEBUG(sb != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(sb->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_view(sb->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(sb->allocator, node, string_list, 1);
		return FALSE;
	}
	node->next = NULL;
	if (sb->last == NULL)
	{
		sb->head = node;
	}
	else
	{
		sb->last->next = node;
	}
	sb->last = node;
	return TRUE;
}

bool
sb_append_dup(struct string_builder *sb, struct string *s)
{
	struct string_list *node = 0;
	ASSERT_DEBUG(sb != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(sb->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_dup(sb->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(sb->allocator, node, string_list, 1);
		return FALSE;
	}
	node->next = NULL;
	if (sb->last == NULL)
	{
		sb->head = node;
	}
	else
	{
		sb->last->next = node;
	}
	sb->last = node;
	return TRUE;
}

bool
sb_append_cstr_dup(struct string_builder *sb, const char *s)
{
	struct string_list *node = 0;
	ASSERT_DEBUG(sb != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(sb->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_cstr_dup(sb->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(sb->allocator, node, string_list, 1);
		return FALSE;
	}
	node->next = NULL;
	if (sb->last == NULL)
	{
		sb->head = node;
	}
	else
	{
		sb->last->next = node;
	}
	sb->last = node;
	return TRUE;
}

bool
sb_append_cstr_view(struct string_builder *sb, const char *s)
{
	struct string_list *node = 0;
	ASSERT_DEBUG(sb != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(sb->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_cstr_view(sb->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(sb->allocator, node, string_list, 1);
		return FALSE;
	}
	node->next = NULL;
	if (sb->last == NULL)
	{
		sb->head = node;
	}
	else
	{
		sb->last->next = node;
	}
	sb->last = node;
	return TRUE;
}

void
sb_free(struct string_builder *sb)
{
	struct string_list *node = 0;
	struct string_list *next = 0;
	ASSERT_DEBUG(sb != NULL);
	node = sb->head;
	while (node != NULL)
	{
		next = node->next;
		str_free(sb->allocator, node->string);
		FREE_STRUCT(sb->allocator, node, string_list, 1);
		node = next;
	}
	FREE_STRUCT(sb->allocator, sb, string_builder, 1);
}

usize
sb_length(struct string_builder *sb)
{
	struct string_list *node = 0;
	usize length = 0;
	ASSERT_DEBUG(sb != NULL);
	node = sb->head;
	while (node != NULL)
	{
		length += node->string->size;
		node = node->next;
	}
	return length;
}

void
sb_copy(char *dst, struct string_builder *src)
{
	struct string_list *node = 0;
	ASSERT_DEBUG(src != NULL);
	node = src->head;
	while (node != NULL)
	{
		memcpy(dst, node->string->start, node->string->size);
		dst += node->string->size;
		node = node->next;
	}
}
