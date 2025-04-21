#include <Drivers/clock/clock.h>
#include <Kernel.h>
#include <Addis/Interrupt/Isr.h>

// syscall_clock_get_time()
uint64_t syscall_get_clock(isr_ctx_t *regs __UNUSED__) 
{
	return get_hw_datetime();
}
