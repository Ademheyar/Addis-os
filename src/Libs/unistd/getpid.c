#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL0(getpid, SYS_GETPID);
/*
pid_t getpid(void) {
	return syscall_getpid();
}
*/
pid_t getppid(void) {
	errno = ENOTSUP;
	return -1;
}
