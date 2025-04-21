#include <Addis/Readcode.h>



_Bool fix_varprop(struct USER_WDB *var, char *dowhat, struct ROW *value);

_Bool get_locale(char *name, char *with);
void marge_vars(int num1, int num2, char *text1, char *text2, char *dowhat);
void workon_unkvar(char *dowhat);
void read_fanctions(list_t *read);
void compar_value(char *compar);
void oprate_value();
char *get_witbmp_h(char *with0, char *with1);
