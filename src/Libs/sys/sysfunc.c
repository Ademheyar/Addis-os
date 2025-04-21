#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Sys/Sysfunc.h>

/*DEFN_SYSCALL2(system_function, SYS_SYSFUNC, int, char **);

extern int sysfunc(int command, char ** args) {
	__sets_errno(syscall_system_function(command, args));
}*/
