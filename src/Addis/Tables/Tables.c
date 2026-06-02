#include <Addis/Tables/WorkingT.h>
#include <Addis/Libs/String/String.h>
#include <Addis/Read_Resivers.h>
#include <Addis/Read_Do.h>
#include <Addis/Process.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>

#include <Addis/Libs/String/Text.h>



Loged_user *loged_user[5];
SYSVARS sysvars[5];

struct READINGINFO *get_reading_table(char c)
{ // this will get where geted var will be saved and sand back rusoult
	struct READINGINFO *frtemp = task_list_current->holded_info->read;
	struct READINGINFO *fpread = task_list_current->holded_info->read->read_prev;
	int rfid = task_list_current->holded_info->read->fread->rfor_id;
	if(c == 0){
		if (issame(frtemp->fread->reading_for[rfid], "RETURN")){
			if (issame(fpread->fread->reading_for[fpread->fread->rfor_id-1], "GEVING")){
				while (!issame(fpread->fread->reading_for[fpread->fread->rfor_id], "GETTING")) fpread = fpread->read_next;
				frtemp = fpread;
			}
			else frtemp = task_list_current->holded_info->read->read_prev;
		}
		else if (issame(task_list_current->holded_info->read->fread->reading_for[rfid], "GEVING")){
			while (!issame(frtemp->fread->reading_for[frtemp->fread->rfor_id], "GETTING") && frtemp->read_next) {
				frtemp = frtemp->read_next;
			}
		}
		else if((int)task_list_current->holded_info->read->fread->reading_value->length  == task_list_current->holded_info->read->fread->reading_stoped){
			DEBUG("Getting reading for what and got %s ", task_list_current->holded_info->read->fread->reading_for[rfid]);
			if(!task_list_current->holded_info->read->read_next && !task_list_current->holded_info->read->read_prev) { DEBUG("no prev no next "); frtemp = NULL; }
			else if(task_list_current->holded_info->read->read_next) frtemp = task_list_current->holded_info->read->read_next;
			else if(task_list_current->holded_info->read->read_prev) frtemp = task_list_current->holded_info->read->read_prev;
			DEBUG("\n");
		}
	}
	else if (c == 'L'){
		while (frtemp->read_prev) frtemp = frtemp->read_prev;
	}
	return frtemp;
}

/*
*
*	UNVERSAL TABLES
*/

void Create_new_Unversal()
{
	//DEBUG("Create_new_Unversal\n");
	sysinfo.last_Unversal = sysinfo.first_Unversal = malloc(sizeof(struct USER_WDB));
	sysinfo.focused_Unversal = sysinfo.last_Unversal;
	sysinfo.last_Unversal->prev_table = NULL;
	sysinfo.last_Unversal->next_table = NULL;
	sysinfo.last_Unversal->last_chiled = sysinfo.last_Unversal->first_chiled = NULL;
}

void Create_last_Unversal()
{
	//DEBUG("Create_last_Unversal\n");
	if(sysinfo.last_Unversal)
	{
		sysinfo.last_Unversal->next_table = malloc(sizeof(struct USER_WDB));
		sysinfo.last_Unversal->next_table->prev_table = sysinfo.last_Unversal;
		sysinfo.last_Unversal->next_table->next_table = NULL;
		sysinfo.last_Unversal->next_table->first_chiled = NULL;
		sysinfo.last_Unversal->next_table->last_chiled = NULL;
		sysinfo.focused_Unversal = sysinfo.last_Unversal->next_table;
		sysinfo.last_Unversal = sysinfo.focused_Unversal;
	}
	else Create_new_Unversal();
}


void Delete_late_Unversal()
{
	//DEBUG("Delete_late_Unversal\n");
	if(sysinfo.last_Unversal)
	{
		if(sysinfo.last_Unversal->prev_table) 
		{
			sysinfo.focused_Unversal = sysinfo.last_Unversal->prev_table;
			free(sysinfo.focused_Unversal->next_table);
			sysinfo.focused_Unversal->next_table = NULL;
			sysinfo.last_Unversal = sysinfo.focused_Unversal;
		}
		else 
		{
			free(sysinfo.last_Unversal);
			sysinfo.last_Unversal = sysinfo.first_Unversal = NULL;
			sysinfo.focused_Unversal->next_table = NULL;
		}
	}
}
		

