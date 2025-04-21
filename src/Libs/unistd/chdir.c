#include <Libs/Unistd/Unistd.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Errno/Errno.h>

//DEFN_SYSCALL1(chdir, SYS_CHDIR, char *);
/*
int chdir(const char *path) {
	__sets_errno(syscall_chdir((char*)path));
}
*/
