#include <Addis/Tables/WorkingT.h>
#include <Addis/Libs/String/String.h>
#include <Addis/Read_Resivers.h>
#include <Addis/Read_Do.h>
#include <Addis/Readcode.h>
#include <Addis/Process.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>

#include <Addis/Libs/String/Text.h>

Loged_user *loged_user[5];
SYSVARS sysvars[5];

/*
*
*
*
*			for systeminfo functions will be fount here
*
*
*
*/

/*
*
*	UNVERSAL TABLES
*
*	GLOBAL TABLES
*/


void Create_new_Unversal()
{
	//DEBUG("Create_new_Unversal\n");
	sysinfo.last_Unversal = sysinfo.first_Unversal = malloc(sizeof(struct READINGINFO));
	sysinfo.focused_Unversal = sysinfo.last_Unversal;
	sysinfo.last_Unversal->read_prev = NULL;
	sysinfo.last_Unversal->read_next = NULL;
	sysinfo.last_Unversal->last_chiled = sysinfo.last_Unversal->first_chiled = NULL;
}

void Create_last_Unversal()
{
	//DEBUG("Create_last_Unversal\n");
	if(sysinfo.last_Unversal)
	{
		sysinfo.last_Unversal->read_next = malloc(sizeof(struct READINGINFO));
		sysinfo.last_Unversal->read_next->read_prev = sysinfo.last_Unversal;
		sysinfo.last_Unversal->read_next->read_next = NULL;
		sysinfo.last_Unversal->read_next->first_chiled = NULL;
		sysinfo.last_Unversal->read_next->last_chiled = NULL;
		sysinfo.focused_Unversal = sysinfo.last_Unversal->read_next;
		sysinfo.last_Unversal = sysinfo.focused_Unversal;
	}
	else Create_new_Unversal();
}


void Delete_late_Unversal()
{
	//DEBUG("Delete_late_Unversal\n");
	if(sysinfo.last_Unversal)
	{
		if(sysinfo.last_Unversal->read_prev) 
		{
			sysinfo.focused_Unversal = sysinfo.last_Unversal->read_prev;
			free(sysinfo.focused_Unversal->read_next);
			sysinfo.focused_Unversal->read_next = NULL;
			sysinfo.last_Unversal = sysinfo.focused_Unversal;
		}
		else 
		{
			free(sysinfo.last_Unversal);
			sysinfo.last_Unversal = sysinfo.first_Unversal = NULL;
			sysinfo.focused_Unversal->read_next = NULL;
		}
	}
}


