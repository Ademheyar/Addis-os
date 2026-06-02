/*#include <Libs/Gui/windows/window.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Addis/Process.h>
#include <Kernel.h>
 #include <Addis/Libs/String/String.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
*/

#include <Addis/Interrupt/Isr.h>


#include <Libs/Stdint/Stdint.h>

// syscall_windows_create(int x, int y, int width, int height, char* title)
uint64_t syscall_windows_create(isr_ctx_t *regs) {
	/*int x = regs->rdi;
	int y = regs->rsi;
	int width = regs->rdx;
	int height = regs->rcx;
	uint32_t attributes = (uint32_t) regs->r8;
	window_m_t* win = Window_m(x, y, width, height, attributes);
	*/
	//return win->handle;
	return regs->rsi;
}

// syscall_windows_present(uint32_t handle, context_t*)
uint64_t syscall_windows_present(isr_ctx_t *regs) {
	regs=regs;
	/*window_m_t* win = window_m_find(regs->rdi);
	if (win != NULL) {
		context_t* context = (context_t*)regs->rsi;
		//window_m_present_context(win, context);
		return 1;
	}*/
	return 0;
}
