#include <Libs/Assert/Assert.h>
#include <Drivers/Serial/Serial.h>
#include <x86.h>
#include <Kernel.h>

void __assert_func(const char * file, int line, const char * func, const char * failedexpr) {
file = file;line=line;func=func;failedexpr =failedexpr;
 //fprintf(stderr, "Assertion failed in %s:%d (%s): %s\n", file, line, func, failedexpr);
 // void
}

int serial_printf_help(unsigned c, void *ptr __UNUSED__) {
  serial_write_com(1, c);
  return 0;
}

void _kdebug(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  (void)_printf(fmt, args, serial_printf_help, NULL);
  va_end(args);
}

void HALT_AND_CATCH_FIRE(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  (void)_printf(fmt, args, serial_printf_help, NULL);
  va_end(args);
  x86_hlt();
  while(1) {}
}
