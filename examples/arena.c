#include <luz/alignof.h>
#include <luz/allocator.h>
#include <luz/gen/gen-defines.h>
#include <stdio.h>
#include <string.h>

struct person
{
	char name[6];
	char age[3];
	u32 what; /**< This forces the structure to be aligned to 32 bits, which introduces padding. */
};
DEFINE_ALIGNOF_STRUCT(person); /**< This allows the usage of "ALIGNOF_STRUCT" macro. */

int
main(int argc, char *argv[])
{
	struct allocator libc_allocator = { 0 };
	struct allocator arena_allocator = { 0 };
	struct arena_allocator arena_ctx = { 0 };
	u8 *buffer = 0;
	struct person *bob = 0;
	struct person *alice = 0;
	struct person *jon = 0;
	char *first = 0;
	usize first_size = 0;
	char *second = 0;
	usize second_size = 0;
	char *end = 0;
	usize end_size = 0;
	usize buffer_size = 0;
	usize i = 0;
	usize sizeof_struct_person = 0;

	/* Mute warnings on unused args. */
	(void)argc;
	(void)argv;

	/* Initializes libc allocator (will use malloc/realloc/free). */
	allocator_libc(&libc_allocator);

	/* Allocates the buffer we want to use for the arena using libc's allocator. */
	buffer_size = 70;
	buffer = (u8 *)ALLOC(&libc_allocator, buffer_size, ALIGNOF(u8));

	/* Initializes the entire buffer as spaces so it's easier to visualize the padding. */
	memset(buffer, ' ', buffer_size);

	/* Initializes the arena allocator using the buffer we just allocated. */
	allocator_arena(&arena_allocator, &arena_ctx, buffer, buffer_size);

	sizeof_struct_person = sizeof(struct person);

	/* Allocates 2 bytes in the arena. This is the first allocation, should not produce any padding. */
	first_size = 2;
	first = ALLOC(&arena_allocator, first_size, ALIGNOF(char));

	/* Allocates "bob", as we are at offset 2, and person has an alignment of 4, this will produce padding in the
	 * underlying buffer. */
	bob = ALLOC(&arena_allocator, sizeof_struct_person, ALIGNOF_STRUCT(person));

	/* Allocates 5 bytes in the arena. Should not introduce any padding as ALIGNOF(char) is probably 1. */
	second_size = 5;
	second = ALLOC(&arena_allocator, second_size, ALIGNOF(char));

	/* Allocates "alice", as the previous allocation was 5 bytes, this will introduce padding as well. */
	alice = ALLOC(&arena_allocator, sizeof_struct_person, ALIGNOF_STRUCT(person));

	/* Allocates "jon", as the previous allocation was another person, this should not introduce padding. */
	jon = ALLOC(&arena_allocator, sizeof_struct_person, ALIGNOF_STRUCT(person));

	/* Allocates 1 extra byte. */
	end_size = 1;
	end = ALLOC(&arena_allocator, end_size, ALIGNOF(char));

	/* Make it easier to see in the underlying buffer where each piece of memory is. */
	memset(first, '1', first_size);
	memset(bob, 'B', sizeof_struct_person);
	memset(second, '2', second_size);
	memset(alice, 'A', sizeof_struct_person);
	memset(jon, 'J', sizeof_struct_person);
	memset(end, '.', end_size);

	/* Print out stuff to visualize. */
	printf("sizeof_struct_person = %lu\n", sizeof_struct_person);
	printf("ALIGNOF_STRUCT(person) = %lu\n", ALIGNOF_STRUCT(person));
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

	FREE(&libc_allocator, buffer, buffer_size);
	return 0;
}
