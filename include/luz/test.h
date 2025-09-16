#ifndef __LUZ_TEST_H__
#define __LUZ_TEST_H__

#include "gen/gen-defines.h"

int test_assert(int test, const char *test_cstr, const char *file, u32 line);

#define TEST_ASSERT(P)                                                                                                 \
	do                                                                                                             \
	{                                                                                                              \
		if (!test_assert(P, #P, __FILE__, __LINE__))                                                           \
		{                                                                                                      \
			goto done;                                                                                     \
		}                                                                                                      \
	}                                                                                                              \
	while (0)

#define TEST_BEGIN do

#define TEST_END                                                                                                       \
	while (0)                                                                                                      \
		;                                                                                                      \
	done:                                                                                                          \
	do                                                                                                             \
	{                                                                                                              \
	}                                                                                                              \
	while (0)

#endif /* ! __LUZ_TEST_H__ */
