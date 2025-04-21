#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

/*DEFN_SYSCALL3(read,  SYS_READ, int, char *, int);

int read(int file, void *ptr, size_t len) {
	__sets_errno(syscall_read(file,ptr,len));
}*/
