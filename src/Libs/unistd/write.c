#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL3(write, SYS_WRITE, int, char *, int);
/*
ssize_t write(int file, const void *ptr, size_t len) {
	__sets_errno(syscall_write(file,(char *)ptr,len));
}
*/