/*
*
*	GLOBAL TABLES
*/




/*
*
*	LOCAL TABLES
*/



/*
*
*	STATES TABLES
*/




/*
*
*	TEMPRARE TABLES
*/




/*
*
*	CHILED TABLES
*/

void create_newchiled(struct USER_WDB *parent)
{
	DEBUG("create_newchiled\n");
	parent->last_chiled = parent->first_chiled = malloc(sizeof(struct USER_WDB));
	parent->last_chiled->chiled_Parent = parent;
	parent->foucsed_chiled = parent->last_chiled;
	parent->last_chiled->prev_table = NULL;
	parent->last_chiled->next_table = NULL;
	parent->last_chiled->last_chiled = parent->last_chiled->first_chiled = NULL;
}

void create_lastchiled(struct USER_WDB *parent)
{
	if(parent->last_chiled)
	{
		DEBUG("parent->last_chiled\n");
		if(parent->first_chiled->name) DEBUG("parent->first_chiled name %s\n", parent->first_chiled->name);
		parent->last_chiled->next_table = malloc(sizeof(struct USER_WDB));
		parent->last_chiled->next_table->chiled_Parent = parent;
		parent->last_chiled->next_table->prev_table = parent->last_chiled;
		parent->last_chiled->next_table->next_table = NULL;
		parent->last_chiled->next_table->first_chiled = NULL;
		parent->last_chiled->next_table->last_chiled = NULL;
		parent->last_chiled = parent->last_chiled->next_table;
		parent->foucsed_chiled = parent->last_chiled;
	}
	else create_newchiled(parent);
}

void Add_chiled_to_lastparent(struct USER_WDB *parent, struct USER_WDB *chiled)
{
	DEBUG("Add_chiled_to_lastparent\n");
	if(parent->last_chiled)
	{
		DEBUG("parent->last_chiled\n");
		chiled->chiled_Parent = parent;
		parent->last_chiled->next_table = chiled;
		parent->last_chiled->next_table->prev_table = parent->last_chiled;
		parent->last_chiled->next_table->next_table = NULL;
		parent->last_chiled = parent->last_chiled->next_table;
		parent->foucsed_chiled = parent->last_chiled;
	}
	else {
		DEBUG("else\n");
		chiled->chiled_Parent = parent;
		parent->last_chiled = parent->first_chiled = chiled;
		parent->foucsed_chiled = parent->last_chiled;
		parent->last_chiled->prev_table = NULL;
		parent->last_chiled->next_table = NULL;
	}
}

void Add_chiled_to_parent(struct USER_WDB *parent, struct USER_WDB *chiled)
{
	DEBUG("Add_chiled_to_parent\n");
	if(chiled && parent){
		if (chiled->name) DEBUG("name (%s) && ", chiled->name);
		if (parent->name) DEBUG("name (%s) ", parent->name);
		DEBUG("chiled && parent\n");
		if(chiled->prev_table)
		{
			DEBUG("chiled->prev_table\n");
			if(chiled->next_table)
			{
				DEBUG("chiled->next_table\n");
				chiled->prev_table->next_table = chiled->next_table;
				chiled->next_table->prev_table = chiled->prev_table;
			}
			else 
			{
				chiled->prev_table->next_table = NULL;
				chiled->prev_table->chiled_Parent->last_chiled = chiled->prev_table;
			}
		}
		else if(chiled->next_table)
		{
			DEBUG("chiled->next_table\n");
			if(chiled->chiled_Parent)
			{
				DEBUG("chiled->chiled_Parent\n");
				chiled->chiled_Parent->first_chiled = chiled->next_table;
				if(!chiled->next_table->next_table) chiled->chiled_Parent->last_chiled = chiled->next_table;
			}
		}
		DEBUG("DONE GETTING\n");
		Add_chiled_to_lastparent(parent, chiled);
	}
	DEBUG("DONE ADDING CHILED\n");
	//while(1);
}


