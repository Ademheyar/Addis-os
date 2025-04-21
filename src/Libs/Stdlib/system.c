#include <Libs/Stdlib/Stdlib.h>
#include <Libs/Unistd/Unistd.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Sys/Wait.h>

int system(const char * command) {
    command = command;
	/*char * args[] = {
		"/bin/sh",
		"-c",
		(char *)command,
		NULL,
	};
	pid_t pid = fork();
	if (!pid) {
		execvp(args[0], args);
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}*/
    return 0;
}
