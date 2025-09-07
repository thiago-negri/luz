#include <errno.h>
#include <luz/luz.h>
#include <sys/stat.h>
#include <unistd.h>

u64 fs_timestamp(const char *path)
{
	int rc;
	struct stat file_stat;
	file_stat.st_mtime = 0;
	ASSERT_DEBUG(path != NULL);
	rc = stat(path, &file_stat);
	ASSERT_DEBUG(rc == 0);
	return file_stat.st_mtime;
}

void fs_delete(const char *path)
{
	int rc;
	ASSERT_DEBUG(path != NULL);
	rc = unlink(path);
	ASSERT_DEBUG(rc == 0);
}

void fs_mkdir(const char *path)
{
	int rc;
	rc = mkdir(path, 0700);
	ASSERT_DEBUG(rc == 0 || errno == EEXIST);
}
