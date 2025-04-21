#include <Libs/Gui/windows.h>
#include <Libs/Addisos.h>
#include <Libs/Syscalls.h>
#include <Libs/Gui/Pictures/Bitmap/Bitmap.h>
#include <Libs/Gui/messags/messaging.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdlib/Stdlib.h>
 #include <Addis/Libs/String/String.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Kernel.h>
#include <Libs/Assert/Assert.h>
#include <Addis/Process.h>
#include <Addis/Interrupt/Isr.h>
#include <Drivers/mouse/mouse.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Libs/Stdio/Stdio.h>
#include <Libs/Time/Timer.h>

#include <Addis/Drivers/Screen/Vesa/Vesa.h>

int __old_mouse_x, __old_mouse_y;
short mouse_icon[] =  {
	0,1,1,1,0,0,0,0,0,0,0,
	0,1,2,2,1,0,0,0,0,0,0,
	0,1,2,2,2,1,0,0,0,0,0,
	0,1,2,2,2,2,1,0,0,0,0,
	0,1,2,2,2,2,2,1,0,0,0,
	0,1,2,2,2,2,2,2,1,0,0,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,2,2,2,2,2,2,2,2,1,
	0,1,2,2,2,2,2,2,2,2,1,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,2,2,2,2,2,2,1,0,0,
	0,1,2,2,2,2,2,1,0,0,0,
	0,1,2,2,1,1,2,2,1,0,0,
	0,1,2,1,0,1,2,2,2,1,0,
	0,1,1,0,0,0,1,2,2,1,0,
	0,0,0,0,0,0,0,1,1,0,0,
};
uint32_t mouse_color_mapping[] = {0, 0xFFFFFFFF, 0};

#define WIN_SORT_VAL(win) (win == NULL ? 1000000000 : win->z) 

window_m_t* window_m_list[MAX_WINDOW_M_COUNT];
window_m_t* focused_window;
Screen_info_t buffer_video_info;
uint64_t __last_update_tick = 0;
uint16_t __window_handle = 1000; // 1000 just magic number to start from (0 means no window)
int __window_count = 0;

bool __window_need_redraw = true;

window_m_t* window_to_drag = NULL;


void init_kernel_window_m_manager() {
	DEBUG("SIZEOF LIST :%i\n", sizeof(window_m_list));
	//memset(window_m_list, 0, MAX_WINDOW_M_COUNT * sizeof(window_m_t*));
	
	memcpy(&buffer_video_info, &screen_info, sizeof(Screen_info_t));
	uint32_t screen_size = VIDEO_INFO_MEM_SIZE(screen_info); 
	buffer_video_info.addr = (uint64_t)malloc(screen_size); // 
	DEBUG("WIN: Allocating double buffer size : %i (%i MB)\n", screen_size, screen_size / 1024 / 1024);
  DEBUG("WIN: Double Frame buffer addr: 0x%X\n", buffer_video_info.addr);
  DEBUG("WIN: Double Frame buffer linear addr: 0x%X\n", buffer_video_info.linear_addr);
  DEBUG("WIN: Double Frame buffer info: %i x %i : %ibpp\n", buffer_video_info.width, buffer_video_info.height, buffer_video_info.bits);
  DEBUG("WIN: Double Frame buffer pitch: %i\n", buffer_video_info.pitch);
  DEBUG("WIN: Double Frame buffer type: %i\n", buffer_video_info.type);
}


//////////////////////////////////////////////msg///////////////////////////////////////
void window_send_message(window_w_t* win, message_t* msg) {
	//syscall(SYSCall_messaging_create, msg, win->handle);
	window_m_t* win0 = window_m_find(win->handle);
	if (win0 != NULL) {
		window_m_add_message(win0, msg);
	}
}

void window_send_message_simple(window_w_t* win, int message) {
	message_t msg;
	msg.message = message;
	window_send_message(win, &msg);
}

bool window_get_message(window_w_t* win, message_t* msg) {
	window_m_t* win0 = window_m_find(win->handle);
	if (win0 != NULL) {
		// If no events - go to WAIT state
		if(!window_m_pop_message(win0, msg)) {
			//task_list_current->state = PROCESS_STATE_WAIT; // PROCESS_STATE_READY, PROCESS_STATE_RUNNING, PROCESS_STATE_WAIT
			schedule();
		}
	}
	return true;
	//return syscall(SYSCall_messaging_get, msg, win->handle);
}

