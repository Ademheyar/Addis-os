#include <Libs/Gui/Pictures/Bitmap/Bitmap.h>

//#include <Libs/Malloc/Malloc.h>


Bitmap_t *Bitmap(char *filename) {
  Bitmap_t *bmp_header = calloc(sizeof(Bitmap_t), 1);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
    //DEBUG("[BMP]: Failed to opening BMP header\n");
		return NULL;
	}
	uint32_t size = fsize(file);

	void *buffer = malloc(size + 512);

	fread(buffer, size, 1, file);

	bmp_header->buffer = buffer;
	bmp_header->header = (bmp_header_t*)bmp_header->buffer;
	bmp_header->data = (uint32_t*)(((uint8_t*)buffer) + bmp_header->header->offset);
	// Usually header is negative (means that image is stored : top - to -bottom)
	// bmp_header->header->height_px = abs(bmp_header->header->height_px);
    return bmp_header;
}

void bmp_close(Bitmap_t *bmp_image) {
	free(bmp_image->buffer);
}

// TODO this function will write outside of context buffer with invalid x, y or height and width values
// It should be validated before dumping data
/*void bmp_blit(context_t* context, Bitmap_t *bmp, int x, int y) {
	int height = bmp->header->height_px;
	int width = bmp->header->width_px;
	// If negative : stored TOP -> BOTTOM
	if (height < 0) {
		gfx_blit_c(context, x, y, width, -height, bmp->data);
	}
	else {
		// If positive : stored BOTTOM -> TOP ... make inverted blit (height - i)
		uint32_t *src = bmp->data;
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				CONTEXT_32[FIRST_PIXEL_c(x+j, y+(height-1-i))] = *(src + (j + i * width));
			}
		}		
	}
}*/

/*void bmp_blit_clipped(context_t *context, Bitmap_t *bmp, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h) {
	int w_codeidth = bmp->header->width_px;
	int bmp_height = bmp->header->height_px;

	// If negative : stored TOP -> BOTTOM
	if (bmp_height < 0) {
		uint32_t *src = bmp->data + (clip_y * w_codeidth + clip_x);
		for (int i=0; i<clip_h; i++) {
			for (int j=0; j<clip_w; j++) {
				CONTEXT_32[FIRST_PIXEL_c(x+j, y+i)] = *src++;
			}
			src += (w_codeidth - clip_w);
		}
	}
	else {
		// If positive : stored BOTTOM -> TOP ... make inverted blit (height - i)
		uint32_t *src = bmp->data + ((bmp_height - clip_y - clip_h) * w_codeidth + clip_x);

		for (int i=0; i<clip_h; i++) {
			for (int j=0; j<clip_w; j++) {
				CONTEXT_32[FIRST_PIXEL_c(x+j, y+(clip_h-1-i))] = *src++;
			}
			src += (w_codeidth - clip_w);
		}
	}
}
*/


//////////////////////////////////bmp //////////////////////////////////////



void bmp_from_file(char *filename, Bitmap_t *bmp_out) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	uint32_t size = fsize(file);

	void *buffer = malloc(size + 512);

	fread(buffer, size, 1, file);

	bmp_out->buffer = buffer;
	bmp_out->header = (bmp_header_t*)bmp_out->buffer;
	bmp_out->data = (uint32_t*)(((uint8_t*)buffer) + bmp_out->header->offset);
	// Usually header is negative (means that image is stored : top - to -bottom)
	// bmp_out->header->height_px = abs(bmp_out->header->height_px);
}