void Create_working_place(char *newcode, char on) 
{
	DEBUG("Create_working_place\n");
	int ns = 0; 
	if (sizeof(struct READINGINFO) > 512) ns = 512;
	else if (sizeof(struct READINGINFO) > 1024) ns = 1024;
	struct READINGINFO *newread = malloc(sizeof(struct READINGINFO) + ns);
	newread->read_next = NULL;
	newread->chiled_id = -1;

	            
	DEBUG("Going to read Bodys (%s)\n", newcode);
	// Read the body of the code
	char *rbcode = read_bodys(newcode);
	DEBUG("Done reding body\n");
	// If the code is valid, process it further
	if(rbcode && (!issame(rbcode, "") || !issame(rbcode, " ")))
	{
		DEBUG("Going to fixe Code(%s)....\n", rbcode);
		// Update the task with the retrieved code
		newread->reading_task = task_list_current;
		
		// Update the reading task with the processed code
		newread->main_readcode = stradd(rbcode," ", 0);
		newread->reading_value = str_splitL(stradd(rbcode," ", 0), " ", 0);
		newread->read_new = 0;
		newread->reading_on = 0;
		newread->reading_stoped = 0;
		DEBUG("Code Found\n");
		//newread->name = strdup(task_list_current->holded_info->bootinfo->code_splited1[i-1]);
		newread->getas = "";
		newread->getwhat = "";
		DEBUG("Setting code\n");
		newread->rfor_id = 0;
		newread->reading_for[0] = "READING";

		// Log debug information about the completed code fixing
		DEBUG(" done Pripering Working Table r %d %d  l %d v %s\n", newread->reading_on, newread->reading_stoped, (int)newread->reading_value->length, rbcode);
		DEBUG(" done creating(");
		if(task_list_current->state) DEBUG("Task-id[%d] ", task_list_current->id);
		if(task_list_current->state) DEBUG(" state[%d]) \n", task_list_current->state);
		DEBUG("And fixing code %s\n", newcode);
	}
	// If no valid code is found, mark the reading as stopped
	else {
		newread->reading_stoped = (int)newread->reading_value->length;
		// Log debug information about the absence of code
		DEBUG("There is no code to fix\n");
		DEBUG(" can't creating and fixing code else %s\n", newcode);
		// Remove the reading task
		Delete_lateworking_place();
		DEBUG(" done deleting \n");
	}

	task_t *pointtask = NULL;
	if(on == 'C') pointtask = task_list_current;
	else if (on == 'L') pointtask = task_list_last;
	else if (on == 'N') pointtask = task_list_new;

	if (pointtask){
		if(pointtask->Last_read){
			pointtask->Last_read->read_next = newread;
			newread->read_prev = pointtask->Last_read;
			newread->id = pointtask->Last_read->id+1;
		}
		else{
			newread->id = 0;
			newread->read_prev = NULL;
			pointtask->Mian_read = newread;
			pointtask->Last_read = newread;
			pointtask->holded_info = newread;
		}
		pointtask->Last_read = newread;	
		if(!sysinfo.USER_NAME || issame(sysinfo.USER_NAME, "")) pointtask->Last_read->state = 'G';
		else pointtask->Last_read->state = 'L';
	}	
	DEBUG("Aolmost Done newread->id %d \n", newread->id);

	DEBUG(" done Createing Working place\n");
	
	/* TODO : CREARTE WORING PLACE ON WHICH TASK C:CORRONT L: LAST N: NEW
	
	
	*/
}

/*
*	removes the last reading envairomant
*/

void Delete_lateworking_place(){
	DEBUG("deleting latest working place \n");
	if(task_list_current->Last_read && task_list_current->Last_read->read_prev){
		task_list_current->holded_info = task_list_current->Last_read->read_prev;
		if(task_list_current->holded_info) {
			task_list_current->holded_info->read_next = NULL;
			//Current_task_delete();
			free(task_list_current->Last_read);
			// set the last read
			task_list_current->Last_read = task_list_current->holded_info;
		}
		DEBUG("done setting reading one task\n");
	}
	else DEBUG("can't find reading\n");
}



/*
*
*
*
*			for systeminfo functions will be fount here
*
*
*
*/
/*
*
*	UNVERSAL Varible
*
*	GLOBAL Varible
*/

void Create_new_extern_varible()
{   // this function will create varible on external or new 

	DEBUG("Create_new_extern_varible Unversal, Global or external Varible \n");
	// create new space
	sysinfo.focused_extern_varible = malloc(sizeof(struct ROW)); 
	// there is no next value
	sysinfo.focused_extern_varible->next_row = NULL;
	// if there no var first or prev created befor
	if(!sysinfo.first_extern_varible) sysinfo.first_extern_varible = sysinfo.focused_extern_varible;
	
	// but if there is prevuse it will set it
	if(sysinfo.last_extern_varible) sysinfo.last_extern_varible->prev_row = sysinfo.last_extern_varible;
	else sysinfo.last_extern_varible->prev_row = NULL;

	// will set it at the last for next time to find it
	sysinfo.last_extern_varible = sysinfo.focused_extern_varible;
	
	// no need to return created item it can be found by global sysinfo stracter
}

// if it is set extern varible eny where canbe called and used 
// 
struct ROW *Set_varible_extern(struct ROW *var)
{ // this will get and set or pint given varible to extern 