bool window_peek_message(window_w_t* win, message_t* msg) {
return syscall(SYSCall_messaging_peek, msg, win->handle);
}

//////////////////////////////////////////////msg end////////////////////////////



//////////////////////////////////////////window manager ///////////////////////////////////

/*
void Bitmap(char *filename, Bitmap_t *bmp_out) {
	DEBUG("BMP FILE OPENING ...");
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	DEBUG("BMP FILE OPENED");
	uint32_t size = fsize(file);


	void *buffer = malloc(size + 512);
	DEBUG("BMP ALLOCATED ...");

	fread(buffer, size, 1, file);
	DEBUG("BMP READ ...");

	bmp_out->buffer = buffer;
	bmp_out->firster = (bmp_header_t*)bmp_out->buffer;
	bmp_out->data = (uint32_t*)(((uint8_t*)buffer) + bmp_out->firster->offset);
	// Usually header is negative (means that image is stored : top - to -bottom)
	bmp_out->firster->height_px = abs(bmp_out->firster->height_px);
}

void bmp_close(Bitmap_t *bmp_image) {
	free(bmp_image->buffer);
}
*/


// TODO: Optimise - move to gfx_blit
void window_draw_mouse() {
	short* buf = mouse_icon;
	for (int i=0; i<16; i++) {
		for (int j=0; j<11; j++) {
			if (*buf) {
				uint32_t color = mouse_color_mapping[*buf];
				if (mouse_buttons & MOUSE_LEFT_CLICK) {
					color ^= 0xFFFFFF;
				}
	    	if (mouse_x + j >= 0 && mouse_x + j < buffer_video_info.width &&
    		  mouse_y + i >= 0 && mouse_y + i < buffer_video_info.height) {
					((uint32_t*)buffer_video_info.addr)[(mouse_y + i) * buffer_video_info.width + mouse_x + j] = color;
				}
			}
			buf++;
		}
	}
}

static int find_window_index(window_m_t *window) {
	for (int i=0; i<MAX_WINDOW_M_COUNT; i++) {
		if (window_m_list[i] == window) {
			return i;
		}
	}
	return -1;
}

static bool add_window(window_m_t *window) {
	for(int i=0; i<MAX_WINDOW_M_COUNT; i++) {
		if (window_m_list[i] == NULL) {
			window_m_list[i] = window;
			return true;
		}
	}
	return false;
}

static int find_max_z() {
	int last_z = 0;
	for (int i=0; i<MAX_WINDOW_M_COUNT; i++) {
		if (window_m_list[i] != NULL && last_z < window_m_list[i]->z) {
			last_z = window_m_list[i]->z;
		}
	}
	return last_z;
}

void window_sort_windows() {
	uint32_t max_idx;
	int n = MAX_WINDOW_M_COUNT;
  for (int i = 0; i < n-1; i++) { 
    max_idx = i; 
    for (int j = i+1; j < n; j++) {
      if (WIN_SORT_VAL(window_m_list[j]) < WIN_SORT_VAL(window_m_list[max_idx])) {
        max_idx = j; 
      }
    }

    window_m_t* tmp = window_m_list[max_idx];
    window_m_list[max_idx] = window_m_list[i];
    window_m_list[i] = tmp;
  } 

  // find count
  int count = 0;
  for (; count < MAX_WINDOW_M_COUNT; count++) {
  	if (window_m_list[count] == NULL) break;
  }
  __window_count = count;
}


bool window_point_inside(window_m_t* win, int x, int y) {
	return x >= win->x && x <= win->x + win->width &&
				 y >= win->y && y <= win->y + win->height;
}


bool window_point_inside_bar(window_m_t* win, int x, int y) {
	return x >= win->x && x <= win->x + win->width &&
				 y >= win->y && y <= win->y + WINDOW_BAR_HEIGHT;
}

int window_find_xy(int x, int y) {
	if (__window_count == 0) {
		return -1;
	}
	for (int i=__window_count-1; i>=0; i--) {
		if (window_m_list[i] == NULL) { // is this IF needed? 
			break;
		}
		if (window_point_inside(window_m_list[i], x, y)) {
			return i;
		}
	}
	return -1;
}

window_m_t* window_m_find(uint32_t handle) {
	if (__window_count == 0) {
		return NULL;
	}
	// Biggest chance that searched handle belongs to topmost window
	for (int i=__window_count-1; i>=0; i--) {
		if (window_m_list[i]->handle == handle) {
            return window_m_list[i];
		}
	}
	return NULL;
}


