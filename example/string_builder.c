#include <luz/luz.h>
#include <stdio.h>

void
print_strl(struct allocator *a, struct string_list *strl)
{
	usize length = 0;
	usize size = 0;
	char *cstr = 0;
	length = strl_str_length(strl);
	size = length + 1;
	cstr = ALLOC(a, char, size);
	strl_copy(cstr, strl);
	cstr[length] = 0;
	printf("%s\n", cstr);
	FREE(a, cstr, char, size);
}

int
main(void)
{
	const char quote[] = "A nice day for coding, isn't it?";
	struct allocator allocator = { 0 };
	struct string_list *strl = 0;
	struct string *str = 0;

	allocator_libc(&allocator);

	/* Hello, World! */
	{
		strl = strl_alloc(&allocator);

		strl_append_cstr_view(strl, "Hello,");
		strl_append_cstr_view(strl, " World!");

		print_strl(&allocator, strl);

		strl_free(strl);
	}

	/* nice coding */
	{
		strl = strl_alloc(&allocator);

		str = str_alloc_cstr_view_slice(&allocator, &quote[2], 4); /* nice */
		strl_append_view(strl, str);
		str_free(&allocator, str);

		strl_append_cstr_view(strl, " ");

		str = str_alloc_cstr_view_slice(&allocator, &quote[15], 6); /* coding */
		strl_append_view(strl, str);
		str_free(&allocator, str);

		print_strl(&allocator, strl);
		strl_free(strl);
	}

	return 0;
}
/*******************************************************************************************
Hello, World!
nice coding
********************************************************************************************/