/*
*
*	commen
*/

void Create_working_place() 
{
	//DEBUG("Create_working_place\n");
	if(!sysinfo.USER_NAME || issame(sysinfo.USER_NAME, "")) {
		Create_last_Unversal();
		task_list_last->holded_info->read->fread = sysinfo.last_Unversal;
    task_list_last->holded_info->read->fread->state = 'G';
	}
	else {
		sysinfo.uwdb_id = loged_user[sysinfo.user_id]->uwdb_id++;
		// creating new processing code
		int ns = 0; 
		if (sizeof(struct USER_WDB) > 512) ns = 512;
		else if (sizeof(struct USER_WDB) > 1024) ns = 1024;
		task_list_last->holded_info->read->fread = loged_user[sysinfo.user_id]->user_wdb[sysinfo.uwdb_id] = malloc(sizeof(struct USER_WDB) + ns);
		task_list_last->holded_info->read->fread->chiled_id = -1;
		task_list_last->holded_info->read->fread->state = 'L';
	}
}

void Delete_lateworking_place() {
	int id;
	if(!sysinfo.USER_NAME || issame(sysinfo.USER_NAME, "")) {
		Delete_late_Unversal();
		task_list_last->holded_info->read->fread = sysinfo.last_Unversal;
	}
	else {
		id = loged_user[sysinfo.user_id]->uwdb_id--;
		free(loged_user[sysinfo.user_id]->user_wdb[id]);
		task_list_last->holded_info->read->fread = loged_user[sysinfo.user_id]->user_wdb[id];
	}
}

struct USER_WDB *Create_vartable(list_t *read, struct USER_WDB *var, int isret) 
{ // TO read command that will ask to create table vars or chiled 
	struct USER_WDB *ret = NULL;
	char *main_state = "", *state = "";
	char *as_name[10];
	int as_id = -1;
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
	for (int i = 0; ; i++)
	{
		if (issame(readword->value, "GLOBAL") || issame(readword->value, "STATE") || issame(readword->value, "LOCALE") || issame(readword->value, "TEMP")){
			if(i == 0) main_state = readword->value;
			state = readword->value;
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			if (issame(readword->value, "OR")) {
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				continue;
			}
		}
		
		if (issame(readword->value, "AS")){
			while(1)
			{
				if (issame(readword->value, "AS")) {
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				}

				struct ROW *ret = Resive(read, 0, 1);
				if(ret && ret->word){
					as_id++;
					as_name[as_id] = strdup(ret->word);
				}

				if (issame(readword->value, "AND")) {
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					continue;
				}
				break;
			}
			continue;
		}
		if (issame(state, "")) state = main_state;
		DEBUG("Going to Create %s in ", state);
		int frid = task_list_current->holded_info->read->fread->rfor_id;	
		struct USER_WDB *parent_var = NULL;
		if (issame(state, "LOCALE") ){
			DEBUG("LOCALE var table\n");
			struct READINGINFO *fLrtemp = get_reading_table('L');
			parent_var = fLrtemp->fread;
		}
		else if (issame(state, "GLOBAL")){
			DEBUG("GLOBAL Var\n");
			Create_last_Unversal();
			parent_var = sysinfo.last_Unversal;
		}
		else if (issame(state, "STATE") || issame(state, "") && issame(task_list_current->holded_info->read->fread->reading_for[frid], "GEVING"))
		{
			DEBUG("STATE var table\n");
			parent_var = task_list_current->holded_info->read->fread;
		}
		else if (var != NULL) 
		{
			DEBUG("Given Var\n");
			parent_var = var;
		}
		else 
		{
			DEBUG("TEMP var table\n");
			struct READINGINFO *frtemp = get_reading_table(0);
			parent_var = frtemp->fread;
		}
		
		if (parent_var != NULL){
			DEBUG("Var ");
			if (parent_var->name) DEBUG("%s ", parent_var->name);
			DEBUG("ITEM \n");
			create_lastchiled(parent_var);
			//parent_var->last_chiled->state = 'S';
			parent_var->last_chiled->name = "";
			parent_var->last_chiled->withcodes = "";
			parent_var->last_chiled->getas = "";
			parent_var->last_chiled->getwhat = "";
			parent_var->last_chiled->value.type = "";
			parent_var->last_chiled->value.word = "";
			parent_var->last_chiled->type = "";
			if (isret) ret = parent_var->last_chiled;
			
			if (issame(task_list_current->holded_info->read->fread->reading_for[frid], "GEVING")){
				struct ROW *row = malloc(sizeof(struct ROW));
				row->value.variable.fuwdb = parent_var->last_chiled;
				row->type = "VARIABLE";
				row->on = 6;
				Addrow_to_row_byindex(row, 0, 0);
			}

			if(as_id >= 0){
				char *c = task_list_current->holded_info->read->fread->reading_for[frid];
				task_list_current->holded_info->read->fread->reading_for[frid] = "";
				struct ROW *row = malloc(sizeof(struct ROW));
				row->value.variable.fuwdb = parent_var->last_chiled;
				row->type = "VARIABLE";
				row->As = as_name[as_id];
				row->on = 6;
				DEBUG("going to create as\n");
				Addrow_to_row_byindex(row, 1, 0);
				task_list_current->holded_info->read->fread->reading_for[frid] = c;
				as_id--;
			}

			DEBUG("saved var on %s\n", task_list_current->holded_info->read->fread->reading_for[frid]);
			if(as_id < 0) break;
			else continue;
		}
	/*
	new -
	new As a -
	new position As a -
	new As a position -
	new position As a  position -
	new As a and b -
	new position As a and b -
	new As a position and b position-
	new position As a position and b position-*/
	}
	return ret;
}

