#ifndef THREAD_H
#define THREAD_H
#include <System.h>
#include <Addis/Process.h>
#include <list.h>

// Thread control block
typedef struct tcb {
    list_t * self;
    pcb_t * parent;
    void * stack;
    int state;
    context_t regs;
}tcb_t;

#endif

