#include <Addis/Libs/String/String.h>
#include <Libs/Stdio/Stdio.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stddef/Stddef.h>
#include <Libs/Stdarg/Stdarg.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Libs/Stdlib/Stdlib.h>
#include <System.h>
#include <Addis/Libs/type/type.h>


#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

#define FAST_MEMCPY 0

#define BITOP(A, B, OP) \
 ((A)[(size_t)(B)/(8*sizeof *(A))] OP (size_t)1<<((size_t)(B)%(8*sizeof *(A))))



#define ALIGN (sizeof(size_t))

#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(X) (((X)-ONES) & ~(X) & HIGHS)


//  ------------------------------------------------------------------------ counting and geting sizes

size_t strlen(const char * s) 
{ // this will give size of str
	const char * a = s;
	const size_t * w;
	for (; (uintptr_t)s % ALIGN; s++) {
		if (!*s) {
			return s-a;
		}
	}
	for (w = (const void *)s; !HASZERO(*w); w++);
	for (s = (const void *)w; *s; s++);

	/*int len = 0;
    while(*s++)
        len++;
    return len;*/

	return s-a;
}


//  ------------------------------------------------------------------------ converting and Adding

char *returnstr(char str[])
{ // to return str[] to *str
  return str;
}

void swap(char *x, char *y) 
{ // inline function to swap two numbers
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j) {
	while (i < j) swap(&buffer[i++], &buffer[j--]);
	return buffer;
}

//void fast_memcpy(char * dst, char * src, uint32_t n);

void *memcpy(void *dest, void *src, register uint64_t len) 
{ // this will copy str to dest needs length most given as len
	register unsigned char *bdest = (unsigned char*)dest;
	register unsigned char *bsrc = (unsigned char*)src;
	#if FAST_MEMCPY
    fast_memcpy(dest, src, len);
    return dest;
	#else
    while(len-- > 0)
		*bdest++ = *bsrc++;
	#endif
	return dest;
}

void strcpy(void *dst, const void *src)
{ // this will copy src to dest and cpy than return length
	char *bdest = (char*)dst;
	char *bsrc = (char*)src;
	//for (; (*dest=*src); src++, dest++);
  while ((*bdest++ = *bsrc++) != 0);
}


/*

this is not good if the word dosnot have \0 it will include deffernt
char * strcat(char *dest, const char *src) {
	char * end = dest;
	while (*end != '\0') {
		++end;
	}
	while (*src) {
		*end = *src;
		end++;
		src++;
	}
	*end = '\0';
	return dest;
}


*/

