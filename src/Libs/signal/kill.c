#include <Libs/signal/signal.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Errno/Errno.h>

/*DEFN_SYSCALL2(send_signal, SYS_KILL, uint32_t, uint32_t);

int kill(int pid, int sig) {
	__sets_errno(syscall_send_signal(pid, sig));
}

*/