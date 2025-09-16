#ifndef __LUZ_H__
#define __LUZ_H__

#include "gen/gen-defines.h"
#include <stddef.h>

/** Use this to allow usage of ALIGNOF on typedefs. */
#define DEFINE_ALIGNOF(T)                                                                                              \
	struct __luz_alignof_##T                                                                                       \
	{                                                                                                              \
		char a;                                                                                                \
		T v;                                                                                                   \
	}

/** Use this to allow usage of ALIGNOF_STRUCT on structs. */
#define DEFINE_ALIGNOF_STRUCT(T)                                                                                       \
	struct __luz_alignof_struct_##T                                                                                \
	{                                                                                                              \
		char a;                                                                                                \
		struct T v;                                                                                            \
	}

/** Use this to get the alignment of a typedef. */
#define ALIGNOF(T) offsetof(struct __luz_alignof_##T, v)

/** Use this to get the alignment of a struct. */
#define ALIGNOF_STRUCT(T) offsetof(struct __luz_alignof_struct_##T, v)

/* Some useful alignments to have out of the box. */
DEFINE_ALIGNOF(char);
DEFINE_ALIGNOF(u8);
DEFINE_ALIGNOF(i8);
DEFINE_ALIGNOF(u16);
DEFINE_ALIGNOF(i16);
DEFINE_ALIGNOF(u32);
DEFINE_ALIGNOF(i32);
DEFINE_ALIGNOF(u64);
DEFINE_ALIGNOF(i64);
DEFINE_ALIGNOF(f32);
DEFINE_ALIGNOF(f64);

/** Memory allocator */
struct allocator
{
	/** Allocates memory, returns pointer to allocated memory, or NULL if fails. */
	void *(*alloc)(usize size,       /**< Size of memory to allocate. */
	               usize align,      /**< Alignment of the memory to allocate. */
	               void *ctx,        /**< Allocator context. */
	               const char *file, /**< File from where the alloc is being called. */
	               u32 line);        /**< Line from where the alloc is being called. */
	/** Free memory. */
	void (*free)(void *p,          /**< Pointer of previously allocated memory. */
	             usize size,       /**< Size of allocated memory. */
	             void *ctx,        /**< Allocator context. */
	             const char *file, /**< File from where the free is being called. */
	             u32 line);        /**< Line from where the free is being called. */
	/** Re-allocates memory, returns pointer to the new allocated memory, or NULL if fails. */
	void *(*realloc)(void *p,          /**< Pointer of previously allocated memory. */
	                 usize old_size,   /**< Old (current) size of allocated memory. */
	                 usize new_size,   /**< New size of allocated memory. */
	                 usize align,      /**< Alignment of the memory to allocate. */
	                 void *ctx,        /**< Allocator context. */
	                 const char *file, /**< File from where the realloc is being called. */
	                 u32 line);        /**< Line from where the realloc is being called. */
	/** Context pointer that should be passed to each allocator call. */
	void *ctx;
};
DEFINE_ALIGNOF_STRUCT(allocator);

/** Arena memory allocator. */
struct arena_allocator
{
	u8 *buffer;   /**< Underlying memory buffer. */
	usize offset; /**< Current offset into the buffer. */
	usize size;   /**< Size (capacity) of the buffer. */
};
DEFINE_ALIGNOF_STRUCT(arena_allocator);

/** Debug allocator context. */
struct debug_allocator
{
	/* TODO(tnegri) 2025-09-09 Add a list of allocated objects that can be checked at any point to detect leaks. */
	const char *ref; /**< A reference string to be logged as part of each operation. */
};
DEFINE_ALIGNOF_STRUCT(debug_allocator);

/** Macro to use to allocate memory. */
#define ALLOC(A, T, N) ((A)->alloc(sizeof(T) * (N), ALIGNOF(T), (A)->ctx, __FILE__, __LINE__))

/** Macro for allocating memory to hold an array of N struct T. */
#define ALLOC_STRUCT(A, T, N) ((A)->alloc(sizeof(struct T) * (N), ALIGNOF_STRUCT(T), (A)->ctx, __FILE__, __LINE__))

/** Macro to use to free memory. */
#define FREE(A, P, T, N) ((A)->free((P), sizeof(T) * (N), (A)->ctx, __FILE__, __LINE__))

/** Macro for freeing memory to hold an array of N struct T. */
#define FREE_STRUCT(A, P, T, N) ((A)->free((P), sizeof(struct T) * (N), (A)->ctx, __FILE__, __LINE__))

/** Macro to use to realloc memory. */
#define REALLOC(A, P, T, O, N)                                                                                         \
	((A)->realloc((P), sizeof(T) * (O), sizeof(T) * (N), ALIGNOF(L), (A)->ctx, __FILE__, __LINE__))

