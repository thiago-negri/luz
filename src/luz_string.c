#include <luz/luz.h>
#include <string.h>

void string_to_cstr(char *dst, struct string *src)
{
	usize length;
	ASSERT_DEBUG(dst != NULL);
	ASSERT_DEBUG(src != NULL);
	length = src->length;
	memcpy(dst, src->start, length);
	dst[length] = 0;
}
