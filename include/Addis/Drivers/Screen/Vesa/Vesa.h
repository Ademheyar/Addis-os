#ifndef __VESA_H
#define __VESA_H

#include <Addis/Drivers/Screen/Vga/Vga.h>

pdpe_t  pdpe_video[512] __attribute__((aligned(4096)));
pde_t   pde_video[512] __attribute__((aligned(4096)));

#define COLOR_WHITE 0x00ffffff
#define COLOR_BLACK 0x00000000
#define COLOR_RED   0x00ff0000
#define COLOR_GREEN 0x0000ff00
#define COLOR_BLUE  0x000000ff
#define COLOR_MAGENTA  0x00ff00ff

#define VESA_FRAMEBUFFER_8 ((uint8_t*)KERNEL_VIDEO_MEMORY)
#define VESA_FRAMEBUFFER_32 ((uint32_t*)KERNEL_VIDEO_MEMORY)
#define VESA_FRAMEBUFFER_64 ((uint64_t*)KERNEL_VIDEO_MEMORY)

#define VIDEO_INFO_MEM_SIZE(vi) (vi.width * vi.height * (vi.bits/8))


typedef struct Screen_info_struct {
  uint64_t linear_addr;
  uint64_t addr;
  int width;
  int height;
	int x;
  int y;
	// color fg bg
  int bits;
  int pitch;
  uint8_t  type;
} Screen_info_t;

Screen_info_t screen_info;

//void init_kernel_vesa(multiboot_info_t *mbi);
void Set_VESA_DRIVER();
struct ROW *Get_VESA_DRIVER(list_t *read, int isret);
struct ROW *VESA_DRIVER(list_t *read, int isret);

#define FRAMEBUFFER_32 ((uint32_t*)screen_info->addr)
#define FIRST_PIXEL_v(x, y) ((x) + ((y) * (screen_info->width)))


#define CONTEXT_32 ((uint32_t*)context->buffer)
#define FIRST_PIXEL_c(x, y) ((x) + ((y) * (context->width)))

// Be carefull with this macro :)
#define CLIP_X_v(x) x = x < 0 ? 0 : x; x = x < screen_info->width ? x : screen_info->width-1
#define CLIP_Y_v(y) y = y < 0 ? 0 : y; y = y < screen_info->height ? y : screen_info->height-1
#define CLIP_XY_v(x, y) CLIP_X_v(x); CLIP_Y_v(y)

#define CLIP_X_c(x) x = x < 0 ? 0 : x; x = x < context->width ? x : context->width-1
#define CLIP_Y_c(y) y = y < 0 ? 0 : y; y = y < context->height ? y : context->height-1
#define CLIP_XY_c(x, y) CLIP_X_c(x); CLIP_Y_c(y)

int get_takeinfo(struct USER_WDB *var, char *text, char *name);

void put_pixel(Screen_info_t* screen_info, int x, int y, uint32_t color);
//void Draw_Character(Screen_info_t* screen_info, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c);
//void Draw_String(Screen_info_t* screen_info, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char *c);

void Draw_Character(Screen_info_t* screen_info, int x, int y, int w, int h, int pixel_w, int pixel_h, int pw, int ph, uint32_t fgcolor, uint32_t bgcolor, const char c);
void Draw_String(Screen_info_t* screen_info, int x, int y, int w, int h, int pixel_w, int pixel_h, int pw, int ph, uint32_t fgcolor, uint32_t bgcolor, const char *c);

void Draw_line(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, int c);
void Draw_hline(Screen_info_t* screen_info, int x1, int x2, int y, uint32_t color);
void Draw_vline(Screen_info_t* screen_info, int x, int y1, int y2, uint32_t color);
void Draw_rect(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color);
void Draw_filled_rect(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color);
void Draw_round(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, int c);


void gfx_rect_width_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color, int width);
void gfx_draw_shadowed_box_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color, uint32_t bg_color);

void gfx_fillrect_dot_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color);


void gfx_blit_v(Screen_info_t* screen_info, int x, int y, int width, int height, uint32_t* src);
void gfx_blit_transparent_v(Screen_info_t* screen_info, int x, int y, int width, int height, uint32_t* src, uint32_t trans_color);


// 


//void gfx_putchar_c(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c);
//
//void gfx_hline_c(context_t* context, int x1, int x2, int y, uint32_t color);
//void gfx_vline_c(context_t* context, int x, int y1, int y2, uint32_t color);
//
//void gfx_rect_c(context_t* context, int x1, int y1, int x2, int y2, uint32_t color);
//void gfx_fillrect_c(context_t* context, int x1, int y1, int x2, int y2, uint32_t color);
//void gfx_rect_width_c(context_t *context, int x1, int y1, int x2, int y2, uint32_t color, int width);
//void gfx_draw_shadowed_box_c(context_t *context, int x1, int y1, int x2, int y2, uint32_t color, uint32_t bg_color);
//
//void gfx_putchar_trans_c(context_t* context, int x, int y, uint32_t color, const char c);
//void gfx_puts_c(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char *c);
//void gfx_puts_trans_c(context_t* context, int x, int y, uint32_t color, const char *c);

//void gfx_blit_c(context_t* context, int x, int y, int width, int height, uint32_t* src);

#endif
