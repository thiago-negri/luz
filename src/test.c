#include "luz/test.h"
#include <stdio.h>
#include <stdlib.h>

bool
test_assert(bool test, const char *test_cstr, const char *file, u32 line)
{
	if (test)
	{
		fprintf(stdout, "TEST OK   %s:%u: %s\n", file, line, test_cstr);
	}
	else
	{
		fprintf(stderr, "TEST FAIL %s:%u: %s\n", file, line, test_cstr);
		exit(1);
	}
	return test;
}
