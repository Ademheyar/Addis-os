#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

/*DEFN_SYSCALL3(readlink, SYS_READLINK, char *, char *, int);

ssize_t readlink(const char * name, char * buf, size_t len) {
	__sets_errno(syscall_readlink((char*)name, buf, len));
}*/