	DEBUG("Setting_new_extern_varible Unversal, Global or external Varible \n");
	// Settig given item to extern value
	struct ROW *new = malloc(sizeof(struct ROW));
	new->As = var->As;
	new->type = var->type;
	if(new->value.Buffer) new->value.Buffer = var->value.Buffer;
	if(new->value.charcter) new->value.charcter = var->value.charcter;

	// NOte: if var parent in local is distroi is it going to distroy this function or not ?
	sysinfo.focused_extern_varible = new; 
	
	// there is no next value
	sysinfo.focused_extern_varible->next_row = NULL;
	// if there no var first or prev created befor
	if(!sysinfo.first_extern_varible) sysinfo.first_extern_varible = sysinfo.focused_extern_varible;
	// but if there is prevuse it will set it
	if(sysinfo.last_extern_varible) sysinfo.focused_extern_varible->prev_row = sysinfo.last_extern_varible;
	else sysinfo.focused_extern_varible->prev_row = NULL;
	// will set it at the last for next time to find it
	sysinfo.last_extern_varible = sysinfo.focused_extern_varible;
	// we can return it
	var->As=NULL;
	var->type=NULL;
	free(var);
	var=NULL;
	return sysinfo.focused_extern_varible;
}

// Get Varible form local go up to globle
// 
// for finding variable by name 
// arg* = name 
struct ROW *Find_Variable_by(char *name)
{
	struct ROW *retrow = NULL;
	struct READINGINFO *rdinfo = task_list_current->holded_info;
	DEBUG("going to find varible on local %s\n", name);
	while (rdinfo)
	{
		DEBUG("rearing plcae id %d\n", rdinfo->id);
		struct COLUMN *column = rdinfo->worktables.worktable->column;
		int columnid = 0;
		while (column)
		{
			DEBUG("on column %d\n", columnid);
			if(column && column->row){
				struct ROW *row = column->row;
				int rowid = 0;
				while (row)
				{
					DEBUG("on row %d\n", rowid);
					if(name && row->As && issame(row->As, name)) {
						retrow = row;
						task_list_current->holded_info->worktables.worktable->focused_column->focused_row = row;
						return retrow;
					}
					// we can even conntinue chacking by vaule, vaible type, .... 
					row = row->next_row; // got to next row 
					rowid += 1;
					DEBUG("next on row %d\n", rowid);
				}
				DEBUG("out on row %d\n", rowid);
			}
			//DEBUG("out column %d\n", columnid);
			column = column->next_column; // got to next columen 
			if(!column || (column && !column->row)) break;
			//DEBUG("out column %d\n", columnid);
			columnid += 1;
		}
		//DEBUG("out red column %d\n", columnid);
		rdinfo = rdinfo->read_prev; // goback until master code varibles
	}
	
	
	if(retrow == NULL && sysinfo.first_extern_varible){
		DEBUG("going to find varible on global %s\n", name);
		int countgv = 0;
		struct ROW *globalrow = sysinfo.first_extern_varible;
		while(globalrow){
		
			DEBUG("rearing plcae id %d ", countgv);
			if(globalrow->As)  DEBUG("AS %s ", globalrow->As);
			if(name && issame(globalrow->As, name)) {
				retrow = globalrow;
				task_list_current->holded_info->worktables.worktable->focused_column->focused_row = globalrow;
				return retrow;
			}
			DEBUG("\n");
			// we can even conntinue chacking by vaule, vaible type, .... 
			globalrow = globalrow->next_row; // chacke next varible
			countgv+=1;
		}
	}
	return retrow;
}



















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
*	CHILED TABLES
*/

void create_newchiled(struct READINGINFO *parent)
{
	DEBUG("create_newchiled\n");
	parent->last_chiled = parent->first_chiled = malloc(sizeof(struct READINGINFO));
	parent->last_chiled->chiled_Parent = parent;
	parent->foucsed_chiled = parent->last_chiled;
	parent->last_chiled->read_prev = NULL;
	parent->last_chiled->read_next = NULL;
	parent->last_chiled->last_chiled = parent->last_chiled->first_chiled = NULL;
}

