#ifndef __LUZ_FS_H__
#define __LUZ_FS_H__

#include "gen/gen-defines.h"

/** Returns file timestamp. */
u64 fs_timestamp(const char *path);

/** Deletes a file. */
void fs_delete(const char *path);

/** Creates a directory. */
void fs_mkdir(const char *path);

#endif /* ! __LUZ_FS_H__ */
