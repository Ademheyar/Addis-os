// PIC is very complex, for a better understanding, visist
// http://www.brokenthorn.com/Resources/OSDevPic.html or some other materials that explain PIC, otherwise the following code is impossible to uderstand....
 
#include <Addis/Interrupt/Pic.h>
#include <Kernel.h>

#include <Addis/Drivers/Screen/Vesa/Vesa.h>

//#include <pic.h>
#include <x86.h>

void init_kernel_pic() {
    // ICW1
    outp(PIC_MASTER_CTRL, 0x11);  // init master PIC
    outp(PIC_SLAVE_CTRL, 0x11);   // init slave PIC
    // ICW2, irq 0 to 7 is mapped to 0x20 to 0x27, irq 8 to F is mapped to 28 to 2F
    outp(PIC_MASTER_DATA, 0x20);  // IRQ 0..7 remaped to 0x20..0x27
    outp(PIC_SLAVE_DATA, 0x28);   // IRQ 8..15 remaped to 0x28..0x37
    // ICW3, connect master pic with slave pic
    outp(PIC_MASTER_DATA, 0x04);  // set as Master
    outp(PIC_SLAVE_DATA, 0x02);   // set as Slave
    // ICW4, set x86 mode
    outp(PIC_MASTER_DATA, 0x01);  // set x86 mode
    outp(PIC_SLAVE_DATA, 0x01);   // set x86 mode
    // clear the mask register
    outp(PIC_MASTER_DATA, 0xff);  // all interrupts disabled
    outp(PIC_SLAVE_DATA, 0xff);

    // JUST IN CASE ?!?
    __asm__ __volatile__("nop");
}

static uint16_t ocw1 = 0xFFFB;

void irq_enable(uint8_t irq) {
  DEBUG("in irq_enable ......................................................................irq 0%x\n", irq);
  ocw1 &= (uint16_t)~((1 << irq));
  if (irq < 8) outp(PIC_MASTER_DATA, (uint8_t)(ocw1 & 0xFF));
  else outp(PIC_SLAVE_DATA, (uint8_t)(ocw1 >> 8));
  DEBUG("out irq_enable\n");
}

// Tell PIC interrupt is handled
void pic_acknowledge(uint8_t irq) {
  DEBUG("in pic_acknowledge irq %x............................\n\n\n\n\n\n\n\n", irq);
  if (irq > 7) outp(PIC_SLAVE_CTRL, 0x20);
  outp(PIC_MASTER_CTRL, 0x20);
}