struct ROW *get_table(list_t *read, int isret)
{
	DEBUG("searching table 1\n");
  DEBUG("in get tabel");
  isret=isret;
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
  while(1){
  	DEBUG(" reading |%s|\n", readword->value);
		if (issame(readword->value, "NEW")){
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			DEBUG("G createing new table 1 [%d]%s \n", task_list_current->holded_info->read->fread->reading_on, readword->value);
			Create_vartable(read, task_list_current->holded_info->read->fread, 0); 
			return NULL;
		}
		else {
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		}
  }
  DEBUG(" got focsed vars\n");
  return NULL;
}






/*
*
*	WORKINGTABLES
*/


void list_allin_table()
{ // this will list all table items and there chileds
  char *nodes = "";
  void listin(struct USER_WDB *chack) {
		if(chack && !chack->first_chiled) return;
    struct USER_WDB *c = chack->first_chiled;
		char *oldnodes = strdup(nodes);
		while(1){
			if(c->name)
      DEBUG("%s>sub_name(%s)\n", nodes, c->name);
      nodes = stradd(nodes, "-", 0);
      listin(c);
      nodes = oldnodes;
			if(c->next_table) c = c->next_table;
			else break;
		}
  }

  if(sysinfo.user_id >= 0 && (sysinfo.USER_NAME || !issame(sysinfo.USER_NAME, ""))) {
    for (int j = 0; loged_user[sysinfo.user_id]->user_wdb[j]; j++) {
      if(loged_user[sysinfo.user_id]->user_wdb[j]->name)
      DEBUG("%schaecking Local_name(%s)\n", nodes, loged_user[sysinfo.user_id]->user_wdb[j]->name);
      nodes = "-";
      listin(loged_user[sysinfo.user_id]->user_wdb[j]);
      nodes = "";
    }
  }
  else {
		struct USER_WDB *c = sysinfo.first_Unversal;
		while(1){
			if(c->name)
      DEBUG("%schaecking global_name(%s)\n", nodes, c->name);
      nodes = "-";
      listin(c);
      nodes = "";
			if(c->next_table) c = c->next_table;
			else break;
		}
  }
}
struct USER_WDB *Get_list_by_names(struct USER_WDB *parent, char *name)
{
	DEBUG("Geting list by name\n");
	struct USER_WDB *list = parent;
	while(list)
	{
		DEBUG("list(%s) == name(%s)\n", list->name, name);
		if (list->name && issame(list->name, name)) return list;
		if(list->next_table) list = list->next_table;
		else break;
	}
	return NULL;
}

