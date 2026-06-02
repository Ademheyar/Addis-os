#include <Addis/Events.h>
#include <Addis/Libs/List/List.h> // for defining list_t, listnode_t, and other list-related functions

#ifndef DEFT_H
#define DEFT_H

struct DEF_LIST {
	char *value, *non_def;
	char *name[20];
	int name_id;
	struct DEF_LIST *main_chaild_def;
	struct DEF_LIST *focused_chaild_def;
	struct DEF_LIST *last_chaild_def;

	struct DEF_LIST *next_def;
	struct DEF_LIST *parent;
	struct DEF_LIST *prev_def;
};

_Bool chack_def_names(struct DEF_LIST *deft, char *name);
void find_defc(char *name);
void find_defl(char *name);
_Bool isdefined(char *name);
_Bool get_defco(list_t *read, int isret);
struct ROW *marge_row(struct ROW *f, struct ROW *s);

#endif