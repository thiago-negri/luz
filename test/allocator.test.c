#include "luz/luz.h"

static void
arena_align(void)
{
	struct allocator libc = { 0 };
	struct allocator arena = { 0 };
	struct arena_allocator ctx = { 0 };
	u8 *data = NULL;
	u8 *p = NULL;
	usize size = 0;
	TEST_BEGIN
	{
		size = 10;
		allocator_libc(&libc);
		data = ALLOC(&libc, u8, size);
		allocator_arena(&arena, &ctx, data, size);
		p = ALLOC(&arena, u8, 1);
		TEST_ASSERT(p == data);
		p = ALLOC(&arena, i32, 1);
		TEST_ASSERT(p == &data[4]);
	}
	TEST_END;
	if (data != NULL)
	{
		FREE(&libc, data, u8, size);
	}
}

int
main(void)
{
	arena_align();
	return 0;
}
