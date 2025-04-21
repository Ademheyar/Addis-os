#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>

#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL2(symlink, SYS_SYMLINK, const char *, const char *);
/*
int symlink(const char *target, const char *name) {
	__sets_errno(syscall_symlink(target, name));
}
*/
