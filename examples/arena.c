#include <luz/alignof.h>
#include <luz/allocator.h>
#include <luz/gen/gen-defines.h>
#include <stdio.h>
#include <string.h>

struct person
{
	char name[6]; /**< Check the produced buffer and move the declarations around, you'll notice that the compiler
	                 changes the order of the fields. */
	char age[3];
	u32 what; /**< This forces the structure to be aligned to 32 bits, which introduces padding. */
};
DEFINE_ALIGNOF_STRUCT(person);

int
main(int argc, char *argv[])
{
	struct allocator libc_allocator = { 0 };
	struct allocator arena_allocator = { 0 };
	struct arena_allocator arena_ctx = { 0 };
	u8 *buffer = 0;
	struct person *bob = 0;
	struct person *alice = 0;
	char *first = 0;
	usize first_size = 0;
	char *second = 0;
	usize second_size = 0;
	char *end = 0;
	usize end_size = 0;
	usize buffer_size = 0;
	usize i = 0;
	usize sizeof_struct_person = 0;

	(void)argc;
	(void)argv;

	allocator_libc(&libc_allocator);
	buffer_size = 50;
	buffer = (u8 *)ALLOC(&libc_allocator, buffer_size, ALIGNOF(u8));
	memset(buffer, ' ', buffer_size); /**< Initialize the entire buffer to ' '. */

	allocator_arena(&arena_allocator, &arena_ctx, buffer, 1024);

	sizeof_struct_person = sizeof(struct person);
	first_size = 2;
	first = ALLOC(&arena_allocator, first_size, ALIGNOF(char));
	bob = ALLOC(&arena_allocator, sizeof_struct_person, ALIGNOF_STRUCT(person));
	second_size = 5;
	second = ALLOC(&arena_allocator, second_size, ALIGNOF(char));
	alice = ALLOC(&arena_allocator, sizeof_struct_person, ALIGNOF_STRUCT(person));
	end_size = 1;
	end = ALLOC(&arena_allocator, end_size, ALIGNOF(char));

	printf("sizeof_struct_person = %lu\n", sizeof_struct_person);
	printf("ALIGNOF_STRUCT(person) = %lu\n", ALIGNOF_STRUCT(person));

	memset(first, '1', 2);
	memset(bob, 'B', sizeof_struct_person);
	memset(second, '2', 5);
	memset(alice, 'A', sizeof_struct_person);
	memset(end, '.', 1);

	printf("\nBUFFER (any space before '.' is padding):\n[");
	for (i = 0; i < buffer_size; i++)
	{
		if (buffer[i] == 0)
		{
			printf("0");
		}
		else
		{
			putc(buffer[i], stdout);
		}
	}
	printf("]\n");
	putc(' ', stdout);
	for (i = 0; i < buffer_size; i++)
	{
		if (arena_ctx.offset == i)
		{
			printf("^ the arena offset is here.\n");
			break;
		}
		else
		{
			putc(' ', stdout);
		}
	}

	FREE(&libc_allocator, buffer, 1024);
	return 0;
}
