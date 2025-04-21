#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL1(pipe, SYS_PIPE, int *);
/*
int pipe(int fildes[2]) {
	__sets_errno(syscall_pipe((int *)fildes));
}
*/