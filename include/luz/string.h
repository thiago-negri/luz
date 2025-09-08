#ifndef __LUZ_STRING_H__
#define __LUZ_STRING_H__

#include "alignof.h"
#include "gen/gen-defines.h"

/** A string with defined length, does not end with a '\0' byte. */
struct string
{
	usize length; /**< Length of the string. */
	char *start;  /**< Where the string starts (first character). */
};
DEFINE_ALIGNOF_STRUCT(string);

/** Copies a string to a C string, adds ending '\0' byte. */
void string_copy_cstr(char *dst,           /**< Destination. Must be able to hold (str->length + 1) bytes. */
                      struct string *src); /**< Source string. */

/** Initializes a string backed by a C string, does not copy the memory, i.e. dst->start == src. */
void string_from_cstr(struct string *dst, /**< Destination string. */
                      char *src);         /**< Source C string, must end with a '\0' byte. */

#endif /* ! __LUZ_STRING_H__ */
