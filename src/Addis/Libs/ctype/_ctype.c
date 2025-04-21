#include <Addis/Libs/type/type.h>

char _ctype_[256]= {
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_C|_S, _C|_S, _C|_S,	_C|_S,	_C|_S,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_S|_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_N,	_N,	_N,	_N,	_N,	_N,	_N,	_N,
	_N,	_N,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P,
	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C
};

// ALL CARACTER

int iscntrl(int c) { 
    return ((c >= 0 && c <= 0x1f) || (c == 0x7f));
}

int isascii(int c) {
	return (c <= 0x7f);
}

// LETTER

int isalpha(int c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

int toupper(int c) {
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 'A';
	}
	return c;
}

int islower(int c) {
    return (c >= 'a' && c <= 'z');
}

int tolower(int c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 'a';
	}
	return c;
}

int isspace(int c) {
    return (c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == ' ');
}


// NUMBER

int isdigit(int c) { // to chack if it is numerical
	return (c >= '0' && c <= '9');
}

int isxdigit(int c) { // to chack if it is hex num
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}


int isalnum(int c) { // to chack if it is letter or number
    return isalpha(c) || isdigit(c);
}

// other cahracters 

int isgraph(int c) { // to chack if it is only main characters from > 32 and < 128
	return (c >= '!' && c <= '~');
}

int isgraphsymbol(int c) { // to chack if it is semboly
	return (!(isalpha(c) || isdigit(c)) && isgraph(c));
}

int ispunct(int c) {
	return isgraph(c) && !isalnum(c);
}

int isprint(int c) { // will chack if c is preanable including space
    return isgraph(c) || c == ' ';
}