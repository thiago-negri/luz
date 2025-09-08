#ifndef __LUZ_ALIGNOF_H__
#define __LUZ_ALIGNOF_H__

#include "gen/gen-defines.h"
#include <stddef.h>

/** Use this to allow usage of ALIGNOF on typedefs. */
#define DEFINE_ALIGNOF(T) struct __luz_alignof_##T { char a; T v; }

/** Use this to allow usage of ALIGNOF_STRUCT on structs. */
#define DEFINE_ALIGNOF_STRUCT(T) struct __luz_alignof_struct_##T { char a; struct T v; }

/** Use this to get the alignment of a typedef. */
#define ALIGNOF(T) offsetof(struct __luz_alignof_##T, v)

/** Use this to get the alignment of a struct. */
#define ALIGNOF_STRUCT(T) offsetof(struct __luz_alignof_struct_##T, v)

/* Some useful alignments to have out of the box. */
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
