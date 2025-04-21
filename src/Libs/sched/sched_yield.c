#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/sched/sched.h>
#include <Libs/Errno/Errno.h>

DEFN_SYSCALL0(yield, SYS_YIELD);

int sched_yield(void) {
	__sets_errno(syscall_yield());
}
