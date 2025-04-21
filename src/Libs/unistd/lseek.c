#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

/*DEFN_SYSCALL3(lseek, SYS_SEEK, int, int, int);

off_t lseek(int file, off_t ptr, int dir) {
	__sets_errno(syscall_lseek(file,ptr,dir));
}

*/