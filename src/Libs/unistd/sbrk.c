#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL1(sbrk,  SYS_SBRK, int);
/*
void *sbrk(intptr_t increment) {
	return (void *)syscall_sbrk(increment);
}*/