/** Macro for reallocating memory to hold an array of (O->N) struct T. */
#define REALLOC_STRUCT(A, P, T, O, N)                                                                                  \
	((A)->realloc((P), sizeof(struct T) * (O), sizeof(struct T) * (N), ALIGNOF_STRUCT(T), (A)->ctx, __FILE__,      \
	              __LINE__))

/** Memory allocator using libc defaults (stdlib's malloc, free, and realloc). */
void allocator_libc(struct allocator *allocator);

/** Memory allocator using libc defaults (stdlib's malloc, free, and realloc), prints debug messages. */
/* TODO(tnegri) 2025-09-09 Docs and maybe return the context? (using libc to allocate anyway) */
void allocator_libc_debug(struct allocator *allocator, struct debug_allocator *ctx);

/** Initializes an arena allocator. */
void allocator_arena(struct allocator *allocator,   /**< Allocator interface to be initialized. */
                     struct arena_allocator *arena, /**< Arena allocator context to use. */
                     u8 *buffer,                    /**< Underlying buffer to use. */
                     usize size);                   /**< Size of buffer. */

/** Resets an arena allocator. */
void arena_reset(struct arena_allocator *arena); /**< Arena allocator to reset. */

/** Asserts something must be true. Will exit the program if it's not. */
void assert(const char *file,      /**< File from where this is called. */
            u32 line,              /**< Line number where the assertion is made. */
            const char *check_str, /**< The stringified check. */
            int check);            /**< The check being made. */

/** Macro that should be used instead of calling assert directly. This will be kept in release mode. */
#define ASSERT_RELEASE(P) assert(__FILE__, __LINE__, #P, (P))

/** Static assertion. */
#define ASSERT_STATIC(P, M) typedef char __luz_assert_static_##M[(P) ? 1 : -1]

#ifndef NDEBUG

/** Macro that should be used instead of calling assert directly. Only exists in debug mode. */
#define ASSERT_DEBUG(P) assert(__FILE__, __LINE__, #P, (P))

#else /* NDEBUG */

/** No-op in release mode. */
#define ASSERT_DEBUG(P)

#endif /* NDEBUG */

/** Returns file timestamp. */
u64 fs_timestamp(const char *path);

/** Deletes a file. */
void fs_delete(const char *path);

/** Creates a directory. */
void fs_mkdir(const char *path);

/** A string with defined size, does not end with a '\0' byte. */
/* TODO(tnegri) 2025-09-09 Need easier ways to print strings to FILE*, update examples/string_list.c */
struct string
{
	usize size;    /**< Size of the string. */
	char is_owned; /**< Whether the start pointer is owned by this string and must be freed. */
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
 * bytes of the string are not copied. Returns !0 if successful. 0 means it failed to allocate memory. */
int strl_append_view(struct string_list *strl, /**< Destination. */
                     struct string *s);        /**< Source. */

/** Appends a string to a string list, the string list creates a deep copy of the string, i.e. the underlying
 * bytes of the string are copied. Returns !0 if successful. 0 means it failed to allocate memory. */
int strl_append_dup(struct string_list *strl, /**< Destination. */
                    struct string *s);        /**< Source. */

/** Appends a C string to a string list, copies the underlying bytes. Returns !0 if successful. 0 means it failed to
 * allocate memory. */
int strl_append_cstr_dup(struct string_list *strl, /**< Destination. */
                         const char *s);           /**< Source. */

/** Appends a C string to a string list, do not copy the underlying bytes. Returns !0 if successful. 0 means it failed
 * to allocate memory.  */
int strl_append_cstr_view(struct string_list *strl, /**< Destination. */
                          const char *s);           /**< Source. */

/** Frees a string list. */
void strl_free(struct string_list *strl); /**< String list to be freed. */

/** Gets the current total length of the string list (sum the length of all strings in the list). */
usize strl_str_length(struct string_list *strl);

/** Copies all bytes from the string list, will not append a '\0'. */
void strl_copy(char *dst,                /**< Destination, must be able to hold strl_str_length(src) bytes. */
               struct string_list *src); /**< Source. */

int test_assert(int test, const char *test_cstr, const char *file, u32 line);

#define TEST_ASSERT(P)                                                                                                 \
	do                                                                                                             \
	{                                                                                                              \
		if (!test_assert(P, #P, __FILE__, __LINE__))                                                           \
		{                                                                                                      \
			goto done;                                                                                     \
		}                                                                                                      \
	}                                                                                                              \
	while (0)

#define TEST_BEGIN do

#define TEST_END                                                                                                       \
	while (0)                                                                                                      \
		;                                                                                                      \
	done:                                                                                                          \
	do                                                                                                             \
	{                                                                                                              \
	}                                                                                                              \
	while (0)

#endif /* ! __LUZ_H__ */
