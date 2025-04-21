



#include <Libs/Time/Timer.h> // stdio 
// #include <Libs/Stdio/Stdio.h> // hd_driver // ata, Stddef, stdbool, Stdarg // fat // dosfs 
//#include <Addis/Drivers/Filse_system/dosfs/dosfs.h> // mmu_frames // mmu_heap //  mmu_pafing 
//#include <Libs/Malloc/Mmu_paging.h> // #include <Kernel.h> // x86.h // Multiboot.h> 
// #include <Addis/Drivers/BIOSINFO/Multiboot.h> // stdint


//#include <Libs/Gui/windows/window.h>

//#include <Addis/Libs/type/type.h>
//#include <drivers/mouse/mouse.h>
//#include <Libs/Gui/windows/window.h>


/*#include <System.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stddef/Stddef.h>
#include <Libs/Stdarg/Stdarg.h>
#include <Addis/Drivers/Filse_system/Dosfs/Dosfs.h>

#include <Addis/Interrupt/Isr.h>
#include <Addis/Interrupt/Idt.h>
#include <Libs/Syscalls.h>
#include <Memory.h>
#include <Libs/Addisos.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Addis/Libs/String/String.h>
#include <Libs/Errno/Errno.h>
#include <Libs/Stdlib/Stdlib.h>
#include <Libs/Unistd/Unistd.h>
#include <Libs/Fcntl.h>
#include <Addis/Interrupt/Gdt.h>
#include <Addis/Interrupt/Tss.h>
*/
/*
#include <elf.h>
#include <Addis/Process.h>
#include <Libs/kv.h>
*/


#ifndef BITMAP_H
#define BITMAP_H
#pragma once
typedef struct {
    // Constant file signature
    uint16_t type;              // Magic identifier: 0x4d42
    // File size, check
    uint32_t size;              // File size in bytes
    uint16_t reserved1;         // Not used
    uint16_t reserved2;         // Not used
    uint32_t offset;            // Offset to image data in bytes from beginning of file

    // Structure size
    uint32_t dib_header_size;   // DIB Header size in bytes

    // Image definition
    int32_t  width_px;          // Width of the image
    int32_t  height_px;         // Height of image

    // Number of images in file - must be 1
    uint16_t num_planes;        // Number of color planes

    uint16_t bits_per_pixel;    // Bits per pixel

    // Compression (unsupported!)
    uint32_t compression;       // Compression type

    uint32_t image_size_bytes;  // Image size in bytes

    // DPI
    int32_t  x_resolution_ppm;  // Pixels per meter
    int32_t  y_resolution_ppm;  // Pixels per meter

    // Indexed colors fields, unused
    uint32_t num_colors;        // Number of colors
    uint32_t important_colors;  // Important colors
} __attribute__((packed)) bmp_header_t;

typedef struct {
    uint8_t *buffer;
    bmp_header_t *header;
    uint32_t *data;
} Bitmap_t; // bmp_image_t

struct sbmp_head
{
	char magic[8];
	int width;
	int height;
};
typedef unsigned win_color;
struct bitmap
{
	int			width;
	int			height;
	win_color *		pixels;
	struct win_rgba *	rgba;
	int			hotx, hoty;
};

Bitmap_t *Bitmap(char *filename); // read image

//void bmp_from_file(char *filename, Bitmap_t *bmp_out);
//
//void bmp_close(Bitmap_t *bmp_image);
//void bmp_blit(context_t* context, Bitmap_t *bmp, int x, int y);
//void bmp_blit_clipped(context_t *context, Bitmap_t *bmp, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h);




struct bitmap;

//struct bitmap *	bmp_load(const char *pathname);
//struct win_rgba *bmp_rgba(struct bitmap *bmp);
//const win_color *bmp_pixels(struct bitmap *bmp);
//int		bmp_hotspot(struct bitmap *bmp, int *x, int *y);
//int		bmp_size(struct bitmap *bmp, int *w, int *h);
//int		bmp_free(struct bitmap *bmp);
//int		bmp_conv(struct bitmap *bmp);
//int		bmp_draw(int wd, struct bitmap *bmp, int x, int y);
//void		bmp_set_bg(struct bitmap *bmp, int r, int g, int b, int a);
//struct bitmap *	bmp_scale(struct bitmap *bmp, int w, int h);

#endif