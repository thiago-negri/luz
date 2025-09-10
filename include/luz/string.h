#ifndef __LUZ_STRING_H__
#define __LUZ_STRING_H__

#include "alignof.h"
#include "allocator.h"
#include "bool.h"
#include "gen/gen-defines.h"

/** A string with defined size, does not end with a '\0' byte. */
/* TODO 2025-09-09 Need easier ways to print strings to FILE*, update examples/string_list.c */
struct string
{
	usize size;    /**< Size of the string. */
	bool is_owned; /**< Whether the start pointer is owned by this string and must be freed. */
	char *start;   /**< Where the string starts (first character). */
};
DEFINE_ALIGNOF_STRUCT(string);

/** Node for list of strings. */
struct string_list_node
{
	struct string *string;         /**< This node's string. */
	struct string_list_node *next; /**< Next node, NULL if this is the last one. */
};
DEFINE_ALIGNOF_STRUCT(string_list_node);

/** List of strings, will allocate more memory as it needs. */
struct string_list
{
	struct allocator *allocator;   /**< Allocator to use when we need more memory, and to free when done. */
	struct string_list_node *head; /**< First node, NULL if list is empty. */
	struct string_list_node *last; /**< Pointer to the last node for faster inserts, NULL if list is empty. */
};
DEFINE_ALIGNOF_STRUCT(string_list);

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

/** Allocates a string from a string list. */
struct string *str_alloc_from_sl(struct allocator *allocator, /**< Allocator to use. */
                                 struct string_list *strl);   /**< Source. */

/** Frees a string. */
void str_free(struct allocator *allocator, /**< Same allocator used to alloc. */
              struct string *s);           /**< String to be freed. */

/** Allocates a new string list. */
struct string_list *strl_alloc(struct allocator *allocator); /**< Allocator to use. */

/** Appends a string to a string list, the string list creates a shallow copy of the string, i.e. the underlying
 * bytes of the string are not copied. Returns TRUE if successful. FALSE means it failed to allocate memory. */
bool strl_append_view(struct string_list *strl, /**< Destination. */
                      struct string *s);        /**< Source. */

/** Appends a string to a string list, the string list creates a deep copy of the string, i.e. the underlying
 * bytes of the string are copied. Returns TRUE if successful. FALSE means it failed to allocate memory. */
bool strl_append_dup(struct string_list *strl, /**< Destination. */
                     struct string *s);        /**< Source. */

/** Appends a C string to a string list, copies the underlying bytes. */
bool strl_append_cstr_dup(struct string_list *strl, /**< Destination. */
                          const char *s);           /**< Source. */

/** Appends a C string to a string list, do not copy the underlying bytes. */
bool strl_append_cstr_view(struct string_list *strl, /**< Destination. */
                           const char *s);           /**< Source. */

/** Frees a string list. */
void strl_free(struct string_list *strl); /**< String list to be freed. */

/** Gets the current total length of the string list (sum the length of all strings in the list). */
usize strl_str_length(struct string_list *strl);

/** Copies all bytes from the string list, will not append a '\0'. */
void strl_copy(char *dst,                /**< Destination, must be able to hold strl_str_length(src) bytes. */
               struct string_list *src); /**< Source. */

#endif /* ! __LUZ_STRING_H__ */
