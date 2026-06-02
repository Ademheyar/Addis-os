

/* Derived from newlib */
#define _U  01
#define _L  02
#define _N  04
#define _S  010
#define _P  020
#define _C  040
#define _X  0100
#define _B  0200
//KEYWORDS iskeyword
// -----------------
_Bool isKeyword(char *code);
  ////VARKEYWORD isvarkeyword
//  ----------------
_Bool isvarkeyword(char *code);
    //--VARTYPE iskeywordvartype
_Bool iskeywordvartype(char *code);
 //   -----------------
      //--TYPEPOS
      //--TYPEPROP
      //--TYPE
      //--TYPE
//    -----------------
    //--VARPROP iskeywordvarprop
_Bool iskeywordvarprop(char *code);
 //   -----------------
 //   -----------------
    //--VARPOS iskeywordvarpos
_Bool iskeywordvarpos(char *code);
//    -----------------
  //--VALUEKEYWORD isvaluekeyword
_Bool isvaluekeyword(char *code);
//  -----------------
    //--VALUETYPE iskeywordvaluetype
_Bool iskeywordvaluetype(char *code);
//    -----------------
      //TYPEBOOL
      //TYPENUMBER 
      //TYPESTRING
      //TYPELONG 
//   -----------------
_Bool iskeywordvalueopreat(char *code);
    //VALUEBOOL iskeywordvalueboolen
_Bool iskeywordvalueboolen(char *code);
      //BOOLTRUE
      //BOOLFALSE
    //VALUENUMBER iskeywordvaluenumber
_Bool iskeywordvaluenumber(char *code);
      //BOOLFALSE
      //BOOLFALSE
_Bool iskeywordvaluelocater(char *code);
_Bool iskeywordvaluecolor(char *code);
    //VALUESTRING iskeywordvaluestring
_Bool iskeywordvaluestring(char *code);
      //BOOLFALSE
      //BOOLFALSE
    //VALUELONG iskeywordvaluelong
_Bool iskeywordvaluelong(char *code);
      //BOOLFALSE
      //BOOLFALSE
//  -----------------
//  -----------------

extern int isgraphsymbol(int c);
extern int isalnum(int c);
extern int isalpha(int c);
extern int isdigit(int c);
extern int islower(int c);
extern int isprint(int c);
extern int isgraph(int c);
extern int iscntrl(int c);
extern int ispunct(int c);
extern int isspace(int c);
extern int isupper(int c);
extern int isxdigit(int c);
extern int isascii(int c);


char *get_compar_type(char *value);
extern int tolower(int c);
extern int toupper(int c);

extern char _ctype_[256];