#include "luz/da.h"
#include "luz/alloc.h"
#include <string.h>

struct dynamic_array *
da_alloc(struct allocator *a, usize item_size, usize item_align, usize capacity)
{
	struct dynamic_array *da = NULL;
	da = ALLOC_STRUCT(a, dynamic_array, 1);
	if (da == NULL)
	{
		return NULL;
	}
	da->allocator = a;
	da->item_size = item_size;
	da->item_align = item_align;
	da->capacity = capacity;
	da->count = 0;
	da->data = ALLOC_RAW(a, item_size * capacity, item_align);
	if (da->data == NULL)
	{
		FREE_STRUCT(a, da, dynamic_array, 1);
		return NULL;
	}
	return da;
}

int
da_ensure_capacity(struct dynamic_array *da, usize capacity)
{
	void *p = NULL;
	if (capacity <= da->capacity)
	{
		return !0;
	}
	p = REALLOC_RAW(da->allocator, da->data, da->item_size * da->capacity, da->item_size * capacity,
	                da->item_align);
	if (p == NULL)
	{
		return 0;
	}
	da->capacity = capacity;
	da->data = p;
	return !0;
}

int
da_append_copy(struct dynamic_array *da, void *item)
{
	if (da->count >= da->capacity)
	{
		if (!da_ensure_capacity(da, da->capacity * DYNAMIC_ARRAY_GROWTH_FACTOR))
		{
			return 0;
		}
	}
	memcpy(da->data + da->count * da->item_size, item, da->item_size);
	da->count += 1;
	return !0;
}

void
da_free(struct dynamic_array *da)
{
	FREE_RAW(da->allocator, da->data, da->item_size * da->capacity);
	FREE_STRUCT(da->allocator, da, dynamic_array, 1);
}
