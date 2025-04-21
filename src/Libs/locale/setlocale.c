#include <Libs/Stdlib/Stdlib.h>
#include <Libs/locale/locale.h>

char * setlocale(int category, const char *locale) {
    category = category; locale = locale;
    return "en_US";
}

