#ifndef __ADDISOS_H
#define __ADDISOS_H

#include <Libs/Syscalls.h>
#include <Libs/Malloc/Mmu_frames.h>

//#define PAGE_SIZE (2*1024*1024)

void _debug(const char *fmt, ...);

void memory_stats(mmu_frame_stats_t *stats_out);

#endif
