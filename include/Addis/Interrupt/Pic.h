#ifndef __PIC_H_
#define __PIC_H_

#include <Addis/Drivers/Screen/Vesa/Vesa.h>

#define PIC_MASTER_CTRL 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CTRL  0xA0
#define PIC_SLAVE_DATA  0xA1

#define ICW1 0x11
#define ICW4 0x01


// Some IRQ constants
#define IRQ_BASE                0x20
// IRQ 0..7 remaped to 0x20..0x27
#define PIC_IRQ0_Timer              0x00
#define PIC_IRQ1_Keyboard           0x01
#define PIC_IRQ2_CASCADE            0x02
#define PIC_IRQ3_SERIAL_PORT2       0x03
#define PIC_IRQ4_SERIAL_PORT1       0x04
#define PIC_IRQ5_RESERVED           0x05
#define PIC_IRQ6_DISKETTE_DRIVE     0x06

// IRQ 8..15 remaped to 0x28..0x37
#define PIC_IRQ7_PARALLEL_PORT      0x07
#define PIC_IRQ8_CMOS_CLOCK         0x08
#define PIC_IRQ9_CGA                0x09
#define PIC_IRQA_RESERVED          0x0A
#define PIC_IRQB_RESERVED          0x0B
#define PIC_IRQC_AUXILIARY         0x0C
#define PIC_IRQD_FPU               0x0D
#define PIC_IRQE_HARD_DISK         0x0E
#define PIC_IRQF_RESERVED          0x0F

void init_kernel_pic();
void pic_acknowledge(uint8_t irq);
void irq_enable(uint8_t irq);
#endif
