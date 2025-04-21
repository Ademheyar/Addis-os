

extern char * gettext (const char * msgid);
extern char * dgettext (const char * domainname, const char * msgid);
extern char * dcgettext (const char * domainname, const char * msgid, int category);

extern char * ngettext (const char * msgid, const char * msgid_plural, unsigned long int n);
extern char * dngettext (const char * domainname, const char * msgid, const char * msgid_plural, unsigned long int n);
extern char * dcngettext (const char * domainname, const char * msgid, const char * msgid_plural, unsigned long int n, int category);

