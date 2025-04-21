#ifndef SPINLOCK_T
#include <System.h>

void spinlock_lock(uint32_t * lock);

void spinlock_unlock(uint32_t * lock);

#endif