struct USER_WDB *Get_chiled_by_names(struct USER_WDB *list, char *name)
{
	DEBUG("Geting Chiled by name\n");
	struct USER_WDB *chiled = list->first_chiled;
	while(chiled)
	{
		DEBUG("Chiled(%s) == name(%s)\n", chiled->name, name);
		if (chiled->name && issame(chiled->name, name)) return chiled;
		if(chiled->next_table) chiled = chiled->next_table;
		else break;
	}
	return NULL;
}

struct USER_WDB *Get_main_dt(struct USER_WDB *g_main_dt, list_t *read)
{
 	// list all vars
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
  list_allin_table();
	char *main_name = "";
	int st = 0, rb = 1;
	if(g_main_dt) main_name = g_main_dt->name;
	else { st = 1; g_main_dt = sysinfo.first_Unversal; }
	main_name=main_name;
	for(;task_list_current->holded_info->read->fread->reading_on <= (int)read->length;)
	{
		DEBUG("find[%d](%s)dt\n", st, readword->value);
		if (rb){
			struct USER_WDB *glist = Get_chiled_by_names(g_main_dt, readword->value);
			if(glist)
			{
				DEBUG("found(%s)dt\n", readword->value);
				task_list_current->holded_info->read->fread->reading_on++;
				g_main_dt = glist;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				DEBUG("going to find(%s)dt\n", readword->value);
				st = 4;
				continue;
			}
			else rb = 0;
		}
		else
		{
			while(g_main_dt->prev_table) g_main_dt = g_main_dt->prev_table;
			struct USER_WDB *glist = Get_list_by_names(g_main_dt, readword->value);
			if(glist)
			{
				DEBUG("found(%s)dt\n", readword->value);
				task_list_current->holded_info->read->fread->reading_on++;
				g_main_dt = glist;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				DEBUG("going to find(%s)dt\n", readword->value);
				st = 4;
				continue;
			}
		}

		if (st == 0 || st == 1) {
			DEBUG("going back word\n");
			if (st != 1 && g_main_dt->reading_task && g_main_dt->reading_task->holded_info && g_main_dt->reading_task->holded_info->fread)
			{
				DEBUG("getting local table\n");
				if (g_main_dt->reading_task->holded_info->fread->name) DEBUG("name(%s)\n", g_main_dt->reading_task->holded_info->fread->name);
				g_main_dt = g_main_dt->reading_task->holded_info->fread;
				rb = 1;
				st = 1;
			}
			else if (g_main_dt->prev_table) 
			{
				DEBUG("given var not found going to prev table\n");
				g_main_dt = g_main_dt->prev_table;
			}
			else 
			{
				DEBUG("given var not found going to chack in global table\n");
				//else if (st == 1) // TODO: find on global on given user
				st = 2;
			}
		}

		else if (st == 2){
			if(sysinfo.first_Unversal) g_main_dt = sysinfo.first_Unversal;
			DEBUG("given var not found going to chack in global table\n");
			st = 3;
		}
		else break;
	}
	DEBUG("don gatting name id %d var \n", task_list_current->holded_info->read->fread->reading_on);
	if(st == 4) return g_main_dt;
	else return NULL;
}


struct USER_WDB *find_dt(struct USER_WDB *glist, char *USER_NAME, char *USER_PASSWORD, int id_db, char *Local_name, char *State_name, char *name, char *id_addrs)
{
 	// list all vars
  list_allin_table();

  struct USER_WDB *fuwdb;
  glist=glist;
  _Bool found=false;
  
