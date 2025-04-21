#include <Addis/Drivers/Filse_system/hd_Driver.h>

#ifndef __TIMER_H
#define __TIMER_H


#define TIMER_HZ 50

extern volatile uint64_t __tick;

void init_kernel_timer();
uint64_t timer_tick();
void timer_callback();

void timer_enable();
void timer_disable();

void krnl_delay(unsigned int d);

#endif