void create_lastchiled(struct READINGINFO *parent)
{
	if(parent->last_chiled)
	{
		DEBUG("parent->last_chiled\n");
		if(parent->first_chiled->name) DEBUG("parent->first_chiled name %s\n", parent->first_chiled->name);
		parent->last_chiled->read_next = malloc(sizeof(struct READINGINFO));
		parent->last_chiled->read_next->chiled_Parent = parent;
		parent->last_chiled->read_next->read_prev = parent->last_chiled;
		parent->last_chiled->read_next->read_next = NULL;
		parent->last_chiled->read_next->first_chiled = NULL;
		parent->last_chiled->read_next->last_chiled = NULL;
		parent->last_chiled = parent->last_chiled->read_next;
		parent->foucsed_chiled = parent->last_chiled;
	}
	else create_newchiled(parent);
}

void Add_chiled_to_lastparent(struct READINGINFO *parent, struct READINGINFO *chiled)
{
	DEBUG("Add_chiled_to_lastparent\n");
	if(parent->last_chiled)
	{
		DEBUG("parent->last_chiled\n");
		chiled->chiled_Parent = parent;
		parent->last_chiled->read_next = chiled;
		parent->last_chiled->read_next->read_prev = parent->last_chiled;
		parent->last_chiled->read_next->read_next = NULL;
		parent->last_chiled = parent->last_chiled->read_next;
		parent->foucsed_chiled = parent->last_chiled;
	}
	else {
		DEBUG("else\n");
		chiled->chiled_Parent = parent;
		parent->last_chiled = parent->first_chiled = chiled;
		parent->foucsed_chiled = parent->last_chiled;
		parent->last_chiled->read_prev = NULL;
		parent->last_chiled->read_next = NULL;
	}
}

