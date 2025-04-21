#include <Libs/Stdint/Stdint.h>
#include <Libs/Stddef/Stddef.h>
#include <Libs/Sys/Sysfunc.h>

#define _XLOG(_msg) do { \
	char * msg[] = { \
		__FILE__, \
		(char*)__LINE__, \
		(char*)2, \
		_msg, \
	}; \
	sysfunc(TOARU_SYS_FUNC_DEBUGPRINT, msg); \
} while (0);

