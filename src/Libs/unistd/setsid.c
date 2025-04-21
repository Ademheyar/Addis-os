#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL0(setsid, SYS_SETSID);
/*
pid_t setsid(void) {
	__sets_errno(syscall_setsid());
}
*/