void Add_chiled_to_parent(struct READINGINFO *parent, struct READINGINFO *chiled)
{
	DEBUG("Add_chiled_to_parent\n");
	if(chiled && parent){
		if (chiled->name) DEBUG("name (%s) && ", chiled->name);
		if (parent->name) DEBUG("name (%s) ", parent->name);
		DEBUG("chiled && parent\n");
		if(chiled->read_prev)
		{
			DEBUG("chiled->read_prev\n");
			if(chiled->read_next)
			{
				DEBUG("chiled->read_next\n");
				chiled->read_prev->read_next = chiled->read_next;
				chiled->read_next->read_prev = chiled->read_prev;
			}
			else 
			{
				chiled->read_prev->read_next = NULL;
				chiled->read_prev->chiled_Parent->last_chiled = chiled->read_prev;
			}
		}
		else if(chiled->read_next)
		{
			DEBUG("chiled->read_next\n");
			if(chiled->chiled_Parent)
			{
				DEBUG("chiled->chiled_Parent\n");
				chiled->chiled_Parent->first_chiled = chiled->read_next;
				if(!chiled->read_next->read_next) chiled->chiled_Parent->last_chiled = chiled->read_next;
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

struct READINGINFO *Create_vartable(list_t *read, struct READINGINFO *var, int isret) 
{ // TO read command that will ask to create table vars or chiled 
	struct READINGINFO *ret = NULL;
	char *main_state = "", *state = "";
	char *as_name[10];
	int as_id = -1;
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on); 
	for (int i = 0; ; i++)
	{
		if (issame(readword->value, "GLOBAL") || issame(readword->value, "STATE") || issame(readword->value, "LOCALE") || issame(readword->value, "TEMP")){
			if(i == 0) main_state = readword->value;
			state = readword->value;
			task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
			if (issame(readword->value, "OR")) {
				task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
				continue;
			}
		}
		
		if (issame(readword->value, "AS")){
			while(1)
			{
				if (issame(readword->value, "AS")) {
					task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
				}

				struct ROW *ret = Resive(read, 0, 1);
				if(ret && ret->word){
					as_id++;
					as_name[as_id] = strdup(ret->word);
				}

				if (issame(readword->value, "AND")) {
					task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
					continue;
				}
				break;
			}
			continue;
		}
		if (issame(state, "")) state = main_state;
		DEBUG("Going to Create %s in ", state);
		int frid = task_list_current->holded_info->rfor_id;	
		struct READINGINFO *parent_var = NULL;
		if (issame(state, "LOCALE") ){
			DEBUG("LOCALE var table\n");
			parent_var = task_list_current->holded_info;
		}
		else if (issame(state, "GLOBAL")){
			DEBUG("GLOBAL Var\n");
			//Create_last_Unversal();
			parent_var = sysinfo.last_Unversal;
		}
		else if (issame(state, "STATE") || issame(state, "") && issame(task_list_current->holded_info->reading_for[frid], "GEVING"))
		{
			DEBUG("STATE var table\n");
			parent_var = task_list_current->holded_info;
		}
		else if (var != NULL) 
		{
			DEBUG("Given Var\n");
			parent_var = var;
		}
		else 
		{
			DEBUG("TEMP var table\n");
			parent_var = task_list_current->holded_info;
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
			
			if (issame(task_list_current->holded_info->reading_for[frid], "GEVING")){
				struct ROW *row = malloc(sizeof(struct ROW));
				row->value.variable.fuwdb = parent_var->last_chiled;
				row->type = "VARIABLE";
				row->on = 6;
				Addrow_to_row_byindex(row, 0, 0);
			}

			if(as_id >= 0){
				char *c = task_list_current->holded_info->reading_for[frid];
				task_list_current->holded_info->reading_for[frid] = "";
				struct ROW *row = malloc(sizeof(struct ROW));
				row->value.variable.fuwdb = parent_var->last_chiled;
				row->type = "VARIABLE";
				row->As = as_name[as_id];
				row->on = 6;
				DEBUG("going to create as\n");
				Addrow_to_row_byindex(row, 1, 0);
				task_list_current->holded_info->reading_for[frid] = c;
				as_id--;
			}

			DEBUG("saved var on %s\n", task_list_current->holded_info->reading_for[frid]);
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
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on); 
  while(1){
  	DEBUG(" reading |%s|\n", readword->value);
		if (issame(readword->value, "NEW")){
			task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
			DEBUG("G createing new table 1 [%d]%s \n", task_list_current->holded_info->reading_on, readword->value);
			Create_vartable(read, task_list_current->holded_info, 0); 
			return NULL;
		}
		else {
			task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
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
  void listin(struct READINGINFO *chack) 
  {
		if(chack && !chack->first_chiled) return;
    	struct READINGINFO *c = chack->first_chiled;
		char *oldnodes = strdup(nodes);
		while(1){
			if(c->name)
      DEBUG("%s>sub_name(%s)\n", nodes, c->name);
      nodes = stradd(nodes, "-", 0);
      listin(c);
      nodes = oldnodes;
			if(c->read_next) c = c->read_next;
			else break;
		}
  }

  if(sysinfo.user_id >= 0 && (sysinfo.USER_NAME || !issame(sysinfo.USER_NAME, ""))) {
    for (int j = 0; loged_user[sysinfo.user_id]->READINGINFO[j]; j++) {
      if(loged_user[sysinfo.user_id]->READINGINFO[j]->name)
      DEBUG("%schaecking Local_name(%s)\n", nodes, loged_user[sysinfo.user_id]->READINGINFO[j]->name);
      nodes = "-";
      listin(loged_user[sysinfo.user_id]->READINGINFO[j]);
      nodes = "";
    }
  }
  else {
		struct READINGINFO *c = sysinfo.first_Unversal;
		while(1){
			//if(c && c->name) DEBUG("%s chaecking global_name(%s)\n", nodes, c->name);
			nodes = "-";
			listin(c);
			nodes = "";
			if(c->read_next) c = c->read_next;
			else break;
		}
  }
}
struct READINGINFO *Get_list_by_names(struct READINGINFO *parent, char *name)
{
	DEBUG("Geting list by name (%s)\n", name);
	struct READINGINFO *list = parent;
	while(list)
	{
		//DEBUG("list(%s) == name(%s)\n", list->name, name);
		if (list->name && issame(list->name, name)) return list;
		if(list->read_next) list = list->read_next;
		else break;
	}
	return NULL;
}

struct READINGINFO *Get_chiled_by_names(struct READINGINFO *list, char *name)
{
	DEBUG("Geting Chiled by name\n");
	struct READINGINFO *chiled = list->first_chiled;
	while(chiled)
	{
		DEBUG("Chiled(%s) == name(%s)\n", chiled->name, name);
		if (chiled->name && issame(chiled->name, name)) return chiled;
		if(chiled->read_next) chiled = chiled->read_next;
		else break;
	}
	return NULL;
}

struct READINGINFO *Get_main_dt(struct READINGINFO *g_main_dt, list_t *read)
{
 	// list all vars
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on); 
  list_allin_table();
	char *main_name = "";
	int st = 0, rb = 1;
	if(g_main_dt) main_name = g_main_dt->name;
	else { st = 1; g_main_dt = sysinfo.first_Unversal; }
	main_name=main_name;
	for(;task_list_current->holded_info->reading_on <= (int)read->length;)
	{
		DEBUG("find[%d](%s)dt\n", st, readword->value);
		if (rb){
			struct READINGINFO *glist = Get_chiled_by_names(g_main_dt, readword->value);
			if(glist)
			{
				DEBUG("found(%s)dt\n", readword->value);
				task_list_current->holded_info->reading_on++;
				g_main_dt = glist;
				readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
				DEBUG("going to find(%s)dt\n", readword->value);
				st = 4;
				continue;
			}
			else rb = 0;
		}
		else
		{
			while(g_main_dt->read_prev) g_main_dt = g_main_dt->read_prev;
			struct READINGINFO *glist = Get_list_by_names(g_main_dt, readword->value);
			if(glist)
			{
				DEBUG("found(%s)dt\n", readword->value);
				task_list_current->holded_info->reading_on++;
				g_main_dt = glist;
				readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
				DEBUG("going to find(%s)dt\n", readword->value);
				st = 4;
				continue;
			}
		}

		if (st == 0 || st == 1) {
			DEBUG("going back word\n");
			if (st != 1 && g_main_dt->reading_task && g_main_dt->reading_task->holded_info && g_main_dt->reading_task->holded_info)
			{
				DEBUG("getting local table\n");
				//if (g_main_dt->reading_task->holded_info->name) DEBUG("name(%s)\n", g_main_dt->reading_task->holded_info->name);
				g_main_dt = g_main_dt->reading_task->holded_info;
				rb = 1;
				st = 1;
			}
			else if (g_main_dt->read_prev) 
			{
				DEBUG("given var not found going to prev table\n");
				g_main_dt = g_main_dt->read_prev;
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
	DEBUG("don gatting name id %d var \n", task_list_current->holded_info->reading_on);
	if(st == 4) return g_main_dt;
	else return NULL;
}


struct READINGINFO *find_dt(struct READINGINFO *glist, char *USER_NAME, char *USER_PASSWORD, int id_db, char *Local_name, char *State_name, char *name, char *id_addrs)
{
 	// list all vars
  list_allin_table();

  struct READINGINFO *fuwdb;
  glist=glist;
  _Bool found=false;
  
  struct READINGINFO *search_allin(struct READINGINFO *chack) {
    DEBUG("search_allin .....................\n");
    for (int i = 0; chack->variables[i]; i++) {
      if (!(issame(State_name, " ") || issame(State_name, ""))) {
        if (issame(chack->variables[i]->name, State_name)) {
          // parent found chack name
          DEBUG("got State_name(%s) of dt %s", State_name, chack->variables[i]->name);
          //task_list_current->holded_info->worktables.worktable->focused_column->row->value.variable.fuwdb = fuwdb = chack->variables[i];
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
        for (int j = 0; loged_user[sysinfo.user_id]->READINGINFO[j]; j++) {
          //DEBUG(" Local_name in local db lists\n");
          fuwdb = loged_user[sysinfo.user_id]->READINGINFO[j];
          if (!(issame(Local_name, " "))) { // if main name is given
            DEBUG(" chaecking Local_name(%s) with id(%d) main_name(%s)\n", Local_name, j, loged_user[sysinfo.user_id]->READINGINFO[j]->name);
            if (issame(fuwdb->name, Local_name)) { // if Local_name is found
              sysinfo.ftm_uwdb = fuwdb = loged_user[sysinfo.user_id]->READINGINFO[j];
              fuwdb = search_allin(fuwdb);
              if (found) break;
              else fuwdb = loged_user[sysinfo.user_id]->READINGINFO[j];
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
        for (int j = 0; j <= sysinfo.wdb_gid; j++) { // || sysinfo.wdb_G[j]
          //if(!sysinfo.wdb_G[j]) continue;
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
    readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
		struct READINGINFO *fuwdb = Get_main_dt(task_list_current->holded_info, read);
		//task_list_current->holded_info->reading_stoped = task_list_current->holded_info->reading_on;
		DEBUG("don gatting name id %d var \n", task_list_current->holded_info->reading_on);	
		readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);

    if(fuwdb){
      task_list_current->holded_info->reading_stoped = task_list_current->holded_info->reading_on;
      readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
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
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
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
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
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
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
        if (issame(readword->value, "AND")) 
        {
          task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
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
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
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
	if (sizeof(struct READINGINFO) > 512) ns = 512;
	else if (sizeof(struct READINGINFO) > 1024) ns = 1024;
	struct READINGINFO *new = malloc(sizeof(struct READINGINFO));
	new = malloc(sizeof(struct READINGINFO)+ns); // create new table
	new->read_prev = task_list_current->holded_info;
	new->reading_task = task_list_current;
	if(task_list_current->holded_info->read_next) {
		task_list_current->holded_info->read_next->read_prev = new;
		new->read_next = task_list_current->holded_info->read_next;
	}
	else new->read_next = NULL;
	task_list_current->holded_info->read_next = new;
	DEBUG("doen createing new table b\\w\n");
}

// this will remove corrent reading table and rettern valubel values
void Remove_this_readtable() {
	Return_lastworktable();
	DEBUG(" going to remove this tabe\n");
	// first retearn all needed values
}








// To find Full working table Column and Row by given name
struct WORKTABLE *Find_worktable_by_name(const char *name) {
	struct WORKTABLE *wt = task_list_current->holded_info->worktables.worktable;
	for(int w = 0; wt; w++){
		if(wt->column) {
			struct COLUMN *col = wt->column;
			for(int c = 0; col; c++){
				if(col->row) {
					struct ROW *row = col->row;
					for(int r = 0; row; r++){
						if(row->As && strcmp(row->As, name) == 0) return wt;
						row = row->next_row;
					}
				}
				else if(col->next_column) col = col->next_column;
				else break;
			}
		}
		else if(wt->next_wt) wt = wt->next_wt;
		else break;
	}
	return wt;
}

// To find Full working table Column and Row by given Indexs
struct WORKTABLE *Find_worktable_by_indexs(int table_id, int column_id, int row_id) {
	struct WORKTABLE *wt = task_list_current->holded_info->worktables.worktable;
	for(int w = 0; wt; w++){
		if(w == table_id) {
			struct COLUMN *col = wt->column;
			for(int c = 0; col; c++){
				if(c == column_id) {
					struct ROW *row = col->row;
					for(int r = 0; row; r++){
						if(r == row_id) return wt;
						row = row->next_row;
					}
				}
				else if(col->next_column) col = col->next_column;
				else break;
			}
		}
		else if(wt->next_wt) wt = wt->next_wt;
		else break;
	}
	return wt;
}