#ifndef __LUZ_STRING_H__
#define __LUZ_STRING_H__

#include "alignof.h"
#include "allocator.h"
#include "bool.h"
#include "gen/gen-defines.h"

/** A string with defined size, does not end with a '\0' byte. */
struct string
{
	usize size;    /**< Size of the string. */
	bool is_owned; /**< Whether the start pointer is owned by this string and must be freed. */
	char *start;   /**< Where the string starts (first character). */
};
DEFINE_ALIGNOF_STRUCT(string);

/** Linked list of strings. */
struct string_list
{
	struct string *string;    /**< This node's string. */
	struct string_list *next; /**< Next node, NULL if this is the last one. */
};
DEFINE_ALIGNOF_STRUCT(string_list);

/** A string with dynamic size, will allocate more memory as it needs. */
struct string_builder
{
	struct allocator *allocator; /**< Allocator to use when we need more memory, and to free when done. */
	struct string_list *head;    /**< Where the string starts. */
	struct string_list *last;    /**< Pointer to the last node for faster inserts. */
};
DEFINE_ALIGNOF_STRUCT(string_builder);

/** Copies a string to a C string, appends ending '\0' byte. */
void str_copy_cstr(char *dst,           /**< Destination. Must be able to hold (str->size + 1) bytes. */
                   struct string *src); /**< Source string. */

/** Allocates a new string which is a shallow copy of another one, the underlying bytes are not copied. */
struct string *str_alloc_view(struct allocator *allocator, /**< Allocator to use. */
                              struct string *src);         /**< Source. */

/** Allocates a new string which is a deep copy of another one, the underlying bytes are copied. */
struct string *str_alloc_dup(struct allocator *allocator, /**< Allocator to use. */
                             struct string *src);         /**< Source. */

/** Allocates a string view into a C string, does not copy the memory, i.e. dst->start == src. */
struct string *str_alloc_cstr_view(struct allocator *allocator, /**< Allocator to use. */
                                   const char *src);            /**< Source C string, must end with a '\0' byte. */

/** Allocates a string view into a C string, does not copy the memory, i.e. dst->start == src. */
struct string *str_alloc_cstr_view_slice(struct allocator *allocator, /**< Allocator to use. */
                                         const char *src,             /**< Source C string. */
                                         usize length);               /**< Length of the string. */

/** Allocates a string that duplicates a C string, i.e. copies the memory. */
struct string *str_alloc_cstr_dup(struct allocator *allocator, /**< Allocator to use. */
                                  const char *src);            /**< Source C string, must end with a '\0' byte. */

/** Allocates a string that duplicates a C string, i.e. copies the memory. */
struct string *str_alloc_cstr_dup_slice(struct allocator *allocator, /**< Allocator to use. */
                                        const char *src,             /**< Source C string. */
                                        usize length);               /**< Length of the string. */

/** Allocates a string from a string builder. */
struct string *str_alloc_from_sb(struct allocator *allocator, /**< Allocator to use. */
                                 struct string_builder *sb);  /**< Source. */

/** Frees a string. */
void str_free(struct allocator *allocator, /**< Same allocator used to alloc. */
              struct string *s);           /**< String to be freed. */

/** Allocates a new string builder. */
struct string_builder *sb_alloc(struct allocator *allocator); /**< Allocator to use. */

/** Appends a string to a string builder, the string builder creates a shallow copy of the string, i.e. the underlying
 * bytes of the string are not copied. Returns TRUE if successful. FALSE means it failed to allocate memory. */
bool sb_append_view(struct string_builder *sb, /**< Destination. */
                    struct string *s);         /**< Source. */

/** Appends a string to a string builder, the string builder creates a deep copy of the string, i.e. the underlying
 * bytes of the string are copied. Returns TRUE if successful. FALSE means it failed to allocate memory. */
bool sb_append_dup(struct string_builder *sb, /**< Destination. */
                   struct string *s);         /**< Source. */

/** Appends a C string to a string builder, copies the underlying bytes. */
bool sb_append_cstr_dup(struct string_builder *sb, /**< Destination. */
                        const char *s);            /**< Source. */

/** Appends a C string to a string builder, do not copy the underlying bytes. */
bool sb_append_cstr_view(struct string_builder *sb, /**< Destination. */
                         const char *s);            /**< Source. */

/** Frees a string builder. */
void sb_free(struct string_builder *sb); /**< String builder to be freed. */

/** Gets the current total length of the string builder. */
usize sb_length(struct string_builder *sb);

/** Copies all bytes from the string builder, will not append a '\0'. */
void sb_copy(char *dst,                   /**< Destination, must be able to hold sb_length(src) bytes. */
             struct string_builder *src); /**< Source. */

#endif /* ! __LUZ_STRING_H__ */
