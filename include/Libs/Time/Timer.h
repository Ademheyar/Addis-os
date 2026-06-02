#ifndef __TIMER_H
#define __TIMER_H

#include <Addis/Process.h>
#include <Addis/Libs/String/String.h>

#include <Libs/Stdio/Stdio.h>

#include <Libs/Stdint/Stdint.h>


#define TIMER_HZ 50

extern volatile uint64_t __tick;

void init_kernel_timer();
uint64_t timer_tick();
void timer_callback();

void timer_enable();
void timer_disable();

void krnl_delay(unsigned int d);

#endif
