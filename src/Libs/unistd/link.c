#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>

// TODO:
//  We have a system call for this?
int link(const char *old, const char *new) {
	errno = EMLINK; old = old; new = new;
	return -1;
}