void window_m_present_context(window_m_t* win  __UNUSED__, context_t* context) {
	assert(context->bpp == 32);
	memcpy(win->context.buffer, context->buffer, context->width * context->height * 4);
  window_m_need_redraw();
}

window_m_t* Window_m(int x, int y, int width, int height, uint32_t attributes) {
    // created from:  syscall_windows_create()

	window_m_t* new_win = (window_m_t*)malloc(sizeof(window_m_t));
	new_win->x = x;
	new_win->y = y;
	new_win->z = attributes & WINDOW_ATTR_BOTTOM ? 0 : find_max_z() + 1;
	new_win->width = width;
	new_win->height = height;
	new_win->attributes = attributes;
	new_win->handle = __window_handle++; // set new window added

	// Message queue
	new_win->message_queue_index = 0;
	new_win->parent_task = task_list_current;
	memset(new_win->message_queue, 0, sizeof(message_t)*MAX_MESSAGE_QUEUE_LENGTH);

	// Context setup
	new_win->context.width = width;
	new_win->context.height = height;
	new_win->context.bpp = 32;
	new_win->context.buffer = (uint32_t*)malloc(width * height * (32 / 8));

	// Add to list of windows : for now array of pointer to win
	assert(add_window(new_win));
	task_list_current->window = new_win;
	focused_window = new_win;

	window_sort_windows();
	window_m_need_redraw();
	return new_win;
}

void window_m_close(window_m_t *window) {
	int win_idx = find_window_index(window);
	free(window->context.buffer);
	window_m_list[win_idx] = NULL;
	free(window);
	window_sort_windows();
	if (window == focused_window) {
		focused_window = __window_count == 0 ? NULL : window_m_list[__window_count - 1];
	}
	window_m_need_redraw();
}

void window_draw(window_m_t *win, bool is_top __UNUSED__) {
	// Prsent window buffer 
	gfx_blit_v(&buffer_video_info, win->x, win->y, win->width, win->height, win->context.buffer);
}

void window_draw_all() {
	for (int i=0; i<MAX_WINDOW_M_COUNT; i++) {
		// List is sorted : first NULL means rest of them are NULL
		if (window_m_list[i] == NULL) {
			break;
		}
		bool is_top = i == __window_count - 1;
		window_draw(window_m_list[i], is_top);
	}
}

void present_video_buffer() {
	#if WAIT_FOR_VERTICAL_RETRACE
  	while ((inpb(0x3DA) & 0x08));
  	while (!(inpb(0x3DA) & 0x08));
 	#endif	
  memcpy((unsigned char*)screen_info.addr, (unsigned char*)buffer_video_info.addr, VIDEO_INFO_MEM_SIZE(buffer_video_info));
}

void window_bring_to_front(int win_idx) {
	if (__window_count <= 1) {
		return;
	}
	assert(win_idx < __window_count);
	if (win_idx == __window_count-1) {
		return;
	}

	window_m_t* old_top_win = window_m_list[__window_count - 1];
	window_m_t* new_top_win = window_m_list[win_idx];

	// Exchange Z
	int tmpz = new_top_win->z;
	new_top_win->z = old_top_win->z;
	old_top_win->z = tmpz;

	// Sort
	window_m_t* tmp = window_m_list[__window_count - 1];
	window_m_list[__window_count - 1] = window_m_list[win_idx];
	window_m_list[win_idx] = tmp;
	window_m_need_redraw();
}

// HOOLI SHAJT - simplify this poop
void window_m_handle_mouse() {
	if (mouse_buttons & MOUSE_LEFT_CLICK) {
		if (window_to_drag != NULL) {
			window_to_drag->x += mouse_x - __old_mouse_x;
			window_to_drag->y += mouse_y - __old_mouse_y;
			if (__old_mouse_x != mouse_x || __old_mouse_y != mouse_y) {
				message_t msg;
				msg.message = MESSAGE_WINDOW_DRAG;
				msg.x = window_to_drag->x;
				msg.y = window_to_drag->y;
				window_m_add_message_to_focused(&msg);
			}
			__old_mouse_x = mouse_x;
			__old_mouse_y = mouse_y;
		}	
		else {
			// Find win under the mouse and bring it to top
			int idx = window_find_xy(mouse_x, mouse_y);
			if (idx != -1) {
				window_m_t* win = window_m_list[idx];
				focused_window = win;
				if (!(focused_window->attributes & WINDOW_ATTR_BOTTOM)) {
				        window_bring_to_front(idx);
				}
				// Check for dragging
				if (window_point_inside_bar(win, mouse_x, mouse_y) && !(win->attributes & WINDOW_ATTR_NO_DRAG)) {
					window_to_drag = win;
					__old_mouse_x = mouse_x;
					__old_mouse_y = mouse_y;
				} else {
					window_to_drag = NULL; // DIRTY
				}
			} else {
				window_to_drag = NULL; // DIRTY
			}
		}
	} else {
		window_to_drag = NULL; // DIRTY
	}
	window_m_need_redraw();
}