char* strcat(char* dest, const char* src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

/*
char * strncat(char *dest, const char *src, size_t n) {
	char * end = dest;
	while (*end != '\0') {
		++end;
	}
	size_t i = 0;
	while (*src && i < n) {
		*end = *src;
		end++;
		src++;
		i++;
	}
	*end = '\0';
	return dest;
}
*/

char* strncat(char* dest, const char* src, size_t n)
{
    char* temp = dest + strlen(dest);
    size_t i = 0;
    for (; i < n && src[i] != 0; i++)
    {
        temp[i] = src[i];
    }
    if (i < n)
        temp[i] = 0;
    return dest;
}


char * strdup(char src[]) 
{ // this will create new place and copy give string and returns new place
	int l = strlen(src), i = 0;
	char out[l+1];
	for (; (out[i]=*src); src++, i++);
	out[i] = '\0';
	//DEBUG("num l %d ~  i %d |  \n", l , i);//, (char *)out);
	return memcpy(malloc(i+1), (char *)out, i+1);
}

char *stradd(char dest[], char src[], char con) {
  int countdest = strlen(dest), countsrc = strlen(src);
  int num = countdest + countsrc+1;
  char out[num+1];
  int i = 0;
  for (; (out[i]=*dest); dest++, i++);
  if (con) { out[i] = con; i++;}
  for (; (out[i]=*src); src++, i++);
  out[i] = '\0';
  //DEBUG("num strl(%d + %d) ~  i %d | %s \n", countdest, countsrc, i, out);
	return memcpy(malloc(i+1), (char *)out, i+1);
}

void *int_to_ascii(int n, char str[]) {          
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';         
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
    return str;
}

string int_to_string(int n)
{
	string ch = malloc(50);
	int_to_ascii(n,ch);
	int len = strlength(ch);
	int i = 0;
	int j = len - 1;
	while(i<(len/2 + len%2))
	{
		char tmp = ch[i];
		ch[i] = ch[j];
		ch[j] = tmp;
		i++;
		j--;
	}
	return ch;
}

int str_to_int(string ch)
{
	int n = 0;
	int p = 1;
	int strlen = strlength(ch);
	int i;
	for (i = strlen-1;i>=0;i--)
	{
		n += ((int)(ch[i] - '0')) * p;
		p *= 10;
	}
	return n;
}

uint16_t strtohex(char *str)
{ // if str is not hex will ret 0 else return the hex value
  uint16_t val = 0;
  while (*str) {
    // get current char than increment
    uint16_t byte = *str++;
    // transform hex cha to the 4bit equivalent num using the ascii table indexes
    if(byte>='0' && byte <='9') byte = byte-'0';
    else if (byte>='a' && byte <='f') byte = byte-'a'+10;
    else if (byte>='A' && byte <='F') byte = byte-'A'+10;
    // shift 4 to make space for new digit and add the 4 bits of the new digit
    val = (val << 4) | byte&0xF;
  }
  return val;
}

//  ------------------------------------------------------------------------ commparing
/*
 * Compare two buffer, return 1 if they're the same
 */
 /*
int memcmp(uint8_t * data1, uint8_t * data2, int n) {
    while(n--) {
        if(*data1 != *data2)
            return 0;
        data1++;
        data2++;
    }
    return 1;
}
*/


/*int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
    if (num == 0) return (0);

    const uint8_t* s1 = ptr1;
    const uint8_t* s2 = ptr2;
    for (; num > 1 && *s1 == *s2; num--)
    {
        ++s1;
        ++s2;
    }
    return (*s1 - *s2);
}*/

int memcmp (const void *str1, const void *str2, size_t count) 
{ // this will compere two strings 
  register const unsigned char *s1 = (const unsigned char*)str1;
  register const unsigned char *s2 = (const unsigned char*)str2;

  while (count-- > 0) {
    if (*s1++ != *s2++) {
		  return s1[-1] < s2[-1] ? -1 : 1;
    }
  }
  return 0;
}

_Bool issame(char *first1, char *second2){
  //first1 = first1 + '\0';
  int a = strlen(first1), b = strlen(second2), c=0;
  if(a != b) return false; // if length are not equal they are not same
  for(;c<a || c<b; c++){
    // DEBUG("in isame %c == %c \n", first1[c], second2[c]);
    if (!(first1[c] == second2[c])) return false;
  }
  return true;
}











































/*#include <Addis/Libs/String/String.h>

#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stddef/Stddef.h>
#include <Libs/Stdlib/Stdlib.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Addis/Libs/type/type.h>

#include <Libs/Printf.h>
#include <Libs/Stdarg/Stdarg.h>
#include <Kernel.h>
*/























































char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = '\0';
    //DEBUG(" in str_split { %s\n", a_str);
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    //DEBUG("\ncount %d\n", count);
    result = malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token)
        {
          //assert(idx < count);
          //DEBUG(" splited word %d [%s] of %d \n", idx, token, count);
          *(result + idx++) = strdup(token);
          token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }
    //DEBUG("");
    return result;
}


