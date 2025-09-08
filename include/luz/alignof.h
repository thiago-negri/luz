#ifndef __LUZ_ALIGNOF_H__
#define __LUZ_ALIGNOF_H__

#include "gen/gen-defines.h"
#include <stddef.h>

#define DEFINE_ALIGNOF(T) struct __luz_alignof_##T { char a; T v; }
#define DEFINE_ALIGNOF_STRUCT(T) struct __luz_alignof_struct_##T { char a; struct T v; }
#define ALIGNOF(T) offsetof(struct __luz_alignof_##T, v)
#define ALIGNOF_STRUCT(T) offsetof(struct __luz_alignof_struct_##T, v)

DEFINE_ALIGNOF(char);

DEFINE_ALIGNOF(u8);
DEFINE_ALIGNOF(i8);

DEFINE_ALIGNOF(u16);
DEFINE_ALIGNOF(i16);

DEFINE_ALIGNOF(u32);
DEFINE_ALIGNOF(i32);

DEFINE_ALIGNOF(u64);
DEFINE_ALIGNOF(i64);

DEFINE_ALIGNOF(f32);
DEFINE_ALIGNOF(f64);

#endif /* ! __LUZ_ALIGNOF_H__ */
