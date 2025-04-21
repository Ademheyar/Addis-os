#include <Libs/Time/Time.h>
#include <Libs/Sys/time.h>
#include <Libs/Stddef/Stddef.h>

/*
 * TODO: Also supposed to set tz values...
 */
char * ctime(const time_t * timep) {
    timep = timep;
    return NULL;//asctime(localtime(timep));
}
