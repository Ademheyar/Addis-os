//#include <Libs/Assert/Assert.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Process.h> // for defining task_list_current and other process-related functions

_Bool thisvar(char *istype, char *var){
  if (issame(istype, "HEXA")) {
    char *a = var;
    while(*a) {
      uint8_t byte = *a++;
      if(!(byte=='x' || byte>='0' && byte <='9' || byte>='a' && byte <='f' ||  byte>='A' && byte <='F')) return false;
    }
    return thisvar("int", var) ? false : true;
  }
  if (issame(istype, "int")) {
    char *a = var;
    while(*a) {
      uint8_t byte = *a++;
      if(byte=='x' || byte>='a' && byte <='z' ||  byte>='A' && byte <='Z') return false;
    }
    return true;
  }
  return false;
}



void convert(char *text){

  typedef struct {
    char array[10000];
    int integer;
    uint8_t hexa_8;
    uint16_t hexa_16;
    uint32_t hexa_32;
    char list[10000][100], *prevtex[10];
    int listline, linstcount, prevon;
  }to;
  to convertto;
  int i = 0, hasletter = 0, hasnum = 0;
  for (int c = 0; *(text + c) != '\0'; c++){
    convertto.array[i] = *(text + c);
    i++;
    if (*(text + c)>='a' && *(text + c) <='z' || *(text + c)>='A' && *(text + c) <='Z') hasletter = 1;
    if (*(text + c)>='0' && *(text + c) <='9') hasnum = 1;
  }
  if(hasnum == 1 && hasletter == 0){
    convertto.integer = 0;
    convertto.integer = atoi(text);
  }
  
	convertto.hexa_32 = strtohex(text);
	convertto.hexa_16 = (uint16_t)convertto.hexa_32;
  convertto.hexa_8 = (uint8_t)convertto.hexa_32;
}













char* replace(const char* s, const char* oldW, const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

_Bool contains(char *in, char c, char *str){
  int j =0;
  for(int i = 0; in[i] != '\0'; i++){
    if (c!=0 && in[i]==c) return true;
    else if (str[j] == in[i]) {
      j++;
      if (j==(int)strlen(str)) return true;
    }
    else j = 0;
  }
  return false;
}
