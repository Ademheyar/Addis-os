#include <System.h>

char *get_compar_type(char *value)
{
	int o = 0, w = 0, z = strlen(value)/2;
	char out[z], word[z];
	for(int i = 0; *(value + i); i++){
		char c = toupper(*(value + i));
		if(c == ' ') continue;
		if(c == '|' || c == '=' || c == '&' || c == '<' || c == '>' || c == '!')
		{
			out[o] = c;
			o++;
			continue;
		}

		if(isalpha(c)) {
			word[w] = c;
			w++;
		}
		word[w] = '\0';
	
		if(w == 2 && issame(word, "TO")) w = 0;
		if(w == 2 && issame(word, "OR")) { out[o] = '|'; o++; w = 0; }
		if(w == 3 && issame(word, "NOT")) { out[o] = '!'; o++; w = 0; }
		if(w == 3 && issame(word, "AND")) { out[o] = '&'; o++; w = 0; }
		if(w == 4 && issame(word, "THAN")) w = 0;
		if(w == 4 && issame(word, "LESS")) { out[o] = '<'; o++; w = 0; }
		if(w == 5 && issame(word, "EQUAL")) { out[o] = '='; o++; w = 0; }
		if(w == 6 && issame(word, "GRATER")) { out[o] = '>'; o++; w = 0; }
	}
	out[o] = '\0';
	return strdup(out);
}


// ------------------------------------------KEYWORDS-------------------------------------------------
_Bool isKeyword(char *code) // KEYWORD
{ // cackes all kind of keyword
	if(issame(code, "CASE") || issame(code, "DRIVER") || issame(code, "OPEN") || 
		 issame(code, "READ") || issame(code, "WORDS") || issame(code, "ADD") || 
		 issame(code, "TABLE") || issame(code, "GET") || issame(code, "TAKE") || 
     issame(code, "WHEN") || issame(code, "OPERATE") || isvaluekeyword(code) || 
     isvarkeyword(code))
	return true;
  return false;
}

// -----------------------------------VAR KEYWORDS----------------------------------------------------
_Bool isvarkeyword(char *code) // VAR KEYWORDS
{ // cackes all kind of keyword
	if(iskeywordvarpos(code) || iskeywordvartype(code) || iskeywordvarprop(code)) 
	return true;
  return false;
}

_Bool iskeywordvartype(char *code) 
{ // cackes if word is position indecatoer keyword
	if(issame(code, "VARIABLE") || issame(code, "IMAGE") || issame(code, "FANCTION")  || issame(code, "LIST")) 
	return true;
  return false;
}

_Bool iskeywordvarprop(char *code) 
{ // cackes if word is position indecatoer keyword
	if(iskeywordvaluelocater(code) || issame(code, "VALUE") || issame(code, "NAME") || issame(code, "SHAPE")) 
	return true;
  return false;
}

_Bool iskeywordvarpos(char *code) 
{ // cackes if word is position indecatoer keyword
	if(issame(code, "GLOBAL")|| issame(code, "LOCAL") || issame(code, "STATE") ||issame(code, "TEMPRARE")) 
	return true;
  return false;
}
// --------------------------------------END VAR KEYWORDS ------------------------------------------

// -----------------------------------------VALUE KEYWORD -------------------------------------------
_Bool isvaluekeyword(char *code) 
{ // cackes VALUE of keyword
	if(iskeywordvaluetype(code) || iskeywordvalueboolen(code) || iskeywordvaluenumber(code) ||
     iskeywordvaluestring(code) || iskeywordvaluelong(code) || iskeywordvaluelocater(code) ||
     iskeywordvaluecolor(code)) 
	return true;
  return false;
}

_Bool iskeywordvaluetype(char *code) {
	if(issame(code, "BOOL") || issame(code, "NUMBER") || issame(code, "SENTANCE") || issame(code, "FRONT_COLOR") ||
    issame(code, "BACK_COLOR") || issame(code, "COLOR") || issame(code, "OPREATER") || 
		issame(code, "TEXT") || issame(code, "IMAGE") || issame(code, "OPREATER")
		)
	return true;
	return false;
}

_Bool iskeywordvalueopreat(char *code) {
	if(issame(code, "NOT") || issame(code, "THAN") || issame(code, "LESS") ||
		 issame(code, "EQUAL") || issame(code, "GRATER"))
	return true;
	return false;
}

_Bool iskeywordvalueboolen(char *code) {
	if(issame(code, "TRUE") || issame(code, "FALSE"))
	return true;
	return false;
}

_Bool iskeywordvaluenumber(char *code) {
	if(issame(code, "INTEGUR") || issame(code, "NATURAL") || issame(code, "RATIONAL") || issame(code, "WHOLE"))
	return true;
	return false;
}

_Bool iskeywordvaluelocater(char *code) {
	if(issame(code, "X") || issame(code, "Y") || issame(code, "WIDTH")  || issame(code, "HEIGHT"))
	return true;
	return false;
}

_Bool iskeywordvaluecolor(char *code) {
	if(issame(code, "BLACK") || issame(code, "WHITE"))
	return true;
	return false;
}

_Bool iskeywordvaluestring(char *code) {
	if(issame(code, "CHARACTER") || issame(code, "LETTER") || issame(code, "WORD"))
	return true;
	return false;
}

_Bool iskeywordvaluelong(char *code){
	if(issame(code, "SHORT") || issame(code, "LONG") || issame(code, "UNSIGNED") || issame(code, "SIGNED")) 
	return true;
	return false;
}

// --------------------------------------END VALUE KEYWORD ------------------------------------------