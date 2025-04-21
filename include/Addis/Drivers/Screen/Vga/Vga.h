#ifndef SCREEN_H
#define SCREEN_H

#include <Drivers/Serial/Serial.h>

/*
 * Some constans defined for vga 80 * 25 mode
 * */

// Some colors
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHT_GREY      7
#define DARK_GREY       8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_RED       12
#define LIGHT_MAGENTA   13
#define LIGHT_BROWN     14
#define WHITE           15


// VGA 80 * 25 mode screen
#define SCREEN ((uint16_t*)(LOAD_MEMORY_ADDRESS + 0xB8000))
#define WIDTH 80
#define HEIGHT 25
#define DEFAULT_COLOR 0x0F

// Make a pixel(2 bytes), background is always black, a is a char, b is foreground color
#define PAINT(a,b) (((b & 0xF) << 8) | (a & 0xFF))

// Get pixel
#define PIXEL(x, y) SCREEN[y * 80 + x]

enum {
  PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GRAY = 7,
	PRINT_COLOR_DARK_GRAY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_PINK = 13,
	PRINT_COLOR_YELLOW = 14,
	PRINT_COLOR_WHITE = 15,
};

void print_clear();
void print_char(char character);
void print_str(char* string);
void print_set_color(uint8_t foreground, uint8_t background);

void video_init();

void print_string(char * s);

void print_char(char c);

void scroll();

void update_cursor();

void set_curr_color(uint8_t color);

uint8_t get_curr_color();

void clear();

int cursorX , cursorY;
const uint8 sw ,sh ,sd ; 
                                                    //We define the screen width, height, and depth.
void clearLine(uint8 from,uint8 to);

void updateCursor();

void clearScreen();

void scrollUp(uint8 lineNumber);

void newLineCheck();

void printch(char c);

void print (string ch);
void set_screen_color_from_color_code(int color_code);
void set_screen_color(int text_color,int bg_color);
void print_colored(string ch,int text_color,int bg_color);


#endif
