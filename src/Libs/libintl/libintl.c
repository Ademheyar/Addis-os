/* Stub. */
#include <Libs/libintl/libintl.h>
char * gettext (const char * msgid) {
	return (char*)msgid;
}

char * dgettext (const char * domainname, const char * msgid) {
    domainname=domainname;
	return (char*)msgid;
}

char * dcgettext (const char * domainname, const char * msgid, int category) {
    domainname=domainname;    category=category;
	return (char*)msgid;
}

char * ngettext (const char * msgid, const char * msgid_plural, unsigned long int n) {
	if (n != 1) return (char*)msgid_plural;
	return (char*)msgid;
}

char * dngettext (const char * domainname, const char * msgid, const char * msgid_plural, unsigned long int n) {
    domainname=domainname;
	if (n != 1) return (char*)msgid_plural;
	return (char*)msgid;
}

char * dcngettext (const char * domainname, const char * msgid, const char * msgid_plural, unsigned long int n, int category) {
    domainname=domainname;    category=category;
	if (n != 1) return (char*)msgid_plural;
	return (char*)msgid;
}