  struct USER_WDB *search_allin(struct USER_WDB *chack) {
    DEBUG("search_allin .....................\n");
    for (int i = 0; chack->variables[i]; i++) {
      if (!(issame(State_name, " ") || issame(State_name, ""))) {
        if (issame(chack->variables[i]->name, State_name)) {
          // parent found chack name
          DEBUG("got State_name(%s) of dt %s", State_name, chack->variables[i]->name);
          //task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb = fuwdb = chack->variables[i];
          State_name="";
          if (!(issame(name, " ") || issame(name, "")) && !issame(chack->name, name))
            fuwdb = search_allin(chack->variables[i]); // if this teabl name is not same
          if (!found) fuwdb = chack->variables[i];
          else found=true;
          return fuwdb;
        }
      }
      if (!(issame(name, " ") || issame(name, ""))) {
        fuwdb = chack->variables[i];
        DEBUG("%s == name(%s)", name, chack->variables[i]->name);
        if (issame(chack->variables[i]->name, name)) {
          DEBUG("is same as %s\n", chack->variables[i]->name);
          found=true;
          return chack->variables[i];
        }
        else {
          DEBUG("is not same going to deep search\n");
          found=false;
          fuwdb = search_allin(chack->variables[i]); // if this teabl name is not same
          DEBUG(" out from deep search found(%d)\n", found);
        }
        if (found) { found=true; return fuwdb; }
        fuwdb = chack->variables[i];
      }
      else {
        DEBUG(" no name(%s) or State_name(%s) was given\n", name, State_name);
        break;
      }
    }
    DEBUG("out from search_allin .....................\n");
    return fuwdb;
  }
  

  DEBUG(" chack if it is given by addrs\n");
  // otherwaith will be choosing user must have name and password given or knowen
  // if user name is given get into it or use loged one
  if (USER_NAME && USER_PASSWORD) { 

  }
  else if (!(issame(id_addrs, " ") || issame(id_addrs, "")))
  {

  }
  
  // than will be choosing in which database 
  int iddb = 0;
  if(id_db != -1){
    iddb = id_db;
  }
  else if (!(issame(id_addrs, " ") || issame(id_addrs, "")))
  {

  }

  // chack Local_name in local db lists
  if (!(issame(Local_name, ""))) {}
  else if (sysinfo.Local_name && !(issame(sysinfo.Local_name, ""))) {
    Local_name = sysinfo.Local_name;
  }

  for(; iddb < 3;) {  
    if(iddb == 0){ // working data base lists
      DEBUG(" chack ");
      if(sysinfo.user_id >= 0 && (sysinfo.USER_NAME || !issame(sysinfo.USER_NAME, ""))) {
        for (int j = 0; loged_user[sysinfo.user_id]->user_wdb[j]; j++) {
          //DEBUG(" Local_name in local db lists\n");
          fuwdb = loged_user[sysinfo.user_id]->user_wdb[j];
          if (!(issame(Local_name, " "))) { // if main name is given
            DEBUG(" chaecking Local_name(%s) with id(%d) main_name(%s)\n", Local_name, j, loged_user[sysinfo.user_id]->user_wdb[j]->name);
            if (issame(fuwdb->name, Local_name)) { // if Local_name is found
              sysinfo.ftm_uwdb = fuwdb = loged_user[sysinfo.user_id]->user_wdb[j];
              fuwdb = search_allin(fuwdb);
              if (found) break;
              else fuwdb = loged_user[sysinfo.user_id]->user_wdb[j];
              break;
            }
            else continue; // if Local_name is not found
          }
          else { // if Local_name is not given or unknown will start on searching on working data base or local lists
            DEBUG("main name(%s)\n", fuwdb->name);
            if (issame(fuwdb->name, name)) {
              DEBUG("got main name(%s) of dt %s\n", name, fuwdb->name);
              found=true;
            }
            else
            {
              fuwdb = search_allin(fuwdb);
            }
            if (found) break;
          }
        }
      }
      else {
        for (int j = 0; j <= sysinfo.wdb_gid || sysinfo.wdb_G[j]; j++) {
          if(!sysinfo.wdb_G[j]) continue;
          /*fuwdb = sysinfo.wdb_G[j];
          if (!(issame(Local_name, ""))) { // if main name is given
            if (issame(fuwdb->name, Local_name)) { // if Local_name is found
              DEBUG("chaecking global_name[%d](%s) with ", j, sysinfo.wdb_G[j]->name);
              DEBUG("local name(%s)\n", Local_name);
              fuwdb = search_allin(sysinfo.wdb_G[j]);
              if (found) break;
              else fuwdb = sysinfo.wdb_G[j];
            }
          }

          if (!(issame(name, ""))) { // if main name is given
            if (issame(fuwdb->name, name)) {
              DEBUG("chaecking global_name[%d](%s) with ", j, sysinfo.wdb_G[j]->name);
              DEBUG("name(%s) ", name);
              DEBUG("issame\n");
              found=true;
            }
            if (found) break; 
          }*/
        }
      }      
    }
    else if(iddb == 1){ // interapters data base lists

    }
    else if(iddb == 2){ // events data base lists

    }
    else break;
    if(id_db != -1 || !(issame(id_addrs, " ") || issame(id_addrs, ""))) break;
    else iddb++;
  }
  DEBUG("out find var in db\n");
  if(fuwdb && found) return fuwdb;
  else return NULL;
}

