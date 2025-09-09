#include <luz/allocator.h>
#include <luz/assert.h>
#include <luz/string.h>
#include <stdio.h>

void
print_sb(struct allocator *a, struct string_builder *sb)
{
	usize length = 0;
	char *cstr = 0;
	length = sb_length(sb);
	cstr = ALLOC(a, length + 1, ALIGNOF(char));
	sb_copy(cstr, sb);
	cstr[length] = 0;
	printf("%s\n", cstr);
	FREE(a, cstr, length + 1);
}

int
main(void)
{
	const char quote[] = "A nice day for coding, isn't it?";
	struct allocator allocator = { 0 };
	struct string_builder *sb = 0;
	struct string *str = 0;

	allocator_libc(&allocator);

	/* Hello, World! */
	{
		sb = sb_alloc(&allocator);

		sb_append_cstr_view(sb, "Hello,");
		sb_append_cstr_view(sb, " World!");

		print_sb(&allocator, sb);

		sb_free(sb);
	}

	/* nice coding */
	{
		sb = sb_alloc(&allocator);

		str = str_alloc_cstr_view_slice(&allocator, &quote[2], 4); /* nice */
		sb_append_view(sb, str);
		str_free(&allocator, str);

		sb_append_cstr_view(sb, " ");

		str = str_alloc_cstr_view_slice(&allocator, &quote[15], 6); /* coding */
		sb_append_view(sb, str);
		str_free(&allocator, str);

		print_sb(&allocator, sb);
		sb_free(sb);
	}

	return 0;
}
/*******************************************************************************************
Hello, World!
nice coding
********************************************************************************************/
