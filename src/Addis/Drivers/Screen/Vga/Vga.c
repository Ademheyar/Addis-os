#include <Addis/Drivers/Screen/Vga/Vga.h>

int cursorX = 0, cursorY = 0;
const uint8 sw = 80,sh = 25,sd = 2; 
struct Char* buffer = (struct Char*)0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clearLine(uint8 from,uint8 to)
{
	uint16 i = sw * from * sd;
	string vidmem=(string)0xb8000;
	for(;i<(sw*to*sd);i++)
	{
		vidmem[(i / 2)*2 + 1 ] = color ;
		vidmem[(i / 2)*2 ] = 0;
	}
}

void updateCursor()
{
  unsigned temp;
  temp = cursorY * sw + cursorX-1;  // Position = (y * width) +  x
	outportb(0x3D4, 14);         // CRT Control Register: Select Cursor Location
	outportb(0x3D5, temp >> 8);  // Send the high byte across the bus
	outportb(0x3D4, 15);         // CRT Control Register: Select Send Low byte
	outportb(0x3D5, temp);       // Send the Low byte of the cursor location
}

void clearScreen()
{
        clearLine(0,sh-1);
        cursorX = 0;
        cursorY = 0;
        updateCursor();
}

void scrollUp(uint8 lineNumber)
{
        string vidmem = (string)0xb8000;
        uint16 i = 0;
        clearLine(0,lineNumber-1);                                            //updated
        for (;i<sw*(sh-1)*2;i++)
        {
                vidmem[i] = vidmem[i+sw*2*lineNumber];
        }
        clearLine(sh-1-lineNumber,sh-1);
        if((cursorY - lineNumber) < 0 ) 
        {
                cursorY = 0;
                cursorX = 0;
        } 
        else 
        {
                cursorY -= lineNumber;
        }
        updateCursor();
}


void newLineCheck()
{
        if(cursorY >=sh-1)
        {
                scrollUp(1);
        }
}

void printch(char c)
{
    string vidmem = (string) 0xb8000;     
    switch(c)
    {
        case (0x08):
                if(cursorX > 0) 
                {
	                cursorX--;									
                        vidmem[(cursorY * sw + cursorX)*sd]=0;	     //(0xF0 & color)                          
	        }
	        break;
       /* case (0x09):
                cursorX = (cursorX + 8) & ~(8 - 1); 
                break;*/
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        default:
                vidmem [((cursorY * sw + cursorX))*sd] = c;
                vidmem [((cursorY * sw + cursorX))*sd+1] = color;
                cursorX++; 
                break;
	
    }
    if(cursorX >= sw)                                                                   
    {
        cursorX = 0;                                                                
        cursorY++;                                                                    
    }
    updateCursor();
    newLineCheck();
}

void print (string ch)
{
        uint16 i = 0;
        uint8 length = strlength(ch);              //Updated (Now we store string length on a variable to call the function only once)
        for(;i<length;i++)
        {
                printch(ch[i]);
        }
       /* while((ch[i] != (char)0) && (i<=length))
                print(ch[i++]);*/
        
}
void set_screen_color(int text_color,int bg_color)
{
	color =  (bg_color << 4) | text_color;;
}
void set_screen_color_from_color_code(int color_code)
{
	color = color_code;
}
void print_colored(string ch,int text_color,int bg_color)
{
	int current_color = color;
	set_screen_color(text_color,bg_color);
	print(ch);
	set_screen_color_from_color_code(current_color);
}


#include <Addis/Drivers/Screen/Vga/Vga.h>
#include <System.h>
#include <Addis/Libs/String/String.h>
#include <x86.h>

int curr_x = 0, curr_y = 0;

uint8_t curr_color = DEFAULT_COLOR;

struct Char {
    uint8_t character;
    uint8_t color;
};

void clear_Line(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < WIDTH; col++) {
      buffer[col + WIDTH * row] = empty;
    }
}

void clear_GLine(size_t from, size_t to) {
	for (size_t i = from; i < to; i++) {
		clear_Line(i);
	}
}

/*
 * Update cursor
 * */
void update_cursor(int x, int y) {
  unsigned curr_pos = y * WIDTH + x; // position = (y * width) + x
	curr_x = x; curr_y = y;
	outp(0x3D4, 14); // CRT control Register: select Cursor location
	outp(0x3D5, curr_pos >> 8); // Send the high byte across the bus
	outp(0x3D4, 15); // CRT control Register: select send low byte 
	outp(0x3D5, curr_pos); // send the low byte of ther courser location
}

/*
 * Clear the screen, simply put space char on video memory
 * */
void clear_screen() {
	clear_GLine(0, HEIGHT-1);
	curr_x = 0;
	curr_y = 0;
	update_cursor(curr_x, curr_y);
}


/*
 * Scroll down by one line, copy the line 1 to line 0, line 2 to line 1...... and delete the last line
 * */
void scroll_up(int num) {
  uint16_t i = 0;
	for(; i<WIDTH*(HEIGHT-1)*2; i++){
		buffer[i] = buffer[i + WIDTH * 2 * num];
	}
	clear_GLine(HEIGHT-1-num, HEIGHT-1);
	if((curr_y - num) < 0){
		curr_y = 0; curr_x = 0;
	}
	else curr_y -= num;
	update_cursor(curr_x, curr_y);
}

void newlineCheck(){
	if(curr_y >= HEIGHT-1){
		scroll_up(1);
	}
}

/* Print a char to screen, under current x,y position
 * scroll if necessary
 * For now, we only care about normal character, tab, and newline character
 * */
void print_char(char character) {
	switch(character){
		case (0x00):
			if( curr_x > 0){
				curr_x--;
				buffer[(curr_y * WIDTH + curr_x)] = (struct Char) {
				character: ' ',
				color: color,
				};			
			}
		break;
		case (0x09):
			curr_x = (curr_x + 8) & -(8 - 1);
		break;
		case ('\r'):
			curr_x = 0;
		break;
		case ('\n'):
			curr_x = 0;
			curr_y++;
		break;
		default:
			buffer[(curr_y * WIDTH + curr_x)] = (struct Char) {
			character: character,
			color: color,
			};		
			curr_x++;
		break;
	}
	if(curr_x >= WIDTH){
		curr_x = 0;
		curr_y++;
	}
	newlineCheck();
 	update_cursor(curr_x, curr_y);
}
void print_newline() {
    col = 0;
    if (row < HEIGHT - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < HEIGHT; row++) {
			for (size_t col = 0; col < WIDTH; col++) {
				struct Char character = buffer[col + WIDTH * row];
				buffer[col + WIDTH * (row - 1)] = character;
			}
    }
    clear_Line(WIDTH - 1);
}

void print_string(char * s) {
	while(*s != '\0') {
		print_char(*s);
		s++;
	}
}

void print_set_color(uint8_t foreground, uint8_t background) {
	//color = foreground + (background << 4);
  color = foreground | background << 4;
}

/*
 * Set current foreground color
 * */
void set_curr_color(uint8_t color) {
   curr_color = color & 0x0F;
}

/*
 * Initialize
 * */
 /*
void video_init() {
   print_clear();
}
*/

/*
 * Get current foreground color
 * */
uint8_t get_curr_color() {
     return curr_color;
}


