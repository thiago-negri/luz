#include "luz/luz.h"
#include <stdio.h>
#include <string.h>

struct foo
{
	const char *name;
};
DEFINE_ALIGNOF_STRUCT(foo);

static void
dynamic_array(void)
{
	struct allocator libc = { 0 };
	struct dynamic_array *da = NULL;
	struct foo temp = { 0 };
	struct foo *temp_array = NULL;
	int rc = 0;
	TEST_BEGIN
	{
		allocator_libc(&libc);
		da = da_alloc(&libc, sizeof(struct foo), ALIGNOF_STRUCT(foo), 10);
		TEST_ASSERT(da->count == 0 && da->capacity == 10);

		temp.name = "0001";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 1 && da->capacity == 10);

		temp.name = "0002";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 2 && da->capacity == 10);

		temp.name = "0003";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 3 && da->capacity == 10);

		temp.name = "0004";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 4 && da->capacity == 10);

		temp.name = "0005";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 5 && da->capacity == 10);

		temp.name = "0006";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 6 && da->capacity == 10);

		temp.name = "0007";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 7 && da->capacity == 10);

		temp.name = "0008";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 8 && da->capacity == 10);

		temp.name = "0009";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 9 && da->capacity == 10);

		temp.name = "0010";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 10 && da->capacity == 10);

		temp.name = "0011";
		rc = da_append_copy(da, &temp);
		TEST_ASSERT(rc && da->count == 11 && da->capacity == 15);

		temp_array = (struct foo *)da->data;
		TEST_ASSERT(strcmp(temp_array[0].name, "0001") == 0);
		TEST_ASSERT(strcmp(temp_array[1].name, "0002") == 0);
		TEST_ASSERT(strcmp(temp_array[2].name, "0003") == 0);
		TEST_ASSERT(strcmp(temp_array[3].name, "0004") == 0);
		TEST_ASSERT(strcmp(temp_array[4].name, "0005") == 0);
		TEST_ASSERT(strcmp(temp_array[5].name, "0006") == 0);
		TEST_ASSERT(strcmp(temp_array[6].name, "0007") == 0);
		TEST_ASSERT(strcmp(temp_array[7].name, "0008") == 0);
		TEST_ASSERT(strcmp(temp_array[8].name, "0009") == 0);
		TEST_ASSERT(strcmp(temp_array[9].name, "0010") == 0);
		TEST_ASSERT(strcmp(temp_array[10].name, "0011") == 0);
	}
	TEST_END;
	if (da != NULL)
	{
		da_free(da);
	}
}

int
main(void)
{
	dynamic_array();
	return 0;
}
