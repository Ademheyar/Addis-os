#include <Libs/Gui/windows.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdlib/Stdlib.h>
 #include <Addis/Libs/String/String.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Libs/Gui/messags/messaging.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Libs/Addisos.h>
#include <Libs/Syscalls.h>
#include <Libs/Gui/Pictures/Bitmap/Bitmap.h>
#include <Libs/Gui/labels/label.h>


label_t *label_create(window_w_t *parent, int x, int y, int width, int height, char* title, int id) {
	label_t *window = calloc(sizeof(label_t), 1);

	// Init window structure
	window->type = WINDOW_TYPE_LABEL;
	window->id = id;
	window->x = x;
	window->y = y;
	window->height = height;
	window->width = width;
	window->default_procedure = &label_default_procedure;
	window->title = strdup(title);
	window->parent = parent;

	window_w_add_child(parent, window);
	return window;	
}

void label_set_text(label_t *label, const char *text) {
	free(label->title);
	label->title = strdup((char *)text);
	window_w_invalidate(label->parent);
}

bool label_default_procedure(label_t *label, struct message_struct *msg) {
	switch(msg->message) {
		case WINDOW_LIB_MESSAGE_CREATE:
			window_w_dispatch_message_simple(label, WINDOW_LIB_MESSAGE_PREDRAW);
			break;
		case WINDOW_LIB_MESSAGE_PREDRAW:
			on_label_predraw(label);
			window_w_dispatch_message_simple(label, WINDOW_LIB_MESSAGE_DRAW);
			break;
		case WINDOW_LIB_MESSAGE_DRAW:
			window_w_dispatch_message_simple(label, WINDOW_LIB_MESSAGE_PRESENT);
			break;
	}
	return false;
}

void on_label_predraw(label_t *label) {
	int x1 = label->x;
	int y1 = label->y;
	int x2 = x1 + label->width;
	int y2 = y1 + label->height;


	gfx_fillrect_c(WINDOW_EXT(label->parent)->context, x1, y1, x2, y2, LABEL_BACKGROUND_COLOR);
	gfx_puts_c(WINDOW_EXT(label->parent)->context, label->x, label->y, LABEL_TEXT_COLOR, LABEL_BACKGROUND_COLOR, label->title);
}
