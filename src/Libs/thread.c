#include <Libs/thread.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Libs/Addisos.h>

thread_t* thread_create(thread_function_t thread_function, uint32_t stack_size) {
	thread_t *thread = malloc(sizeof(thread_t));

	thread->stack_size = stack_size;
	thread->stack = malloc(stack_size);

	thread->id = syscall(SYSCall_process_clone, thread_function, thread->stack - stack_size - 8);

	if (thread->id == 0) {
		free(thread);
		return NULL;
	}
	return thread;
}
