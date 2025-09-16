#include "luz/test.h"
#include <stdio.h>

int
test_assert(int test, const char *test_cstr, const char *file, u32 line)
{
	if (test)
	{
		fprintf(stderr, "TEST OK   %s:%u: %s\n", file, line, test_cstr);
	}
	else
	{
		fprintf(stderr, "TEST FAIL %s:%u: %s\n", file, line, test_cstr);
	}
	return test;
}
