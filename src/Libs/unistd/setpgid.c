#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

/*DEFN_SYSCALL2(setpgid, SYS_SETPGID, int, int);
//DEFN_SYSCALL1(getpgid, SYS_GETPGID, int);

int setpgid(pid_t pid, pid_t pgid) {
	__sets_errno(syscall_setpgid((int)pid,(int)pgid));
}

pid_t getpgid(pid_t pid) {
	__sets_errno(syscall_getpgid((int)pid));
}

*/