// Bitmap_t* bmp_read_from_memory(void* bmp_file) {
// 	Bitmap_t* bmp = (Bitmap_t*)bmp_file;
// 	DEBUG("BMP[wallpaper]: type  = %i\n", bmp->firster.type);
// 	DEBUG("BMP[wallpaper]: size  = %i\n", bmp->firster.size);
// 	DEBUG("BMP[wallpaper]: reserved1 = %i\n", bmp->firster.reserved1);
// 	DEBUG("BMP[wallpaper]: reserved2 = %i\n", bmp->firster.reserved2);
// 	DEBUG("BMP[wallpaper]: offset = %i\n", bmp->firster.offset);
// 	DEBUG("BMP[wallpaper]: dib_header_size = %i\n", bmp->firster.dib_header_size);
// 	DEBUG("BMP[wallpaper]: width_px = %i\n", bmp->firster.width_px);
// 	DEBUG("BMP[wallpaper]: height_px = %i\n", bmp->firster.height_px);
// 	DEBUG("BMP[wallpaper]: num_planes = %i\n", bmp->firster.num_planes);
// 	DEBUG("BMP[wallpaper]: bits_per_pixel = %i\n", bmp->firster.bits_per_pixel);
// 	DEBUG("BMP[wallpaper]: compression = %i\n", bmp->firster.compression);
// 	DEBUG("BMP[wallpaper]: image_size_bytes = %i\n", bmp->firster.image_size_bytes);
// 	DEBUG("BMP[wallpaper]: x_resolution_ppm = %i\n", bmp->firster.x_resolution_ppm);
// 	DEBUG("BMP[wallpaper]: y_resolution_ppm = %i\n", bmp->firster.y_resolution_ppm);
// 	DEBUG("BMP[wallpaper]: num_colors = %i\n", bmp->firster.num_colors);
// 	DEBUG("BMP[wallpaper]: important_colors = %i\n", bmp->firster.important_colors);
// 	assert(bmp->firster.type == 0x4D42); // 'B' 'M'
// 	assert(bmp->firster.bits_per_pixel == 32);
// 	assert(bmp->firster.compression == 0);
// 	bmp->data = (uint32_t*)(((uint8_t*)bmp_file) + bmp->firster.offset);
// 	return bmp;
// }

//Bitmap_t wallpaper_bmp;

void window_m_need_redraw() {
	__window_need_redraw = true;
}

void window_m_manager_redraw() {
	if (!__window_need_redraw) {
		return;
	}
	window_sort_windows();
	// if (wallpaper_bmp.data != NULL) {
	// 	fast_memcpy((void*)buffer_video_info.addr, (unsigned char*)wallpaper_bmp.data, VIDEO_INFO_MEM_SIZE(buffer_video_info));
	// }
	// else {
	// 	memset((void*)buffer_video_info.addr, 0xC1, VIDEO_INFO_MEM_SIZE(buffer_video_info));
	// }
    window_m_handle_mouse();
    window_draw_all();
    window_draw_mouse();
    present_video_buffer();
    __window_need_redraw = false;
}

window_m_t* window_m_find_top() {
	if (__window_count == 0) {
		return NULL;
	}
	return window_m_list[__window_count-1];
}

void window_m_add_message(window_m_t *win, message_t *msg) {
	win->message_queue[win->message_queue_index++] = *msg;
	// if in WAIT state, awake 
	//win->parent_task->state = PROCESS_STATE_READY;
	if (win->message_queue_index >= MAX_MESSAGE_QUEUE_LENGTH) {
		win->message_queue_index = 0;
	}
}

void window_m_add_message_to_focused(message_t *msg) {
	if (focused_window == NULL) {
		return;
	}
	window_m_add_message(focused_window, msg);
}