int strcasecmp(const char * s1, const char * s2) {
	for (; tolower(*s1) == tolower(*s2) && *s1; s1++, s2++);
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncasecmp(const char *s1, const char *s2, size_t n) {
	if (n == 0) return 0;

	while (n-- && tolower(*s1) == tolower(*s2)) {
		if (!n || !*s1) break;
		s1++;
		s2++;
	}
	return (unsigned int)tolower(*s1) - (unsigned int)tolower(*s2);
}

void * memchr(const void * src, int c, size_t n) {
	const unsigned char * s = src;
	c = (unsigned char)c;
	for (; ((uintptr_t)s & (ALIGN - 1)) && n && *s != c; s++, n--);
	if (n && *s != c) {
		const size_t * w;
		size_t k = ONES * c;
		for (w = (const void *)s; n >= sizeof(size_t) && !HASZERO(*w^k); w++, n -= sizeof(size_t));
		for (s = (const void *)w; n && *s != c; s++, n--);
	}
	return n ? (void *)s : 0;
}

void * memrchr(const void * m, int c, size_t n) {
	const unsigned char * s = m;
	c = (unsigned char)c;
	while (n--) {
		if (s[n] == c) {
			return (void*)(s+n);
		}
	}
	return 0;
}

int strcoll(const char * s1, const char * s2) {
	return strcmp(s1,s2); /* TODO locales */
}

char * stpcpy(char * restrict d, const char * restrict s) {
	size_t * wd;
	const size_t * ws;

	if ((uintptr_t)s % ALIGN == (uintptr_t)d % ALIGN) {
		for (; (uintptr_t)s % ALIGN; s++, d++) {
			if (!(*d = *s)) {
				return d;
			}
		}
		wd = (void *)d;
		ws = (const void *)s;
		for (; !HASZERO(*ws); *wd++ = *ws++);
		d = (void *)wd;
		s = (const void *)ws;
	}

	for (; (*d=*s); s++, d++);

	return d;
}





size_t strspn(const char * s, const char * c) {
	const char * a = s;
	size_t byteset[32/sizeof(size_t)] = { 0 };

	if (!c[0]) {
		return 0;
	}
	if (!c[1]) {
		for (; *s == *c; s++);
		return s-a;
	}

	for (; *c && BITOP(byteset, *(unsigned char *)c, |=); c++);
	for (; *s && BITOP(byteset, *(unsigned char *)s, &); s++);

	return s-a;
}

char * strchrnul(const char * s, int c) {
	size_t * w;
	size_t k;

	c = (unsigned char)c;
	if (!c) {
		return (char *)s + strlen(s);
	}

	for (; (uintptr_t)s % ALIGN; s++) {
		if (!*s || *(unsigned char *)s == c) {
			return (char *)s;
		}
	}

	k = ONES * c;
	for (w = (void *)s; !HASZERO(*w) && !HASZERO(*w^k); w++);
	for (s = (void *)w; *s && *(unsigned char *)s != c; s++);
	return (char *)s;
}

/*
char * strchr(const char * s, int c) {
	char *r = strchrnul(s, c);
	return *(unsigned char *)r == (unsigned char)c ? r : 0;
}
char * strrchr(const char * s, int c) {
	return memrchr(s, c, strlen(s) + 1);
}*/

size_t strcspn(const char * s, const char * c) {
	const char *a = s;
	if (c[0] && c[1]) {
		size_t byteset[32/sizeof(size_t)] = { 0 };
		for (; *c && BITOP(byteset, *(unsigned char *)c, |=); c++);
		for (; *s && !BITOP(byteset, *(unsigned char *)s, &); s++);
		return s-a;
	}
	return strchrnul(s, *c)-a;
}
/*
char * strpbrk(const char * s, const char * b) {
	s += strcspn(s, b);
	return *s ? (char *)s : 0;
}*/
/*
static char *strstr_2b(const unsigned char * h, const unsigned char * n) {
	uint16_t nw = n[0] << 8 | n[1];
	uint16_t hw = h[0] << 8 | h[1];
	for (h++; *h && hw != nw; hw = hw << 8 | *++h);
	return *h ? (char *)h-1 : 0;
}

static char *strstr_3b(const unsigned char * h, const unsigned char * n) {
	uint32_t nw = n[0] << 24 | n[1] << 16 | n[2] << 8;
	uint32_t hw = h[0] << 24 | h[1] << 16 | h[2] << 8;
	for (h += 2; *h && hw != nw; hw = (hw|*++h) << 8);
	return *h ? (char *)h-2 : 0;
}

static char *strstr_4b(const unsigned char * h, const unsigned char * n) {
	uint32_t nw = n[0] << 24 | n[1] << 16 | n[2] << 8 | n[3];
	uint32_t hw = h[0] << 24 | h[1] << 16 | h[2] << 8 | h[3];
	for (h += 3; *h && hw != nw; hw = hw << 8 | *++h);
	return *h ? (char *)h-3 : 0;
}
static char *strstr_twoway(const unsigned char * h, const unsigned char * n) {
	size_t mem;
	size_t mem0;
	size_t byteset[32 / sizeof(size_t)] = { 0 };
	size_t shift[256];
	size_t l;

	// Computing length of needle and fill shift table 
	for (l = 0; n[l] && h[l]; l++) {
		BITOP(byteset, n[l], |=);
		shift[n[l]] = l+1;
	}

	if (n[l]) {
		return 0; // hit the end of h 
	}

	// Compute maximal suffix 
	size_t ip = -1;
	size_t jp = 0;
	size_t k = 1;
	size_t p = 1;
	while (jp+k<l) {
		if (n[ip+k] == n[jp+k]) {
			if (k == p) {
				jp += p;
				k = 1;
			} else {
				k++;
			}
		} else if (n[ip+k] > n[jp+k]) {
			jp += k;
			k = 1;
			p = jp - ip;
		} else {
			ip = jp++;
			k = p = 1;
		}
	}
	size_t ms = ip;
	size_t p0 = p;

	// And with the opposite comparison 
	ip = -1;
	jp = 0;
	k = p = 1;
	while (jp+k<l) {
		if (n[ip+k] == n[jp+k]) {
			if (k == p) {
				jp += p;
				k = 1;
			} else {
				k++;
			}
		} else if (n[ip+k] < n[jp+k]) {
			jp += k;
			k = 1;
			p = jp - ip;
		} else {
			ip = jp++;
			k = p = 1;
		}
	}
	if (ip+1 > ms+1) {
		ms = ip;
	} else {
		p = p0;
	}

	// Periodic needle?
	if (memcmp(n, n+p, ms+1)) {
		mem0 = 0;
		p = MAX(ms, l-ms-1) + 1;
	} else {
		mem0 = l-p;
	}
	mem = 0;

	// Initialize incremental end-of-haystack pointer
	const unsigned char * z = h;

	// Search loop 
	for (;;) {
		// Update incremental end-of-haystack pointer 
		if ((size_t)(z-h) < l) {
			// Fast estimate for MIN(l,63) 
			size_t grow = l | 63;
			const unsigned char *z2 = memchr(z, 0, grow);
			if (z2) {
				z = z2;
				if ((size_t)(z-h) < l) {
					return 0;
				}
			} else {
				z += grow;
			}
		}

		// Check last byte first; advance by shift on mismatch 
		if (BITOP(byteset, h[l-1], &)) {
			k = l-shift[h[l-1]];
			if (k) {
				if (mem0 && mem && k < p) k = l-p;
				h += k;
				mem = 0;
				continue;
			}
		} else {
			h += l;
			mem = 0;
			continue;
		}

		// Compare right half 
		for (k=MAX(ms+1,mem); n[k] && n[k] == h[k]; k++);
		if (n[k]) {
			h += k-ms;
			mem = 0;
			continue;
		}
		// Compare left half 
		for (k=ms+1; k>mem && n[k-1] == h[k-1]; k--);
		if (k <= mem) {
			return (char *)h;
		}
		h += p;
		mem = mem0;
	}
}
*/

long atol(const char * s) {
	int n = 0;
	int neg = 0;
	while (isspace(*s)) {
		s++;
	}
	switch (*s) {
		case '-':
			neg = 1;
			/* Fallthrough is intentional here */
		case '+':
			s++;
	}
	while (isdigit(*s)) {
		n = 10*n - (*s++ - '0');
	}
	/* The sign order may look incorrect here but this is correct as n is calculated
	 * as a negative number to avoid overflow on INT_MAX.
	 */
	return neg ? n : -n;
}

size_t lfind(const char * str, const char accept) {
	return (size_t)strchr(str, accept);
}

size_t rfind(const char * str, const char accept) {
	return (size_t)strrchr(str, accept);
}

/*
static void memcpyr(void* dest, const void* src, size_t bytes)
{
    // Calculate starting addresses
    void* temp = dest+bytes-1;
    src += bytes-1;

    size_t dwords = bytes/4;
    bytes %= 4;

    __asm__ volatile("std\n"
                     "rep movsb\n"
                     "sub $3, %%edi\n"
                     "sub $3, %%esi\n"
                     "mov %%edx, %%ecx\n"
                     "rep movsl\n"
                     "cld" : "+S"(src), "+D"(temp), "+c"(bytes) : "d"(dwords) : "memory");
}

void* memmove(void* dest, const void* src, size_t bytes)
{
    if (src == dest || bytes == 0) // Copying is not necessary. Calling memmove with source==destination or size==0 is not a bug.
    {
        return (dest);
    }

    // Check for out of memory
    const uintptr_t memMax = ~((uintptr_t)0) - (bytes - 1); // ~0 is the highest possible value of the variables type. (No underflow possible on substraction, because size < adress space)
    if ((uintptr_t)src > memMax || (uintptr_t)dest > memMax)
    {
        return (dest);
    }

    // Arrangement of the destination and source decides about the direction of copying
    if (src < dest)
    {
        memcpyr(dest, src, bytes);
    }
    else // In all other cases, it is ok to copy from the start to the end of source.
    {
        memcpy(dest, src, bytes); // We assume, that memcpy does forward copy
    }
    return (dest);
} */

char* strchr(const char* str, char character)
{
    for (;; str++)
    {
        // the order here is important
        if (*str == character)
        {
            return (char*)str;
        }
        if (*str == 0) // end of string
        {
            return (0);
        }
    }
}

char* strrchr(const char* s, int c)
{
    const char* p = s + strlen(s);
    if (c == 0)
        return (char*)p;
    while (p != s)
    {
        if (*(--p) == c)
            return (char*)p;
    }
    return (0);
}

int strcoll(const char* str1, const char* str2); /// TODO
/*
size_t strcspn(const char* str, const char* key)
{
    const char* ostr = str;
    for (; *str != 0; str++)
        if (strchr(key, *str) != 0)
            return (str-ostr);
    return (str-ostr);
}

char* strerror(int errornum)
{
    switch (errornum)
    {
        case EDOM:
            return "Domain error";
        case ERANGE:
            return "Range error";
        default:
            return "";
    }
}*/

char* strpbrk(const char* str, const char* delim)
{
    for (; *str != 0; str++)
        for (size_t i = 0; delim[i] != 0; i++)
            if (*str == delim[i])
                return ((char*)str);

    return (0);
}

/*size_t strspn(const char* str, const char* key)
{
    const char* ostr = str;
    for (; *str != 0; str++)
        if (strchr(key, *str) == 0)
            return (str-ostr);
    return (str-ostr);
}
*/
char * strtok_r(char * str, const char * delim, char ** saveptr) {
	char * token;
	if (str == NULL) {
		str = *saveptr;
	}
	str += strspn(str, delim);
	if (*str == '\0') {
		*saveptr = str;
		return NULL;
	}
	token = str;
	str = strpbrk(token, delim);
	if (str == NULL) {
		*saveptr = (char *)lfind(token, '\0');
	} else {
		*str = '\0';
		*saveptr = str + 1;
	}
	return token;
}

char * strtok(char * str, const char * delim) {
	static char * saveptr = NULL;
	if (str) {
			saveptr = NULL;
	}
	return strtok_r(str, delim, &saveptr);
}

size_t strxfrm(char* destination, const char* source, size_t num); /// TODO


/*
* Copyright (c) 2010-2017 The PrettyOS Project. All rights reserved.
*
* http://www.prettyos.de
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTstruct COLUMNS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
* CONTRIBUTstruct COLUMNS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



void *memset(void* dest, register int value, register uint64_t len) {
	register unsigned char *ptr = (unsigned char*)dest;
	while(len-- > 0) {
		*ptr++ = value;
	}
	return dest;
}

/*
void *memset(void *dst,char val, int n)
{
    char *temp = dst;
    for(;n != 0; n--) *temp++ = val;
    return dst;
}

void* memset(void* dest, char val, size_t bytes)
{
    void* retval = dest;
    uint8_t uval = val;
    size_t dwords = bytes/4; // Number of dwords (4 Byte blocks) to be written
    bytes %= 4;              // Remaining bytes
    uint32_t dval = (uval<<24)|(uval<<16)|(uval<<8)|uval; // Create dword from byte value
    __asm__ volatile("cld\n"
                     "rep stosl\n"
                     "mov %%edx, %%ecx\n"
                     "rep stosb" : "+D"(dest), "+c"(dwords) : "a"(dval), "d"(bytes) : "memory");
    return retval;
}*/


/*
 * memset by word(16 bit)
 * */
uint16_t *memsetw(uint16_t *dest, uint16_t val, uint32_t count)
{	
	uint16_t *temp = (uint16_t *)dest;
	for( ; count != 0; count--) *temp++ = val;
  //DEBUG("mmemseting by word 16bit\n");
	return dest;
}

/*
 * memset by double word(32 bit)
 * */
uint32_t *memsetdw(uint32_t *dest, uint32_t val, uint32_t count)
{
    uint32_t *temp = (uint32_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

/*int strcmp(const char *dst, char *src)
{
    int i = 0;

    while ((dst[i] == src[i])) {
        if (src[i++] == 0)
            return 0;
    }

    return 1;
}
*/

int strcmp(const char * l, const char * r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}


// Return a pointer to the first occurrence of str in in, or a null pointer if str is not part of in.
char * strstr(const char *in, const char *str) {
    char c;
    uint32_t len;

    c = *str++;
    if (!c)
        return (char *) in;

    len = strlen(str);
    do {
        char sc;

        do {
            sc = *in++;
            if (!sc)
                return (char *) 0;
        } while (sc != c);
    } while (strncmp(in, str, len) != 0);

    return (char *) (in - 1);
}


/*
char *strstr(const char * h, const char * n) {
	// Return immediately on empty needle 
	if (!n[0]) {
		return (char *)h;
	}

	// Use faster algorithms for short needles 
	h = strchr(h, *n);
	if (!h || !n[1]) {
		return (char *)h;
	}

	if (!h[1]) return 0;
	if (!n[2]) return strstr_2b((void *)h, (void *)n);
	if (!h[2]) return 0;
	if (!n[3]) return strstr_3b((void *)h, (void *)n);
	if (!h[3]) return 0;
	if (!n[4]) return strstr_4b((void *)h, (void *)n);

	// Two-way on large needles 
	return strstr_twoway((void *)h, (void *)n);
}

char* strstr(const char* str1, const char* str2)
{
    const char* p1 = str1;
    while (*str1)
    {
        const char* p2 = str2;
        while (*p2 && (*p1 == *p2))
        {
            ++p1;
            ++p2;
        }
        if (*p2 == 0)
        {
            return (char*)str1;
        }
        ++str1;
        p1 = str1;
    }
    return (0);
}*/



char* strncpy(char* dest, const char* src, size_t num) {
	char* ptr = dest;
	if (dest == NULL)
		return NULL;
	while (*src && num--) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';

	return ptr;
}
/*
char *strncpy(char *destString, const char *sourceString,int maxLength)
{
    unsigned count;

    if ((destString == (char *) NULL) || (sourceString == (char *) NULL))
    {
        return (destString = NULL);
    }

    if (maxLength > 255)
        maxLength = 255;

    for (count = 0; (int)count < (int)maxLength; count ++)
    {
        destString[count] = sourceString[count];

        if (sourceString[count] == '\0')
            break;
    }

    if (count >= 255)
    {
        return (destString = NULL);
    }

    return (destString);
}

char* strncpy(char* dest, const char* src, size_t n)
{
    size_t i = 0;
    for (; i < n && src[i] != 0; i++)
    {
        dest[i] = src[i];
    }
    memset(dest+i, 0, n-i);
    return (dest);
}
*/

int strncmp(const char* s1, const char* s2, size_t n)
{
    if (n == 0) return (0);

    for (; *s1 && n > 1 && *s1 == *s2; n--)
    {
        ++s1;
        ++s2;
    }
    return (*s1 - *s2);
}
/*
int strncmp( const char* s1, const char* s2, int c ) {
    int result = 0;

    while ( c ) {
        result = *s1 - *s2++;

        if ( ( result != 0 ) || ( *s1++ == 0 ) ) {
            break;
        }

        c--;
    }
    return result;
}
*/
// Iterative function to implement itoa() function in C
// value = num, buffer = text, base = num base
char* itoa(int value, char* buffer, int base) {
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

/*void itoa(char *buf, unsigned long int n, int base)
{
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
}
*/

/*int atoi(char * string) {
    int result = 0;
    unsigned int digit;
    int sign;

    while (isspace(*string)) {
        string += 1;
    }

    if (*string == '-') {
        sign = 1;
        string += 1;
    } else {
        sign = 0;
        if (*string == '+') {
            string += 1;
        }
    }

    for ( ; ; string += 1) {
        digit = *string - '0';
        if (digit > 9) {
            break;
        }
        result = (10*result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}
*/
int atoi(const char * s) {
	int n = 0;
	int neg = 0;
	while (isspace(*s)) {
		s++;
	}

  /*
   * Check for a sign.
   */

	switch (*s) {
		case '-':
			neg = 1;
			/* Fallthrough is intentional here */
		case '+':
			s++;
	}
	while (isdigit(*s)) {
		n = 10*n - (*s++ - '0');
	}
	/* The sign order may look incorrect here but this is correct as n is calculated
	 * as a negative number to avoid overflow on INT_MAX.
	 */
	return neg ? n : -n;
}

char *strsep(char **stringp, const char *delim) {
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

/*
   Split a string into list of strings
   */
list_t *str_splitL(const char * str, const char * delim, unsigned int * numtokens) {
    list_t *ret_list = list_create();
    char *s = strdup((char *)str);
    char *token, *rest = s;
    while ((token = strsep(&rest, delim)) != NULL) {
        if(!strcmp(token, ".")) continue;
        if(!strcmp(token, "..")) {
            if(list_size(ret_list) > 0) list_pop(ret_list);
            continue;
        }
        list_push(ret_list, strdup(token));
        if(numtokens) (*numtokens)++;
    }
    free(s);
    return ret_list;
}

/*
 * Reconstruct the string with tokens and delimiters
 * */
char * list2str(list_t * list, const char * delim) {
    char * ret = malloc(256);
    memset(ret, 0, 256);
    int len = 0, ret_len = 256;
    while(list_size(list)> 0) {
        char * temp = list_pop(list)->value;
        int len_temp = strlen(temp);
        if(len + len_temp + 1 + 1 > ret_len) {
            ret_len = ret_len * 2;
            ret = realloc(ret, ret_len);
            len = len + len_temp + 1;
        }
        strcat(ret, delim);
        strcat(ret, temp);
    }
    return ret;
}
/*
void sprintf(char * buf, const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, NULL, fmt, ap);
    va_end(ap);
}*/
uint16 strlength(string ch)
{
	uint16 i = 0;           //Changed counter to 0
	while(ch[i++]);  
	return i-1;               //Changed counter to i instead of i--
}

/* Added in episode 3*/ /*This function compares two strings and returns true (1) if they are equal or false (0) if they are not equal */

uint8 strEql(string ch1,string ch2)                     
{
        uint8 result = 1;
        uint8 size = strlength(ch1);
        if(size != strlength(ch2)) result =0;
        else 
        {
        uint8 i = 0;
        for(;i<=size;i++)
        {
                if(ch1[i] != ch2[i]) result = 0;
        }
        }
        return result;
}


void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);             //    dest[i] = source[i]
    }
}

void memory_set(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}
