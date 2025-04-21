#include <System.h>
#ifndef ASSERT_H
#define ASSERT_H

#define __ASSERT_VOID_CAST (void)

void HALT_AND_CATCH_FIRE(const char *fmt, ...);

/* Panic */
//#define HALT_AND_CATCH_FIRE(mesg, regs) halt_and_catch_fire(mesg, __FILE__, __LINE__, regs)
//#define assert(statement) ((statement) ? (void)0 : assert_failed(__FILE__, __LINE__, #statement))
//void halt_and_catch_fire(char *error_message, const char *file, int line, struct regs * regs);
void assert_failed(const char *file, uint32_t line, const char *desc);

/*#define assert(expr)							\
	((expr)													\
	 ? __ASSERT_VOID_CAST (0)				\
	 : __assert_fail (#expr, __FILE__, __LINE__, __func__))*/

static inline void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function) {
	HALT_AND_CATCH_FIRE("%s  %s:%i @ %s", __assertion, __file, __line, __function);
}

extern void __assert_func(const char * file, int line, const char * func, const char * failedexpr);
#define assert(statement) ((statement) ? (void)0 : __assert_func(__FILE__, __LINE__, __FUNCTION__, #statement))
//#define assert(statement) ((void)0)
#endif