bool window_m_pop_message(window_m_t* win, message_t* msg_out) {
	int peek = win->message_queue_index - 1;
	if (peek < 0) {
		peek = MAX_MESSAGE_QUEUE_LENGTH-1;
	}
	if (win->message_queue[peek].message != 0) {
		*msg_out = win->message_queue[peek];
		win->message_queue[peek].message = 0;
		win->message_queue_index = peek;
		return true;
	}
	else {
		msg_out->message = 0;
	}
	return false;
}

//////////////////////////////////////////window manager end ///////////////////////////////////

//////////////////////////////////////////window ///////////////////////////////////

static window_w_t *window_find_focused_component(window_w_t *win, message_t *msg);

void window_change_state(window_w_t* win, int state) {
	switch(state) {
		case WINDOW_STATE_CREATED:
			//win->state = state;
			window_send_message_simple(win, WINDOW_LIB_MESSAGE_CREATE);
			break;
	}
}

context_t *window_context_create(int width, int height, int bpp) {
	int context_size = width * height * (bpp / 8);
	context_t *context = malloc(sizeof(context_t));

	context->width = width;
	context->height = height;
	context->bpp = bpp;
	context->buffer = malloc(context_size);
	memset(context->buffer, 0xF4, context_size); // f4f4f4 - WINDOW_BACKGROUND_COLOR

	return context;
}

window_w_t *Window_w(int x, int y, int width, int height, char* title, int id, uint32_t attributes, uint32_t frame_flags) {
	window_w_t *window = calloc(sizeof(window_w_t), 1);

	// Init window structure
	window->type = WINDOW_TYPE_WINDOW;
	window->id = id;
	window->x = x;
	window->y = y;
	window->height = height;
	window->width = width;
	window->default_procedure = &window_w_default_procedure;
	window->title = strdup(title);

	// Create Ext params
	window_ext_t *ext = calloc(sizeof(window_ext_t), 1);
	ext->context = window_context_create(width, height, 32);
	ext->frame_flags = frame_flags;
	window->ext = ext;

	// Create window (systemcall)
	window_m_t* win = Window_m(x, y, width, height, attributes);
	window->handle = win->handle;
	//window->handle = syscall(SYSCall_windows_create, x, y, width, height, attributes);

	// Set state to be created
	window_change_state(window, WINDOW_STATE_CREATED);

	// Create Close and Min buttons
	if (!(frame_flags & WINDOW_FRAME_NONE)) {
		WINDOW_EXT(window)->button_close = button_create(window, 10, 13, 20, 20, "", WINDOW_LIB_BUTTON_CLOSE);
        //tyring to set img for close bnt
		Bitmap_t *bmp_close = malloc(sizeof(Bitmap_t));
		bmp_close = Bitmap("/Sys/Shell/Win/btnclose.bmp");
        if (bmp_close == NULL)
            DEBUG("stdio.c: null bmp file %s\n", bmp_close);
		button_set_image(WINDOW_EXT(window)->button_close, BUTTON_STATE_NORMAL, bmp_close);
  
		WINDOW_EXT(window)->button_min = button_create(window, 30, 13, 20, 20, "", WINDOW_LIB_BUTTON_MIN);
        //tyring to set img for min bnt
        Bitmap_t *bmp_min = malloc(sizeof(Bitmap_t));
		bmp_min = Bitmap("/Sys/Shell/Win/btnmin.bmp");
		button_set_image(WINDOW_EXT(window)->button_min, BUTTON_STATE_NORMAL, bmp_min);
	}

	return window;
}

void on_window_predraw(window_w_t *win) {
	int x1 = 0;
	int y1 = 0;
	int x2 = WINDOW_EXT(win)->context->width-1;
	int y2 = WINDOW_EXT(win)->context->height-1;
	// TODO syscall - syscall_windows_is_top(win->handle)
	bool is_top = false;

	if (WINDOW_EXT(win)->frame_flags & WINDOW_FRAME_NONE) return;
	// Draw frame border
	uint32_t top_frame_color = is_top ? WIN_ACTIVE_FRAME_COLOR : WIN_INACTIVE_FRAME_COLOR;
	gfx_draw_shadowed_box_c(WINDOW_EXT(win)->context, x1, y1, x2, y2, top_frame_color, WIN_BACKGROUND_COLOR);

	uint32_t top_bar_color = is_top ? WIN_ACTIVE_BAR_COLOR : WIN_INACTIVE_BAR_COLOR;
	gfx_fillrect_c(WINDOW_EXT(win)->context, x1 + 2, y1 + 2, x2 - 4, y1 + WINDOW_BAR_HEIGHT - 2, top_bar_color);
	// Line under bar
	gfx_hline_c(WINDOW_EXT(win)->context, x1 + 2, x2 - 4, y1 + WINDOW_BAR_HEIGHT - 2, top_frame_color);
	gfx_hline_c(WINDOW_EXT(win)->context, x1 + 2, x2 - 4, y1 + WINDOW_BAR_HEIGHT - 3, top_frame_color);

	// Frame label
	int text_x = (win->width - TEXT_FONT_WIDTH(win->title)) / 2;
	int text_y = (WINDOW_BAR_HEIGHT - TEXT_FONT_HEIGHT(win->title)) / 2;
	gfx_puts_c(WINDOW_EXT(win)->context, x1 + text_x, y1 + text_y, WIN_BAR_TEXT_COLOR, top_bar_color, win->title);
}

