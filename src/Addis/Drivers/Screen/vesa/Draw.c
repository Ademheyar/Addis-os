#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Libs/Bits/Font_8x8.h>

// this will put color spacfic pixel
void put_pixel(Screen_info_t* screen_info, int x, int y, uint32_t color) {
	CLIP_XY_v(x, y);
	FRAMEBUFFER_32[FIRST_PIXEL_v(x, y)] = color;
}

// this will Draw a Character
void Draw_Character2(Screen_info_t* screen_info, int x, int y, int w, int h, int pixel_w, int pixel_h, int pw, int ph, uint32_t fgcolor, uint32_t bgcolor, const char cha) {
  int a, b, c, d;
	int q = 1, qq = 1;
	for(a = 0, c = 0; a <= w; a++) {
    for(b = 0, d = 0; b <= h; b++) {
			if ((x+a >= x && x+a < pw-1 && y+b >= y && y+b < ph-1)) {
				if(cha && (a > 0 && b > 0)  && (a <= pixel_w && b <= pixel_h)){
					FRAMEBUFFER_32[FIRST_PIXEL_v(x+a, y+b)] = ((font8x8_basic[cha & 0x7F][d] >> c ) & 1) ? fgcolor : bgcolor;
					if (q == pixel_w/8) { d++; q = 1; }
					else q++;
				}
				else if (cha && (a > 0 && b > 0)  && a > pixel_w+1) return;
				else FRAMEBUFFER_32[FIRST_PIXEL_v(x+a, y+b)] = bgcolor;
			}
			else return;
    }	
		if (qq == pixel_h/8) { c++; qq = 1; }
		else qq++;
	}
}

// this will Draw a text 
void Draw_String(Screen_info_t* screen_info, int x, int y, int w, int h, int pixel_w, int pixel_h, int pw, int ph, uint32_t fgcolor, uint32_t bgcolor, const char *c) {
	int y0 = y, x0 = x;
	if(pw == 0) pw = screen_info->width;
	if(ph == 0) ph = screen_info->height;
	for(int i = 0; *(c+i) || i < w; i++) {
		if(*(c+i)){
			if(*(c+i) == '\n'){
				x0 = x;
				y0 += pixel_h+1;
				continue;
			}
			else if (y0 >= screen_info->height) {
				continue;
			}
			else{
				if(x0 >= screen_info->width) {
					x0 = x;
					y0 += pixel_h+1;
				}
				Draw_Character2(screen_info, x0, y0, w, h, pixel_w, pixel_h, pw, ph, fgcolor, bgcolor, *(c+i));
			}
			x0 +=pixel_w;
		}
		else {
			Draw_Character2(screen_info, x0, y0, w, h, pixel_w, pixel_h, pw, ph, fgcolor, bgcolor, 0);
			break;
		}

	}
}

// this will Draw horizontal line
void Draw_hline(Screen_info_t* screen_info, int x1, int x2, int y, uint32_t color) {
	CLIP_X_v(x1);
	CLIP_X_v(x2);
	CLIP_Y_v(y);
	for (int x=x1; x<=x2; x++) {
		FRAMEBUFFER_32[FIRST_PIXEL_v(x, y)] = color;
	}
}

// this will Draw vertical line
void Draw_vline(Screen_info_t* screen_info, int x, int y1, int y2, uint32_t color) {
	CLIP_X_v(x);
	CLIP_Y_v(y1);
	CLIP_Y_v(y2);
	for (int y=y1; y<=y2; y++) {
		FRAMEBUFFER_32[FIRST_PIXEL_v(x, y)] = color;
	}
}

// drawing round line like paine
void Draw_round(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, int c) {
	int i = 1;
	while(!(x1 == x2 && y1 == y2)){
		if (i == 1){
			if (x1 > x2) x1--;
			else if (x1 < x2) x1++;
			i=0;
		}
		else i++;

		if (y1 > y2) y1--;
		else if (y1 < y2) y1++;

  	put_pixel(screen_info,x1,y1,c);
	}
}

// drawing line like paine
void Draw_line(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, int c) {
	while(!(x1 == x2 && y1 == y2)){
		if (x1 > x2) x1--;
		else if (x1 < x2) x1++;

		if (y1 > y2) y1--;
		else if (y1 < y2) y1++;
  	put_pixel(screen_info,x1,y1,c);
	}
}

// this will Draw a rectangle line
void Draw_rect(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color) {
	Draw_line(screen_info, x1, y1, x1, y2, color); // to draw horizontal line
	Draw_line(screen_info, x2, y1, x2, y2, color); // to draw horizontal line
	Draw_line(screen_info, x1, y1, x2, y1, color); // to draw vertical line
	Draw_line(screen_info, x1, y2, x2, y2, color); // to draw vertical line
}

// this will Draw felat rectangle filled with color
void Draw_filled_rect(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color) {
	for (int i=y1; i<=y2; i++) {
		Draw_hline(screen_info, x1, x2, i, color);
	}
}




// this will Draw a rectangle line considering WINDOW_FRAME_WIDTH
void gfx_rect_width_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color, int width) {
	for (int i=0; i<width; i++) {
		Draw_line(screen_info, x1, y1, x1, y2+i, color); // to draw horizontal line
		Draw_line(screen_info, x2, y1, x2, y2-i, color); // to draw horizontal line
		Draw_line(screen_info, x1, y1+i, x2, y1, color); // to draw vertical line
		Draw_line(screen_info, x1, y2-i, x2, y2, color); // to draw vertical line
	
		//Draw_hline(screen_info, x1, x2, y1, color);
		//Draw_hline(screen_info, x1, x2, y2, color);

		//Draw_vline(screen_info, x1, y1, y2, color);
		//Draw_vline(screen_info, x2, y1, y2, color);

		//Draw_hline(screen_info, x1, x2, y1, color);
		//Draw_hline(screen_info, x1, x2, y2, color);
		//Draw_vline(screen_info, x1, y1, y2, color);
		//Draw_vline(screen_info, x2, y1, y2, color);
	}
}

