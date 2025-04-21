
#ifndef _STDDEF_H
#define _STDDEF_H

#include <Libs/Stdint/Stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef offsetof
#define offsetof(type, field) ((size_t)&((type *)0)->field)
#endif

void *alloca(size_t size);

#endif
