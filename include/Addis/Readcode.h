#ifndef _READCODE_H

#define _READCODE_H

#pragma once


#include <Libs/Stdint/Stdint.h>

#define MAX_PAIRS 64



extern void *get_pathlists(char *paths);
extern void *split_code(char str1[], char split_by);

void READ_DATA_PORT(uint8_t pic_irq);
void register_driver(uint8_t pic_irq, char *name, char *handler, char *retbin);
void enable_driver(uint8_t pic_irq);
char *read_bodys(char text[]);
char *get_code(char *filepath);

struct ROW *get_varinfo(int id, char *id_addrs, char from, char *getwhat);
#endif