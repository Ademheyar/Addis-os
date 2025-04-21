#include <Libs/Stdarg/Stdarg.h>
#include <Addis/Libs/String/String.h>
#include <Libs/Stdint/Stdint.h>
#include <Libs/Stdlib/Stdlib.h>
#include <Libs/Stdio/Stdio.h>
#include <Libs/Malloc/Mmu_paging.h>
#include <Libs/Malloc/Mmu_heap.h>

#define		PR_LJ	0x01
#define		PR_CA	0x02
#define		PR_SG	0x04
#define		PR_64	0x08
#define		PR_32	0x10
#define		PR_WS	0x20
#define		PR_LZ	0x40
#define		PR_FP	0x80

#define		PR_BUFLEN	32

int _printf(const char *fmt, va_list args, fnptr_t fn, void *ptr)
{
unsigned state, flags, radix, actual_wd, count, given_wd;
char *where, buf[PR_BUFLEN];
long num;

	state = flags = count = given_wd = 0;
	for(; *fmt; fmt++)
	{
		switch(state)
		{
		case 0:
			if(*fmt != '%')	/* not %... */
			{
				fn(*fmt, ptr);	/* ...just echo it */
				count++;
				break;
			}
			state++;
			fmt++;
			/* FALL THROUGH */
		case 1:
			if(*fmt == '%')	/* %% */
			{
				fn(*fmt, ptr);
				count++;
				state = flags = given_wd = 0;
				break;
			}
			if(*fmt == '-')
			{
				if(flags & PR_LJ)/* %-- is illegal */
					state = flags = given_wd = 0;
				else
					flags |= PR_LJ;
				break;
			}
			state++;
			if(*fmt == '0')
			{
				flags |= PR_LZ;
				fmt++;
			}
			/* FALL THROUGH */
		case 2:
			if(*fmt >= '0' && *fmt <= '9')
			{
				given_wd = 10 * given_wd +
					(*fmt - '0');
				break;
			}
			state++;
			/* FALL THROUGH */
		case 3:
			if(*fmt == 'F')
			{
				flags |= PR_FP;
				break;
			}
			if(*fmt == 'N')
				break;
			if(*fmt == 'l')
			{
				flags |= PR_64;
				break;
			}
			if(*fmt == 'h')
			{
				flags |= PR_32;
				break;
			}
			state++;
			/* FALL THROUGH */
		case 4:
			where = buf + PR_BUFLEN - 1;
			*where = '\0';
			switch(*fmt)
			{
			case 'X':
				flags |= PR_CA;
				/* FALL THROUGH */
			case 'x':
			case 'p':
				flags |= PR_64;
				__attribute__ ((fallthrough));
			case 'n':
				radix = 16;
				goto DO_NUM;
			case 'b':
				radix = 2;
				goto DO_NUM;
			case 'd':
			case 'i':
				flags |= PR_SG;
				/* FALL THROUGH */
			case 'u':
				radix = 10;
				goto DO_NUM;
			case 'o':
				radix = 8;
DO_NUM:				if(flags & PR_64)
						num = va_arg(args, int64_t);
					else if(flags & PR_32)
					{
						if(flags & PR_SG)
							num = va_arg(args, int32_t);
						else
							num = va_arg(args, uint32_t);
					}
					else
					{
						if(flags & PR_SG)
							num = va_arg(args, int32_t);
						else
							num = va_arg(args, uint32_t);
					}
					if(flags & PR_SG)
					{
						if(num < 0)
						{
							flags |= PR_WS;
							num = -num;
						}
					}
					do
					{
						uint64_t temp;

						temp = (uint64_t)num % radix;
						where--;
						if(temp < 10)
							*where = temp + '0';
						else if(flags & PR_CA)
							*where = temp - 10 + 'A';
						else
							*where = temp - 10 + 'a';
						num = (uint64_t)num / radix;
					}
					while(num != 0);
				goto EMIT;
			case 'c':
				flags &= ~PR_LZ;
				where--;
				*where = (char)va_arg(args, int);
				actual_wd = 1;
				goto EMIT2;
			case 's':
				flags &= ~PR_LZ;
				where = va_arg(args, char *);
EMIT:
				actual_wd = strlen(where);
				if(flags & PR_WS)
					actual_wd++;
				if((flags & (PR_WS | PR_LZ)) ==
					(PR_WS | PR_LZ))
				{
					fn('-', ptr);
					count++;
				}
EMIT2:				if((flags & PR_LJ) == 0)
				{
					while(given_wd > actual_wd)
					{
						fn(flags & PR_LZ ? '0' :
							' ', ptr);
						count++;
						given_wd--;
					}
				}
				if((flags & (PR_WS | PR_LZ)) == PR_WS)
				{
					fn('-', ptr);
					count++;
				}
				while(*where != '\0')
				{
					fn(*where++, ptr);
					count++;
				}
				if(given_wd < actual_wd)
					given_wd = 0;
				else given_wd -= actual_wd;
				for(; given_wd; given_wd--)
				{
					fn(' ', ptr);
					count++;
				}
				break;
			default:
				break;
			}
		__attribute__ ((fallthrough));
		default:
			state = flags = given_wd = 0;
			break;
		}
	}
	return count;
}

