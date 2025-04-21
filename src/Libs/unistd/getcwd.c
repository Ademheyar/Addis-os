#include <Libs/Unistd/Unistd.h>
#include <Addis/Libs/String/String.h>
#include <Libs/Syscalls.h>
#include <Libs/Syscall_nums.h>
#include <Libs/Malloc/Mmu_heap.h>

//DEFN_SYSCALL2(getcwd, SYS_GETCWD, char *, size_t);
/*
char *getcwd(char *buf, size_t size) {
	if (!buf) buf = malloc(size);
	return (char *)syscall_getcwd(buf, size);
}*/

