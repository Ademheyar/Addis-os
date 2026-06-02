#include <Addis/Process.h>
#include <Kernel.h>
 #include <Addis/Libs/String/String.h>
#include <Addis/Interrupt/Isr.h>

#define __UNUSED__ __attribute__((unused))

// syscall_debug_puts(char* string)
long syscall_debug_puts(isr_ctx_t *regs) {
	DEBUG("APP[%i]: %s\n", task_list_current->id, regs->rdi);
	return 1;
}

