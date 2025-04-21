#include <Libs/Stdio/Stdio.h>
#include <Libs/Unistd/Unistd.h>
/*
FILE * tmpfile(void) {
	static int tmpfile_num = 1;

	char tmp[100];
	sprintf(tmp, "/tmp/tmp%d.%d", getpid(), tmpfile_num++);

	FILE * out = fopen(tmp, "w+b");

	unlink(tmp);

	return out;
}
*/
