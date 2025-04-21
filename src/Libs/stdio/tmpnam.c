#include <Libs/Stdio/Stdio.h>
#include <Libs/Unistd/Unistd.h>
/*
static char _internal[L_tmpnam];

char * tmpnam(char * s) {
	//static int tmp_id = 1;

	if (!s) {
		s = _internal;
	}

	//sprintf(s, "/tmp/tmp%d.%d", getpid(), tmp_id++);

	return s;
}
*/