struct ROW *get_vars(list_t *read, int isret)
{
  struct ROW *fuwdb_in_row[10];
  int fuwdb_inrow_id = -1;
  listnode_t *readword;
  while(1){
    fuwdb_inrow_id++;
    fuwdb_in_row[fuwdb_inrow_id] = NULL;
    readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		struct USER_WDB *fuwdb = Get_main_dt(task_list_current->holded_info->read->fread, read);
		//task_list_current->holded_info->read->fread->reading_stoped = task_list_current->holded_info->read->fread->reading_on;
		DEBUG("don gatting name id %d var \n", task_list_current->holded_info->read->fread->reading_on);	
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);

    if(fuwdb){
      task_list_current->holded_info->read->fread->reading_stoped = task_list_current->holded_info->read->fread->reading_on;
      readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("Going to save variable %d\n", fuwdb_inrow_id);
      fuwdb_in_row[fuwdb_inrow_id] = malloc(sizeof(struct ROW));
      fuwdb_in_row[fuwdb_inrow_id]->type = "VARIABLE";
      fuwdb_in_row[fuwdb_inrow_id]->on = 6;
      fuwdb_in_row[fuwdb_inrow_id]->value.variable.fuwdb = fuwdb;
      fuwdb_in_row[fuwdb_inrow_id]->value.variable.prop = NULL;
      
      char *getas = "";
      int back_fuwdb_inrow_id = fuwdb_inrow_id;
      while(issame(readword->value, "AS")) 
      {
        DEBUG("getting variable AS\n");
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
        struct ROW *row = Resive(read, 1, 1);
        if(row)
        {
          DEBUG("found variable AS\n");
          getas = strdup(row->word);
          free(row);
        }

        if(!issame(getas, "")) {
          for(; back_fuwdb_inrow_id >= 0 && fuwdb_in_row[back_fuwdb_inrow_id];)
          {
            if(!fuwdb_in_row[fuwdb_inrow_id]->As || fuwdb_in_row[fuwdb_inrow_id]->As && !issame(fuwdb_in_row[fuwdb_inrow_id]->As, "")) 
            fuwdb_in_row[fuwdb_inrow_id]->As = strdup(getas);
            DEBUG("going to give for %d vars as %s == %s\n", back_fuwdb_inrow_id, getas, fuwdb_in_row[fuwdb_inrow_id]->As);
            back_fuwdb_inrow_id--;
            if (issame(readword->value, "AND")) break;
          }
          if (issame(readword->value, "AND")) 
          {
            task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
            readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
            continue;
          }
          else break;
        }
        else break; 
      }

      // get vars prop 
      
      char *prop = "";
      DEBUG("is vars prop %s\n", readword->value);
      while(iskeywordvaluetype(readword->value) || iskeywordvarprop(readword->value)) {
				prop = strdup(readword->value);
        // TODO : GET MORE PROP IN ONE VARS
				//if (issame(prop, "")) 
        //else prop = stradd(prop, readword->value, ' ');
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
        if (issame(readword->value, "AND")) 
        {
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          continue;
        }
      }

      // giving prop value to find vars
      if(!issame(prop, "")) {
        DEBUG("going to give vars prop %s\n", prop);
        for(int i = 0; i <= fuwdb_inrow_id && fuwdb_in_row[i]; i++){
          if(!fuwdb_in_row[fuwdb_inrow_id]->value.variable.prop) 
          fuwdb_in_row[fuwdb_inrow_id]->value.variable.prop = strdup(prop);
        }
      }
      
      // get other vars
      if (issame(readword->value, "AND")) 
      {
        DEBUG("will get anther variable\n");
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        fuwdb_inrow_id++;
        continue;
      }

      if(fuwdb_in_row[fuwdb_inrow_id]->value.variable.fuwdb->name) DEBUG("var name %s\n", fuwdb_in_row[fuwdb_inrow_id]->value.variable.fuwdb->name);
      DEBUG("found var will be save 0 or returned 1 = %d\n", isret);
      if(isret == 0) Addrow_to_row_byindex(fuwdb_in_row[fuwdb_inrow_id], 0, 0);
    } 
    break;
  }
  DEBUG("got %d vars\n", fuwdb_inrow_id);
  return fuwdb_in_row[fuwdb_inrow_id];
}





































