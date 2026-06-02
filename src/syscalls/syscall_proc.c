//#include <Libs/Gui/windows/window.h>
//#include <Libs/Malloc/Mmu_heap.h>

#include <Addis/Libs/String/String.h>
#include <Addis/Process.h>
#include <Kernel.h>
#include <Addis/Interrupt/Isr.h>



/************************************************************************************/
/* this is can be called by user to commincation with kernel                       */
/*                                                                                 */
/*                                                                                 */
/*                                                                                 */
/*                                                                                 */
/***********************************************************************************/

// syscall_process_create(void* elf_data)
// this wites for user to call it
// it creates there app and add new task sand the id back
uint64_t syscall_process_create(isr_ctx_t *regs) {
	void *elf_data = (void*)regs->rdi;
	task_t *task = create_user_process(elf_data);
	
	return task != NULL ? task->id : 0;
}

// syscall_process_create(void* elf_data)
uint64_t syscall_process_from_file(isr_ctx_t *regs) {
	char *filename = (char*)regs->rdi;
	task_t *task = create_user_process_file(filename);
	return task != NULL ? task->id : 0;
}

// syscall_process_exit(int exit_code)
uint64_t syscall_process_exit(isr_ctx_t *regs __UNUSED__) {
	// int exit_code = regs->rdi;
	return kill_process_id(task_list_current->id);
}

uint64_t syscall_process_get_id(isr_ctx_t *regs __UNUSED__) {
	return task_list_current->id;
}

// syscall_process_clone(void* entry_point, void* task_stack)
uint64_t syscall_process_clone(isr_ctx_t *regs __UNUSED__) {
	uint64_t entry_point = regs->rdi;
	uint64_t stack_address = regs->rsi;
	task_t *task = create_user_process_clone(task_list_current, entry_point, stack_address);
	return task != NULL ? task->id : 0;
}
