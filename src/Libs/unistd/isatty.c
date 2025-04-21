#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Sys/ioctl.h>
/*
int isatty(int fd) {
	int dtype = ioctl(fd, IOCTLDTYPE, NULL);
	if (dtype == IOCTL_DTYPE_TTY) return 1;
	errno = EINVAL;
	return 0;
}*/
