#ifndef __LUZ_ASSERT_H__
#define __LUZ_ASSERT_H__

#include "gen/gen-defines.h"

/** Asserts something must be true. Will exit the program if it's not. */
void assert(const char *file,      /**< File from where this is called. */
            u32 line,              /**< Line number where the assertion is made. */
            const char *check_str, /**< The stringified check. */
            int check);            /**< The check being made. */

/** Macro that should be used instead of calling assert directly. This will be kept in release mode. */
#define ASSERT_RELEASE(P) assert(__FILE__, __LINE__, #P, (P))

/** Static assertion. */
#define ASSERT_STATIC(P, M) typedef char __luz_assert_static_##M[(P) ? 1 : -1]

#ifndef NDEBUG

/** Macro that should be used instead of calling assert directly. Only exists in debug mode. */
#define ASSERT_DEBUG(P) assert(__FILE__, __LINE__, #P, (P))

#else /* NDEBUG */

/** No-op in release mode. */
#define ASSERT_DEBUG(P)

#endif /* NDEBUG */

#endif /* ! __LUZ_ASSERT_H__ */
