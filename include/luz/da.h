#ifndef __LUZ_DA_H__
#define __LUZ_DA_H__

#include "align.h"
#include "gen/gen-defines.h"

/** An array of dynamic size. */
struct dynamic_array
{
	struct allocator *allocator; /**< Allocator used to alloc/realloc the array as needed. */
	usize item_size;             /**< Size of one item. */
	usize item_align;            /**< Align of one item. */
	usize capacity;              /**< Current capacity of the array (max item count before a realloc is needed). */
	usize count;                 /**< Current count of items in the array. */
	u8 *data;                    /**< Memory block backing this array. */
};
DEFINE_ALIGNOF_STRUCT(dynamic_array);

/** How much a dynamic array grows when it needs to. */
#define DYNAMIC_ARRAY_GROWTH_FACTOR 1.5

/** Allocates a new dynamic array. */
struct dynamic_array *da_alloc(struct allocator *a, /**< Allocator to use. */
                               usize item_size,     /**< Size of an item. */
                               usize item_align,    /**< Align of an item. */
                               usize capacity);     /**< Initial capacity of the array. */

/** Ensure the dynamic array can hold at least N items. */
int da_ensure_capacity(struct dynamic_array *da, /**< Dynamic array. */
                       usize capacity);          /**< Minimal capacity it must hold. */

/** Appends an item (copies memory over), grows the dynamic array if necessary. */
int da_append_copy(struct dynamic_array *da, /**< Dynamic array. */
                   void *item);              /** The item to be copied into the array. */

/** Frees a dynamic array. */
void da_free(struct dynamic_array *da); /**< Dynamic array. */

#endif /* ! __LUZ_DA_H__ */
