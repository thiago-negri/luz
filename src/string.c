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
	s->start = ALLOC(allocator, char, src->size);
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
		s->start = ALLOC(allocator, char, length);
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
	s->is_owned = TRUE;
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

bool
strl_append_view(struct string_list *strl, struct string *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_view(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return FALSE;
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
	return TRUE;
}

bool
strl_append_dup(struct string_list *strl, struct string *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_dup(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return FALSE;
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
	return TRUE;
}

bool
strl_append_cstr_dup(struct string_list *strl, const char *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_cstr_dup(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return FALSE;
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
	return TRUE;
}

bool
strl_append_cstr_view(struct string_list *strl, const char *s)
{
	struct string_list_node *node = 0;
	ASSERT_DEBUG(strl != NULL);
	ASSERT_DEBUG(s != NULL);
	node = ALLOC_STRUCT(strl->allocator, string_list, 1);
	if (node == NULL)
	{
		return FALSE;
	}
	node->string = str_alloc_cstr_view(strl->allocator, s);
	if (node->string == NULL)
	{
		FREE_STRUCT(strl->allocator, node, string_list, 1);
		return FALSE;
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
	return TRUE;
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
