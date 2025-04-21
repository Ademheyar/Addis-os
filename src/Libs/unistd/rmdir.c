#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>

int rmdir(const char *pathname) {pathname=pathname;
	errno = ENOTSUP;
	return -1;
}
