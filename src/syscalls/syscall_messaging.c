
#include <Libs/Gui/windows/window.h>
#include <Libs/Gui/messags/messaging.h>
#include <Addis/Process.h>
#include <Kernel.h>
#include <Addis/Libs/String/String.h>
#include <Addis/Interrupt/Isr.h>

#define __UNUSED__ __attribute__((unused))

// syscall_messaging_get(message_t* msg_out, uint32_t handle)
long syscall_messaging_get(isr_ctx_t *regs) {
	/*message_t* msg = (message_t*)regs->rdi;
	window_m_t* win = window_m_find(regs->rsi);
	if (win != NULL) {
		// If no events - go to WAIT state
		if(!window_m_pop_message(win, msg)) {
			task_list_current->state = PROCESS_STATE_WAIT; // PROCESS_STATE_READY, PROCESS_STATE_RUNNING, PROCESS_STATE_WAIT
			schedule();
		}
	}
	return true;*/
	return regs->rdi;
}

long syscall_messaging_peek(isr_ctx_t *regs) {
	/*message_t* msg = (message_t*)regs->rdi;
	window_m_t* win = window_m_find(regs->rsi);
	if (win != NULL) {
		return window_m_pop_message(win, msg);
	}
	return true;*/
	return regs->rdi;
}

// syscall_messaging_create(message_t* msg, uint32_t handle)
long syscall_messaging_create(isr_ctx_t *regs) {
	/*message_t* msg = (message_t*)regs->rdi;
	window_m_t* win = window_m_find(regs->rsi);
	if (win != NULL) {
		window_m_add_message(win, msg);
	}
	return true;*/
	return regs->rdi;
}
