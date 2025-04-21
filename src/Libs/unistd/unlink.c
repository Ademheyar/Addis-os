#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL1(unlink, SYS_UNLINK, char *);
/*
int unlink(const char * pathname) {
	__sets_errno(syscall_unlink((char *)pathname));
}*/
