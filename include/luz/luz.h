#ifndef __LUZ_H__
#define __LUZ_H__

#include "gen/gen-defines.h"

typedef u8 bool;

#define FALSE (0)
#define TRUE  (!0)

#define UNDEFINED_PTR ((void *)0xCAFEBABE)

/** Memory allocator */
struct allocator
{
	/** Allocates memory, returns pointer to allocated memory, or NULL if fails. */
	void *(*alloc)(usize size,       /**< Size of memory to allocate. */
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
	                 void *ctx,        /**< Allocator context. */
	                 const char *file, /**< File from where the realloc is being called. */
	                 u32 line);        /**< Line from where the realloc is being called. */
	/** Context pointer that should be passed to each allocator call. */
	void *ctx;
};

/** Macro to use to allocate memory. */
#define ALLOC(A, S) ((A)->alloc((S), (A)->ctx, __FILE__, __LINE__))

/** Macro to use to free memory. */
#define FREE(A, P, S) ((A)->free((P), (S), (A)->ctx, __FILE__, __LINE__))

/** Macro to use to realloc memory. */
#define REALLOC(A, P, O, N) ((A)->realloc((P), (O), (N), (A)->ctx, __FILE__, __LINE__))

/** Memory allocator using libc defaults (stdlib's malloc, free, and realloc). */
void allocator_libc(struct allocator *allocator);

/** Memory allocator using libc defaults (stdlib's malloc, free, and realloc), prints debug messages. */
void allocator_libc_debug(struct allocator *allocator);

/** Asserts something must be true. Will exit the program if it's not. */
void assert(const char *file,      /**< File from where this is called. */
            u32 line,              /**< Line number where the assertion is made. */
            const char *check_str, /**< The stringified check. */
            bool check);           /**< The check being made. */

/** Macro that should be used instead of calling assert directly. This will be kept in release mode. */
#define ASSERT_RELEASE(P) assert(__FILE__, __LINE__, #P, (P))

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

/** A string with defined length, does not end with a '\0' byte. */
struct string
{
	usize length; /**< Length of the string. */
	char *start;  /**< Where the string starts (first character). */
};

/** Copies a string to a C string, adds ending '\0' byte. */
void string_copy_cstr(char *dst,           /**< Destination. Must be able to hold (str->length + 1) bytes. */
                      struct string *src); /**< Source string. */

/** Initializes a string backed by a C string, does not copy the memory, i.e. dst->start == src. */
void string_from_cstr(struct string *dst, /**< Destination string. */
                      char *src);         /**< Source C string, must end with a '\0' byte. */

#endif /* ! __LUZ_H__ */
