#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Sys/ioctl.h>
#include <Libs/pty/pty.h>
#include <Libs/Errno/Errno.h>

/*DEFN_SYSCALL5(openpty, SYS_OPENPTY, int *, int *, char *, void *, void *);

int openpty(int * amaster, int * aslave, char * name, const struct termios *termp, const struct winsize * winp) {
	__sets_errno(syscall_openpty(amaster,aslave,name,(struct termios *)termp,(struct winsize *)winp));
}
*/