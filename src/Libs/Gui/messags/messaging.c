/*#include <Libs/Gui/messags/messaging.h>
#include <Addis/Addisos.h>
#include <Addis/syscalls.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Libs/Stdint/Stdint.h>


void window_send_message(window_w_t* win, message_t* msg) {
	syscall(SYSCall_messaging_create, msg, win->handle);
}

void window_send_message_simple(window_w_t* win, int message) {
	message_t msg;
	msg.message = message;
	window_send_message(win, &msg);
}

bool window_get_message(window_w_t* win, message_t* msg) {
	return syscall(SYSCall_messaging_get, msg, win->handle);
}

bool window_peek_message(window_w_t* win, message_t* msg) {
return syscall(SYSCall_messaging_peek, msg, win->handle);	
}*/
