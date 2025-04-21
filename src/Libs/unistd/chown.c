#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL3(chown, SYS_CHOWN, char *, int, int);
/*
int chown(const char * pathname, uid_t owner, gid_t group) {
	__sets_errno(syscall_chown((char*)pathname,owner,group));
}
*/
