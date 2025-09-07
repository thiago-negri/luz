#include <luz/luz.h>
#include <stdio.h>
#include <stdlib.h>

void assert(const char *file, u32 line, const char *check_str, bool check)
{
	if (!check)
	{
		fprintf(stderr, "%s:%u: (assert) %s\n", file, line, check_str);
		fflush(stderr);
		exit(1);
	}
}
