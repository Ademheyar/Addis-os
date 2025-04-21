#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Sys/stat.h>

//DEFN_SYSCALL2(chmod, SYS_CHMOD, char *, int);
/*
int chmod(const char *path, mode_t mode) {
	__sets_errno(syscall_chmod((char *)path, mode));
}
*/
