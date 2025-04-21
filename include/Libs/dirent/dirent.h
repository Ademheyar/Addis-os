



#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdio/Stdio.h>
#include <Fs.h>
/*
#define ATTR_READ_ONLY	0x01
#define ATTR_HIDDEN		0x02
#define ATTR_SYSTEM		0x04
#define ATTR_VOLUME_ID	0x08
#define ATTR_DIRECTORY	0x10
#define ATTR_ARCHIVE	0x20
#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

typedef struct {
  DIRINFO dir_info;
} DIR;

typedef struct dirent {
	uint32_t d_ino;
	char d_name[256];
} dirent;*/

typedef struct DIR {
	int fd;
	int cur_entry;
} DIR;

DIR * opendir (const char * dirname);
int closedir (DIR * dir);
struct dirent * readdir (DIR * dirp);
int mkdir(const char *pathname, mode_t mode);
