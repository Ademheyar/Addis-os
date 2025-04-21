#include <Libs/Unistd/Unistd.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>

//DEFN_SYSCALL2(dup2, SYS_DUP2, int, int);
/*
int dup2(int oldfd, int newfd) {
	return syscall_dup2(oldfd, newfd);
}

int dup(int oldfd) {
	return dup2(oldfd, -1);
}*/