// this will Draw a box that has shadow
void gfx_draw_shadowed_box_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color, uint32_t bg_color) {
	gfx_rect_width_v(screen_info, x1, y1, x2-2, y2-2, color, 2);
	gfx_rect_width_v(screen_info, x1+2, y1+2, x2, y2, color, 2);
	Draw_filled_rect(screen_info, x1+2, y1+2, x2-4, y2-4, bg_color);
}


// this will Draw dot line
void gfx_fillrect_dot_v(Screen_info_t* screen_info, int x1, int y1, int x2, int y2, uint32_t color) {
	for (; x1 <= x2; x1++) {
		for (; y1 <= y2; y1++) {
			if ((x1 % 2 == 0) || (y1 % 2 == 0)) {
				FRAMEBUFFER_32[FIRST_PIXEL_v(x1, y1)] = color;
			}
		}
	}
}

// this two  fanction will create or draw group of color/pic 
// Optimize + CLIP
void gfx_blit_v(Screen_info_t* screen_info, int x, int y, int width, int height, uint32_t* src) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			FRAMEBUFFER_32[FIRST_PIXEL_v(x+j, y+i)] = *(src + (j + i * width));
		}
	}
}

// this will draw other than trans_color
// TODO: this is silly : use transparent map or alpha channel
void gfx_blit_transparent_v(Screen_info_t* screen_info, int x, int y, int width, int height, uint32_t* src, uint32_t trans_color) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			uint32_t c = *(src + (j + i * width));
			if (c != trans_color) {
				FRAMEBUFFER_32[FIRST_PIXEL_v(x+j, y+i)] = *(src + (j + i * width));
			}
		}
	}
}














/*
void gfx_putchar_c(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c) {
  uint8_t i, j;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
    	if (x+i >= 0 && x+1 < context->width &&
    		  y+j >= 0 && y+1 < context->height) {
  			CONTEXT_32[FIRST_PIXEL_c(x+i, y+j)] = ((font8x8_basic[c & 0x7F][j] >> i ) & 1) ? fgcolor : bgcolor;
  		}
    }
  }
}

void gfx_putchar_trans_c(context_t* context, int x, int y, uint32_t color, const char c) {
  uint8_t i, j;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
    	if (x+i >= 0 && x+1 < context->width &&
    		  y+j >= 0 && y+1 < context->height) {
    		bool draw = ((font8x8_basic[c & 0x7F][j] >> i ) & 1);
    		if (draw) {
  				CONTEXT_32[FIRST_PIXEL_c(x+i, y+j)] = color;
  			}
  		}
    }
  }
}

void gfx_puts_c(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char *c) {
	while(*c) {
		gfx_putchar_c(context, x, y, fgcolor, bgcolor, *c++);
		x += 8;
	}
}

void gfx_puts_trans_c(context_t* context, int x, int y, uint32_t color, const char *c) {
	while(*c) {
		gfx_putchar_trans_c(context, x, y, color, *c++);
		x += 8;
	}
}










/////////////////////////////////////gfx app////////////////////
void gfx_hline_c(context_t* context, int x1, int x2, int y, uint32_t color) {
	CLIP_X_c(x1);
	CLIP_X_c(x2);
	CLIP_Y_c(y);
	for (int x=x1; x<=x2; x++) {
		CONTEXT_32[FIRST_PIXEL_c(x, y)] = color;
	}
}

void gfx_vline_c(context_t* context, int x, int y1, int y2, uint32_t color) {
	CLIP_X_c(x);
	CLIP_Y_c(y1);
	CLIP_Y_c(y2);
	for (int y=y1; y<=y2; y++) {
		CONTEXT_32[FIRST_PIXEL_c(x, y)] = color;
	}
}

void gfx_rect_c(context_t* context, int x1, int y1, int x2, int y2, uint32_t color) {
	gfx_hline_c(context, x1, x2, y1, color);
	gfx_hline_c(context, x1, x2, y2, color);
	gfx_vline_c(context, x1, y1, y2, color);
	gfx_vline_c(context, x2, y1, y2, color);
}

void gfx_fillrect_c(context_t* context, int x1, int y1, int x2, int y2, uint32_t color) {
	for (int i=y1; i<=y2; i++) {
		gfx_hline_c(context, x1, x2, i, color);
	}
}

// Optimize + CLIP
void gfx_blit_c(context_t* context, int x, int y, int width, int height, uint32_t* src) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			CONTEXT_32[FIRST_PIXEL_c(x+j, y+i)] = *(src + (j + i * width));
		}
	}
}

void gfx_rect_width_c(context_t *context, int x1, int y1, int x2, int y2, uint32_t color, int width) {
	for (int i=0; i<width; i++) {
		gfx_hline_c(context, x1, x2, y1+i, color);
		gfx_hline_c(context, x1, x2, y2-i, color);
		gfx_vline_c(context, x1+i, y1, y2, color);
		gfx_vline_c(context, x2-i, y1, y2, color);
	}
}

void gfx_draw_shadowed_box_c(context_t *context, int x1, int y1, int x2, int y2, uint32_t color, uint32_t bg_color) {
	gfx_rect_width_c(context, x1, y1, x2-2, y2-2, color, WINDOW_FRAME_WIDTH);
	gfx_rect_width_c(context, x1+2, y1+2, x2, y2, color, WINDOW_FRAME_WIDTH);
	gfx_fillrect_c(context, x1+2, y1+2, x2-4, y2-4, bg_color);
}
*/