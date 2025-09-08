#include "luz/string.h"
#include "luz/assert.h"
#include <string.h>

void
string_copy_cstr(char *dst, struct string *src)
{
	usize length = 0;
	ASSERT_DEBUG(dst != NULL);
	ASSERT_DEBUG(src != NULL);
	length = src->length;
	memcpy(dst, src->start, length);
	dst[length] = 0;
}

void
string_from_cstr(struct string *dst, char *src)
{
	usize length = 0;
	ASSERT_DEBUG(dst != NULL);
	length = strlen(src);
	dst->length = length;
	dst->start = src;
}
