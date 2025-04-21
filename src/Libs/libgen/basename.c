#include <Libs/libgen/libgen.h>
#include <Libs/Stdlib/Stdlib.h>
#include <Addis/Libs/String/String.h>

char * basename(char * path) {
	char * s = path;
	char * c = NULL;
	do {
		while (*s == '/') {
			*s = '\0'; s++;
			if (!*s) goto _done;
		}
		c = s;
		s = strchr(c,'/');
	} while (s);

_done:

	if (!c) {
		return "/";
	}

	return c;
}