void window_w_add_child(window_w_t *parent, window_w_t *child) {
	window_w_t *w = parent;
	while (w->next != NULL) { w = w->next; }
	w->next = child;
}

bool window_w_point_inside(window_w_t* win, int x, int y) {
	return x >= win->x && x <= win->x + win->width &&
				 y >= win->y && y <= win->y + win->height;
}

bool window_w_dispatch_message(window_w_t *win, struct message_struct *msg) {
	bool consumed = false;
	if (win->window_procedure != NULL) {
		consumed = win->window_procedure(win, msg);
	}
	if (!consumed) {
		consumed = win->default_procedure(win, msg);
	}
	return consumed;
}

bool window_w_dispatch_message_simple(window_w_t *win, int message) {
	message_t msg;
	msg.message = message;
	return window_w_dispatch_message(win, &msg);
}

void window_w_dispatch(window_w_t *win, struct message_struct *msg) {
	for (window_w_t *w = win; w != NULL; w = w->next) {
		// If message is consumed, stop the chain
		if (window_w_dispatch_message(w, msg)) {
			break;
		}
	}
}

// this handels or cheacks users input
bool window_w_default_procedure(window_w_t *win, message_t *msg) {
	switch(msg->message) {
		case MESSAGE_MOUSE_PRESS:
		case MESSAGE_MOUSE_RELEASE:
			WINDOW_EXT(win)->focused = window_find_focused_component(win, msg);
			return false;
		case MESSAGE_WINDOW_DRAG:
			win->x = msg->x;
			win->y = msg->y;
			return true;
		case WINDOW_LIB_MESSAGE_CREATE:
			window_send_message_simple(win, WINDOW_LIB_MESSAGE_PREDRAW);
			return false;
		case WINDOW_LIB_MESSAGE_PREDRAW:
			on_window_predraw(win);
			window_send_message_simple(win, WINDOW_LIB_MESSAGE_DRAW);
			return false;
		case WINDOW_LIB_MESSAGE_DRAW:
			window_send_message_simple(win, WINDOW_LIB_MESSAGE_PRESENT);
			return false;
		case WINDOW_LIB_MESSAGE_PRESENT:
			window_w_present(win);
			return true;
		case WINDOW_LIB_BUTTON_CLOSE:
			DEBUG("CLOSE BUTTON PRESSED");
			window_w_close(win, 0);
			return true;
		case WINDOW_LIB_BUTTON_MIN:
			DEBUG("MIN BUTTON PRESSED");
			return true;
	}
	return false;
}

vector_t mouse_to_local(window_w_t *win, message_t *msg) {
	return (vector_t){msg->x - win->x, msg->y - win->y};
}

// found last one
static window_w_t *window_find_focused_component(window_w_t *win, message_t *msg) {
	vector_t local = mouse_to_local(win, msg);
	window_w_t *found = NULL;
	for(window_w_t *w = win->next; w != NULL; w = w->next) {
		if (window_w_point_inside(w, local.x, local.y)) {
			found = w;
		}
	}
	return found;
}

void window_w_invalidate(window_w_t *win) {
	window_send_message_simple(win, WINDOW_LIB_MESSAGE_PREDRAW);
}

void window_w_present(window_w_t *win) {
	//syscall(SYSCall_windows_present, win->handle, WINDOW_EXT(win)->context);
	window_m_t* win0 = window_m_find(win->handle);
	if (win0 != NULL) {
		context_t* context = (context_t*)WINDOW_EXT(win)->context;
		window_m_present_context(win0, context);
	}
}

void window_w_close(window_w_t *window __UNUSED__, int exit_code) {
	syscall(SYSCall_process_exit, exit_code);
}

//////////////////////////////////////////window end ///////////////////////////////////

