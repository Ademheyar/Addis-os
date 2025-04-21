#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Errno/Errno.h>

/*DEFN_SYSCALL5(mount, SYS_MOUNT, char *, char *, char *, unsigned long, void *);

int mount(char * source, char * target, char * type, unsigned long flags, void * data) {
	__sets_errno(syscall_mount(source, target, type, flags, data));
}*/

