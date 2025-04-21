#include <Libs/Malloc/Mmu_paging.h>

#ifndef _MMU_HEAP_H
#define _MMU_HEAP_H


#define HEAP_WALKER(name) for(mblock_t *name = root_mblock; name != NULL; name = name->next) 

#define MBLOCK_MAGIC 0xDEAD 
#define END_OF_INITIALISED_HEAP PAGE_SIZE
extern void* __user_app_end;

struct mblock_struct {
  uint32_t  size;  
  uint16_t  magic;
  uint16_t  free;
  struct mblock_struct*  next;
};
typedef struct mblock_struct mblock_t;

extern uint64_t heap_start;
extern uint64_t heap_end;

void *malloc(uint32_t size);
void *calloc(size_t num, size_t size);
void free(void* size);
void debug_heap_dump();
void init_mmu_heap();

#endif
