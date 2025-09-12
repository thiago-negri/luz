#ifndef __LUZ_ALLOCATOR_H__
#define __LUZ_ALLOCATOR_H__

#include "alignof.h"
#include "gen/gen-defines.h"

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

#endif /* ! __LUZ_ALLOCATOR_H__ */
