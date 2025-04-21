#ifndef __SERIAL_H
#define __SERIAL_H

#include <Addis/Read_Gets.h>

#define PORT_COM1 0x3f8

void serial_init(void);
void serial_write_com(int, unsigned char);
void init_kernel_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);

void qemu_printf(const char * s, ...);


#endif

