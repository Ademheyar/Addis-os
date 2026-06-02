#ifndef STRING_H
#define STRING_H

#pragma once


//#include <Addis/Libs/type/type.h>
#include <Addis/Libs/List/List.h> // for defining list_t, listnode_t, and other list-related functions


uint16_t strtohex(char *str);
char *stradd(char dest[], char src[], char con);
extern char * strdup(char * src);
void strcpy(void *dst, const void *src);




//#include <System.h>


/* vim: tabstop=4 shiftwidth=4 noexpandtab
 */

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))


//void *memcpy(void* dst, const void* src, int n);
void *memcpy(void *dest, void *src, register uint64_t len);
//void* memcpy(void* dest, const void* src, size_t bytes);
//void fast_memcpy_v(void* dst, void* src, size_t s);
//void fast_memcpy_c(unsigned char* dst, unsigned char* src, size_t s);
//extern void * memcpy(void * restrict dest, const void * restrict src, size_t n);





extern char * stpcpy(char * restrict d, const char * restrict s);
//extern char * stpcpy(char * d, const char * s);


/*
extern void * memchr(const void * src, int c, size_t n);
extern void * memrchr(const void * m, int c, size_t n);
extern void * memmove(void *dest, const void *src, size_t n);

extern int memcmp(const void *vl, const void *vr, size_t n);

extern char * strchrnul(const char * s, int c);
extern char * strchr(const char * s, int c);
extern char * strrchr(const char * s, int c);
extern char * strpbrk(const char * s, const char * b);
extern char * strstr(const char * h, const char * n);

extern int strcmp(const char * l, const char * r);

extern size_t strcspn(const char * s, const char * c);
extern size_t strspn(const char * s, const char * c);
extern size_t strlen(const char * s);

extern int atoi(const char * s);

extern char * strtok_r(char * str, const char * delim, char ** saveptr);
// Non-standard broken strtok_r 
*/


//int memcmp(uint8_t * data1, uint8_t * data2, int n);

uint16_t * memsetw(uint16_t *dest, uint16_t val, uint32_t count);

uint32_t * memsetdw(uint32_t *dest, uint32_t val, uint32_t count);


//int strlen(const char * s);

//char *strncpy(char *destString, const char *sourceString,int maxLength);

//int strcmp(const char *dst, char *src);

//int strcpy(char *dst,const char *src);

//void strcat(void *dest,const void *src);

//int strncmp( const char* s1, const char* s2, int c );

char * strstr(const char *in, const char *str);

//void itoa(char *buf, unsigned long int n, int base);

//int atoi(char * string);
//int isprint(char c);


char * strsep(char ** stringp, const char * delim);

list_t * str_splitL(const char * str, const char * delim, unsigned int * numtokens);

char * list2str(list_t * list, const char * delim);

//void sprintf(char * buf, const char * fmt, ...);

//extern void * memset(void * dest, int c, size_t n);
//void *memset(void * dst, char val, int n);
//void* memset(void* dest, char val, size_t bytes);
void *memset(void* dest, register int value, register uint64_t len);
extern void * memmove(void * dest, const void * src, size_t n);

void * memchr(const void * src, int c, size_t n);
extern void * memrchr(const void * m, int c, size_t n);
extern int memcmp(const void *vl, const void *vr, size_t n);

//extern void * __attribute__ ((malloc)) valloc(uint64_t size);
extern void free(void * ptr);

extern char * strchrnul(const char * s, int c);
char* strchr(const char* str, char character);
extern char * strrchr(const char * s, int c);
extern char * strpbrk(const char * s, const char * b);
extern char * strstr(const char * h, const char * n);

extern char * strncpy(char * dest, const char * src, size_t n);

extern int strcmp(const char * l, const char * r);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern int strcoll(const char * s1, const char * s2);

extern size_t strcspn(const char * s, const char * c);
extern size_t strspn(const char * s, const char * c);
extern size_t strlen(const char * s);

int atoi(const char * s);

extern char * strcat(char *dest, const char *src);
extern char * strncat(char *dest, const char *src, size_t n);

extern char * strtok(char * str, const char * delim);
extern char * strtok_r(char * str, const char * delim, char ** saveptr);

extern char * strncpy(char *dest, const char *src, size_t n);

extern char * strerror(int errnum);
extern size_t strxfrm(char *dest, const char *src, size_t n);



void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base);

//void* memmove(void* dest, const void* src, size_t bytes);
int memcmp (const void *str1, const void *str2, size_t count);
//int memcmp(const void* ptr1, const void* ptr2, size_t num);

size_t strlen(const char *str);
char* strncpy(char* dest, const char* src, size_t num);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
int strcoll(const char* str1, const char* str2);
size_t strcspn(const char* str, const char* key);
char* strpbrk(const char* str, const char* delim);
char* strrchr(const char* s, int c);
size_t strspn(const char* str, const char* key);
char* strstr(const char* str1, const char* str2);
extern char * strtok(char * str, const char * delim);
extern char * strtok_r(char * str, const char * delim, char ** saveptr);
//char* strerror(int errornum);
size_t strxfrm(char* destination, const char* source, size_t num);

extern int strcasecmp(const char *s1, const char *s2);
extern int strncasecmp(const char *s1, const char *s2, size_t n);

uint16 strlength(string ch);

uint8 strEql(string ch1,string ch2);
void memory_copy(char *source, char *dest, int nbytes);
void memory_set(uint8 *dest, uint8 val, uint32 len);
void *int_to_ascii(int n, char str[]);
string int_to_string(int n);
int str_to_int(string ch);
#endif
