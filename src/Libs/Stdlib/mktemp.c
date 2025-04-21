#include <Libs/Stdlib/Stdlib.h>
#include <Libs/Stdio/Stdio.h>
#include <Addis/Libs/String/String.h>
#include <Libs/Unistd/Unistd.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Fcntl.h>
/*
char * mktemp(char * template) {
	if (strstr(template + strlen(template)-6, "XXXXXX") != template + strlen(template) - 6) {
		errno = EINVAL;
		return NULL;
	}
	static int _i = 0;
	char tmp[7] = {0};
	//sprintf(tmp,"%04d%02d", getpid(), _i++);
	memcpy(template + strlen(template) - 6, tmp, 6);
	return template;
}

int mkstemp(char * template) {
	mktemp(template);
	return open(template, O_RDWR | O_CREAT, 0600);
}*/