static void print_dec(unsigned int value, unsigned int width, char * buf, int * ptr, int fill_zero, int align_right, int precision) {
	unsigned int n_width = 1;
	unsigned int i = 9;
	if (precision == -1) precision = 1;

	if (value == 0) {
		n_width = 0;
	} else if (value < 10UL) {
		n_width = 1;
	} else if (value < 100UL) {
		n_width = 2;
	} else if (value < 1000UL) {
		n_width = 3;
	} else if (value < 10000UL) {
		n_width = 4;
	} else if (value < 100000UL) {
		n_width = 5;
	} else if (value < 1000000UL) {
		n_width = 6;
	} else if (value < 10000000UL) {
		n_width = 7;
	} else if (value < 100000000UL) {
		n_width = 8;
	} else if (value < 1000000000UL) {
		n_width = 9;
	} else {
		n_width = 10;
	}

	if (n_width < (unsigned int)precision) n_width = precision;

	int printed = 0;
	if (align_right) {
		while (n_width + printed < width) {
			buf[*ptr] = fill_zero ? '0' : ' ';
			*ptr += 1;
			printed += 1;
		}

		i = n_width;
		while (i > 0) {
			unsigned int n = value / 10;
			int r = value % 10;
			buf[*ptr + i - 1] = r + '0';
			i--;
			value = n;
		}
		*ptr += n_width;
	} else {
		i = n_width;
		while (i > 0) {
			unsigned int n = value / 10;
			int r = value % 10;
			buf[*ptr + i - 1] = r + '0';
			i--;
			value = n;
			printed++;
		}
		*ptr += n_width;
		while (printed < (int)width) {
			buf[*ptr] = fill_zero ? '0' : ' ';
			*ptr += 1;
			printed += 1;
		}
	}
}

/*
 * Hexadecimal to string
 */
static void print_hex(unsigned int value, unsigned int width, char * buf, int * ptr) {
	int i = width;

	if (i == 0) i = 8;

	unsigned int n_width = 1;
	unsigned int j = 0x0F;
	while (value > j && j < MUINT32_MAX) {
		n_width += 1;
		j *= 0x10;
		j += 0x0F;
	}

	while (i > (int)n_width) {
		buf[*ptr] = '0';
		*ptr += 1;
		i--;
	}

	i = (int)n_width;
	while (i-- > 0) {
		buf[*ptr] = "0123456789abcdef"[(value>>(i*4))&0xF];
		*ptr += + 1;
	}
}

/*
 * vasprintf()
 */
