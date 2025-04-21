

#include <Libs/Stdbool/Stdbool.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Gui/messags/messaging.h>
//#include <Addis/Process.h>
#include <Drivers/mouse/mouse.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>

/////////////////////////////////commen/////////////////////////////////////

#define WINDOW_ATTR_TRANSP 0x01
#define WINDOW_ATTR_BOTTOM 0x02
#define WINDOW_ATTR_NO_DRAG 0x04


typedef struct message_struct {
	uint16_t 		message;
	uint16_t 		handle;
	int16_t 		x;
	int16_t 		y;
	int32_t 		key;
} message_t;

typedef struct {
	uint32_t* buffer;
	int width;
	int height;
	int bpp;
} context_t;


////////////////////////////////commen end//////////////////////////////////

/////////////////////////////////windows manager ///////////////////////

#define MAX_WINDOW_NAME_LENGTH 128
#define MAX_MESSAGE_QUEUE_LENGTH 128
typedef struct window_m_struct {
	uint32_t handle;
	int x;
	int y;
	int z;
	int width;
	int height;
	uint32_t attributes;
	int message_queue_index;
	task_t* parent_task;
	message_t message_queue[MAX_MESSAGE_QUEUE_LENGTH];
	context_t context;
} window_m_t;
/*
typedef struct {
    uint16_t type;              // Magic identifier: 0x4d42
    uint32_t size;              // File size in bytes
    uint16_t reserved1;         // Not used
    uint16_t reserved2;         // Not used
    uint32_t offset;            // Offset to image data in bytes from beginning of file
    uint32_t dib_header_size;   // DIB Header size in bytes
    int32_t  width_px;          // Width of the image
    int32_t  height_px;         // Height of image
    uint16_t num_planes;        // Number of color planes
    uint16_t bits_per_pixel;    // Bits per pixel
    uint32_t compression;       // Compression type
    uint32_t image_size_bytes;  // Image size in bytes
    int32_t  x_resolution_ppm;  // Pixels per meter
    int32_t  y_resolution_ppm;  // Pixels per meter
    uint32_t num_colors;        // Number of colors
    uint32_t important_colors;  // Important colors
} __attribute__((packed)) bmp_header_t;

typedef struct {
    uint8_t *buffer;
    bmp_header_t *header;
    uint32_t *data;
} Bitmap_t;
*////////////created in bmp.h

#define MAX_WINDOW_M_COUNT 64

extern window_m_t* window_m_list[MAX_WINDOW_M_COUNT];

void  window_m_present_context(window_m_t* win, context_t* context);

void window_m_add_message(window_m_t *win, message_t *msg);
void window_m_add_message_to_focused(message_t *msg);
bool window_m_pop_message(window_m_t* win, message_t* msg_out);

window_m_t* window_m_find(uint32_t handle);
void init_kernel_window_m_manager();
void window_m_manager_redraw();

void window_m_handle_mouse();
void window_m_need_redraw();

window_m_t* Window_m(int x, int y, int width, int height, uint32_t attributes);
void window_m_close(window_m_t *window);

/////////////////////////////////windows manager end///////////////////////////////////////

/////////////////////////////////windows /////////////////////////////////////////////
#define WINDOW_TYPE_WINDOW 0

#define WINDOW_STATE_CREATED 0
#define WINDOW_STATE_DESTORY 1

#define WINDOW_LIB_MESSAGE_CREATE 	1000
#define WINDOW_LIB_MESSAGE_PREDRAW	1001
#define WINDOW_LIB_MESSAGE_DRAW   	1002
#define WINDOW_LIB_MESSAGE_PRESENT 	1003
#define WINDOW_LIB_BUTTON_CLOSE   	1004
#define WINDOW_LIB_BUTTON_MIN     	1005

#define WINDOW_USER_MESSAGE         10000

#define WINDOW_MAX_COMPONENTS 32

#define WIN_BACKGROUND_COLOR 			0xFFFFFF
#define WIN_ACTIVE_BAR_COLOR   		0xD5DFDF
#define WIN_INACTIVE_BAR_COLOR		0xD5DFDF
#define WIN_INACTIVE_FRAME_COLOR	0x000000
#define WIN_ACTIVE_FRAME_COLOR 		0x000000
#define WIN_BAR_TEXT_COLOR  			0x000000

#define WINDOW_FRAME_WIDTH 2
#define WINDOW_BAR_HEIGHT  44

#define WINDOW_ATTR_NONE 0x00

#define WINDOW_FRAME_DEFAULT 0x00
#define WINDOW_FRAME_TRANSPARENT 0x01
#define WINDOW_FRAME_NONE 0x02

#define TEXT_FONT_WIDTH(str) (strlen(str)*8)
#define TEXT_FONT_HEIGHT(str) (8)

typedef uint32_t window_handle;

struct window_struct;
struct message_struct;
typedef bool (*window_procedure_t)(struct window_struct *, struct message_struct *);

typedef struct {
	int x;
	int y;
} vector_t;
#define WINDOW_EXT(win) ((window_ext_t*)win->ext)

typedef struct {
	context_t *context;
	struct window_struct *focused;
	struct window_struct *button_close;
	struct window_struct *button_min;
	uint32_t frame_flags;
} window_ext_t;

struct window_struct {
	// Basic params
	window_handle handle;
	int id;
	int type;
	int x, y;
	int width, height;
	int state;
	char* title;

	// Win proc
	window_procedure_t window_procedure;
	window_procedure_t default_procedure;

	// Extended parameters (based on type ... check TYPE_EXT macro e.g. WINDOW_EXT)
	void *ext;

	// Linked list (in fact it should be general tree to have proper Z order)
	struct window_struct *next;
	struct window_struct *parent;
};

typedef struct window_struct window_w_t;

void window_send_message(window_w_t* win, message_t* msg);
void window_send_message_simple(window_w_t* win, int message);
bool window_get_message(window_w_t* win, message_t* msg);
bool window_peek_message(window_w_t* win, message_t* msg);

window_w_t *Window_w(int x, int y, int width, int height, char* title, int id, uint32_t attributes, uint32_t frame_flags);

void window_w_close(window_w_t *window, int exit_code);
bool window_w_default_procedure(window_w_t *win, struct message_struct *msg);
void window_w_add_child(window_w_t *parent, window_w_t *child);

void window_w_dispatch(window_w_t *win, struct message_struct *msg);
bool window_w_dispatch_message(window_w_t *win, struct message_struct *msg);
bool window_w_dispatch_message_simple(window_w_t *win, int message);
void window_w_present(window_w_t *win);

void window_w_invalidate(window_w_t *win);
void window_w_change_state(window_w_t* win, int state);
void on_window_w_predraw(window_w_t *win);

bool window_w_point_inside(window_w_t* win, int x, int y);

////////////////////////////windows end ////////////////////////////////
