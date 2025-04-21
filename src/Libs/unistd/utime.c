#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>

int utime(const char *filename, const struct utimbuf *times) {
	/* Unimplemented */
	errno = ENOTSUP; filename = filename; times = times;
	return -1;
}
