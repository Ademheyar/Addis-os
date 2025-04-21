#include <Libs/Unistd/Unistd.h>
#include <Libs/Sys/stat.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Errno/Errno.h>

//DEFN_SYSCALL2(fstat, SYS_STAT, int, void *);
/*
int fstat(int file, struct stat *st) {
	__sets_errno(syscall_fstat(file, st));
}*/
