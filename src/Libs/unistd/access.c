#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

/*DEFN_SYSCALL2(access, SYS_ACCESS, char *, int);

int access(const char *pathname, int mode) {
	int result = syscall_access((char *)pathname, mode);
	if (result < 0) {
		errno = ENOENT; // XXX
		return -1;
	}
	return result;
}

*/