int xvasprintf(char * buf, const char * fmt, va_list args) {
	int i = 0;
	char * s;
	char * b = buf;
	int precision = -1;
	for (const char *f = fmt; *f; f++) {
		if (*f != '%') {
			*b++ = *f;
			continue;
		}
		++f;
		unsigned int arg_width = 0;
		int align = 1; /* right */
		int fill_zero = 0;
		int big = 0;
		int alt = 0;
		int always_sign = 0;
		while (1) {
			if (*f == '-') {
				align = 0;
				++f;
			} else if (*f == '#') {
				alt = 1;
				++f;
			} else if (*f == '*') {
				arg_width = (char)va_arg(args, int);
				++f;
			} else if (*f == '0') {
				fill_zero = 1;
				++f;
			} else if (*f == '+') {
				always_sign = 1;
				++f;
			} else {
				break;
			}
		}
		while (*f >= '0' && *f <= '9') {
			arg_width *= 10;
			arg_width += *f - '0';
			++f;
		}
		if (*f == '.') {
			++f;
			precision = 0;
			if (*f == '*') {
				precision = (int)va_arg(args, int);
				++f;
			} else  {
				while (*f >= '0' && *f <= '9') {
					precision *= 10;
					precision += *f - '0';
					++f;
				}
			}
		}
		if (*f == 'l') {
			big = 1;
			++f;
			if (*f == 'l') {
				big = 2;
				++f;
			}
		}
		if (*f == 'z') {
			big = 1;
			++f;
		}
		/* fmt[i] == '%' */
		switch (*f) {
			case 's': /* String pointer -> String */
				{
					size_t count = 0;
					if (big) {
						wchar_t * ws = (wchar_t *)va_arg(args, wchar_t *);
						if (ws == NULL) {
							ws = L"(null)";
						}
						size_t count = 0;
						while (*ws) {
							*b++ = *ws++;
							count++;
							if (arg_width && count == arg_width) break;
						}
					} else {
						s = (char *)va_arg(args, char *);
						if (s == NULL) {
							s = "(null)";
						}
						if (precision >= 0) {
							while (*s && precision > 0) {
								*b++ = *s++;
								count++;
								precision--;
								if (arg_width && count == arg_width) break;
							}
						} else {
							while (*s) {
								*b++ = *s++;
								count++;
								if (arg_width && count == arg_width) break;
							}
						}
					}
					while (count < arg_width) {
						*b++ = ' ';
						count++;
					}
				}
				break;
			case 'c': /* Single character */
				*b++ = (char)va_arg(args, int);
				break;
			case 'p':
				if (!arg_width) {
					arg_width = 8;
				}
			case 'x': /* Hexadecimal number */
				if (alt) {
					*b++ = '0';
					*b++ = 'x';
				}
				i = b - buf;
				if (big == 2) {
					unsigned long long val = (unsigned long long)va_arg(args, unsigned long long);
					if (val > 0xFFFFFFFF) {
						print_hex(val >> 32, arg_width > 8 ? (arg_width - 8) : 0, buf, &i);
					}
					print_hex(val & 0xFFFFFFFF, arg_width > 8 ? 8 : arg_width, buf, &i);
				} else {
					print_hex((unsigned long)va_arg(args, unsigned long), arg_width, buf, &i);
				}
				b = buf + i;
				break;
			case 'i':
			case 'd': /* Decimal number */
				i = b - buf;
				{
					long long val;
					if (big == 2) {
						val = (long long)va_arg(args, long long);
					} else {
						val = (long)va_arg(args, long);
					}
					if (val < 0) {
						*b++ = '-';
						buf++;
						val = -val;
					} else if (always_sign) {
						*b++ = '+';
						buf++;
					}
					print_dec(val, arg_width, buf, &i, fill_zero, align, precision);
				}
				b = buf + i;
				break;
			case 'u': /* Unsigned ecimal number */
				i = b - buf;
				{
					unsigned long long val;
					if (big == 2) {
						val = (unsigned long long)va_arg(args, unsigned long long);
					} else {
						val = (unsigned long)va_arg(args, unsigned long);
					}
					print_dec(val, arg_width, buf, &i, fill_zero, align, precision);
				}
				b = buf + i;
				break;
			case 'g': /* supposed to also support e */
			case 'f':
				{
					/*double val = (double)va_arg(args, double);
					i = b - buf;
					if (val < 0) {
						*b++ = '-';
						buf++;
						val = -val;
					}
					print_dec((long)val, arg_width, buf, &i, fill_zero, align, 1);
					b = buf + i;
					i = b - buf;
					*b++ = '.';
					buf++;
					for (int j = 0; j < ((precision > -1 && precision < 8) ? precision : 8); ++j) {
						if ((int)(val * 100000.0) % 100000 == 0 && j != 0) break;
						val *= 10.0;
						print_dec((int)(val) % 10, 0, buf, &i, 0, 0, 1);
					}
					b = buf + i;*/
				}
				break;
			case '%': /* Escape */
				*b++ = '%';
				break;
			default: /* Nothing at all, just dump it */
				*b++ = *f;
				break;
		}
	}
	/* Ensure the buffer ends in a null */
	*b = '\0';
	return b - buf;
}

int vasprintf(char ** buf, const char * fmt, va_list args) {
	char * b = malloc(1024);
	*buf = b;
	return xvasprintf(b, fmt, args);
}

int vsprintf(char * buf, const char *fmt, va_list args) {
	return xvasprintf(buf, fmt, args);
}

int vsnprintf(char * buf, const char *fmt, va_list args) {
	/* XXX */
	return xvasprintf(buf, fmt, args);
}

int vfprintf(FILE * device, const char *fmt, va_list args) {
	char * buffer;
	vasprintf(&buffer, fmt, args);

	int out = fwrite(buffer, 1, strlen(buffer), device);
	free(buffer);
	return out;
}
/*
int vprintf(const char *fmt, va_list args) {
	return vfprintf(stdout, fmt, args);
}*/

int fprintf(FILE * device, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char * buffer;
	vasprintf(&buffer, fmt, args);
	va_end(args);

	int out = fwrite(buffer, 1, strlen(buffer), device);
	free(buffer);
	return out;
}

int printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	char * buffer;
	vasprintf(&buffer, fmt, args);
	va_end(args);
	//int out = fwrite(buffer, 1, strlen(buffer), stdout);
	free(buffer);
	return 0;
}

int sprintf(char * buf, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int out = xvasprintf(buf, fmt, args);
	va_end(args);
	return out;
}

int snprintf(char * buf, size_t size, const char * fmt, ...) {
	/* XXX This is bad. */
	(void)size;
	va_list args;
	va_start(args, fmt);
	int out = xvasprintf(buf, fmt, args);
	va_end(args);
	return out;
}