/*
*
*	WORKINGTABLES
*/

void LogIn() {
	// TODO: cheack user name and password befor logining
	if(sysinfo.user_id < 5) sysinfo.user_id++;
	else {
		// TODO: erro message way
		// if there is new way to display msg
		// print old user will be gone
		sysinfo.user_id = 0;
	}
	int ns = 0; 
	if (sizeof(Loged_user) > 512) ns = 512;
	else if (sizeof(Loged_user) > 1024) ns = 1024;
	loged_user[sysinfo.user_id] = (Loged_user *)malloc(sizeof(Loged_user) + ns);
}

/*
*	
*/

void Remove_READING(){
	struct READINGINFO *frtemp = get_reading_table(0);
	if(!frtemp){
		DEBUG(" done deleting \n");
		Current_task_delete();
	}
	else {
		DEBUG("done reading one task\n");
		task_list_current->holded_info->read = frtemp;
	}
}

void Remove_ALLF_READING() 
{ // this will reamove all focused reading table
	struct READINGINFO *r = task_list_current->holded_info;
	if(task_list_current->holded_info->read_next){
		for(;r->read_next; ) r = r->read_next; // get last reading table
		for(;r->read_prev; ) {
			r->read_next = NULL;
			r = r->read_prev;
			if(r->read_next) free(r->read_next);
			if(r->bootinfo && r->booton) free(r->bootinfo);
		}
	}
	if(r->bootinfo && r->booton) free(r->bootinfo);
	if(r->read_next) free(r->read_next);
	r->read_next = NULL;
}

// this will create new table b\n this table and next table
void Create_readtable_bn() {
	int ns = 0; 
	if (sizeof(struct USER_WDB) > 512) ns = 512;
	else if (sizeof(struct USER_WDB) > 1024) ns = 1024;
	struct READINGINFO *new = malloc(sizeof(struct READINGINFO));
	new->fread = malloc(sizeof(struct USER_WDB)+ns); // create new table
	new->read_prev = task_list_current->holded_info->read;
	new->fread->reading_task = task_list_current;
	if(task_list_current->holded_info->read->read_next) {
		task_list_current->holded_info->read->read_next->read_prev = new;
		new->read_next = task_list_current->holded_info->read->read_next;
	}
	else new->read_next = NULL;
	task_list_current->holded_info->read->read_next = new;
	DEBUG("doen createing new table b\\w\n");
}

// this will remove corrent reading table and rettern valubel values
void Remove_this_readtable() {
	Return_lastworktable();
	DEBUG(" going to remove this tabe\n");
	// first retearn all needed values
}

