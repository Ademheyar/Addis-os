#include <Addis/Tables/WorkingT.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>




/*
*
*	WORKINGTABLE
*/
void Create_worktable(struct WORKTABLE *n) 
{ // this will create new worktable in a given pointed row
	if(!n) n = malloc(sizeof(struct WORKTABLE));
}

void Create_newworktable()
{ // this will create new main worktable
	struct READINGINFO *frtemp = get_reading_table(0);
	if(!frtemp->fread->worktables.worktable)
	{
		DEBUG("Create_newworktable\n");
		frtemp->fread->worktables.worktable = malloc(sizeof(struct WORKTABLE));
		frtemp->fread->worktables.last_wt = frtemp->fread->worktables.worktable;
		frtemp->fread->worktables.last_wt->column = NULL;
		frtemp->fread->worktables.last_wt->last_column = NULL;
		frtemp->fread->worktables.last_wt->focused_column = NULL;
		frtemp->fread->worktables.last_wt->next_wt = NULL;
		frtemp->fread->worktables.last_wt->prev_wt = NULL;
		frtemp->fread->worktables.focused_wt = frtemp->fread->worktables.worktable;
	}
	else {
		task_list_current->holded_info->read->fread->worktables.worktable = malloc(sizeof(struct WORKTABLE));
		frtemp->fread->worktables.last_wt = task_list_current->holded_info->read->fread->worktables.worktable;
		frtemp->fread->worktables.last_wt->column = NULL;
		frtemp->fread->worktables.last_wt->last_column = NULL;
		frtemp->fread->worktables.last_wt->focused_column = NULL;
		frtemp->fread->worktables.last_wt->next_wt = NULL;
		frtemp->fread->worktables.last_wt->prev_wt = NULL;
		frtemp->fread->worktables.focused_wt = task_list_current->holded_info->read->fread->worktables.worktable;
	}
}

void Create_nextworktable()
{ // this will create new main worktable
	DEBUG("Create_nextworktable\n");
	struct READINGINFO *frtemp = get_reading_table(0);
	frtemp->fread->worktables.focused_wt->next_wt = malloc(sizeof(struct WORKTABLE));
	frtemp->fread->worktables.focused_wt->next_wt->prev_wt = frtemp->fread->worktables.focused_wt;
	frtemp->fread->worktables.focused_wt->next_wt->next_wt = NULL;
	frtemp->fread->worktables.focused_wt->next_wt = frtemp->fread->worktables.last_wt;
}

void Create_lastworktable()
{ // this will create new main worktable
	struct READINGINFO *frtemp = get_reading_table(0);
	if(frtemp->fread->worktables.last_wt){
		DEBUG("Create_lastworktable\n");
		frtemp->fread->worktables.last_wt->next_wt = malloc(sizeof(struct WORKTABLE));
		frtemp->fread->worktables.last_wt->next_wt->prev_wt = frtemp->fread->worktables.last_wt;
		frtemp->fread->worktables.last_wt->next_wt->next_wt = NULL;
		frtemp->fread->worktables.last_wt = frtemp->fread->worktables.last_wt->next_wt;
		frtemp->fread->worktables.focused_wt = frtemp->fread->worktables.last_wt;
	}
	else Create_newworktable();
}

void Get_worktable_byindex(int wid)
{ // this will get worktable by index given index wid
	struct READINGINFO *frtemp = get_reading_table(0);
	DEBUG("Get_worktable_byindex %d\n", wid);
	if(wid < 0) { Create_lastworktable(); return; }
	else Create_newworktable();
	if(wid == 0) frtemp->fread->worktables.focused_wt = frtemp->fread->worktables.worktable;
	else {
		int i = 0;
		struct WORKTABLE *f = frtemp->fread->worktables.worktable;
		for(; f && f->next_wt && wid != i; i++) f = f->next_wt;
		if(i == 0 && wid >= 1) Create_lastworktable();
		else frtemp->fread->worktables.focused_wt = f;
	}
}

void Return_lastworktable()
{ // this will returns last worktable to previose reading table
	DEBUG("going to backup\n");
	task_list_current->holded_info->read->fread->reading_for[task_list_current->holded_info->read->fread->rfor_id] = "RETURN";
	struct USER_WDB *fread = task_list_current->holded_info->read->fread;
	if (fread->worktables.worktable) 
	{
		DEBUG("main worktable\n");
		if(fread->worktables.worktable->last_column)
		{
			fread->worktables.worktable->last_column->next_column = NULL;
			struct COLUMN *c = fread->worktables.worktable->column;
			while(1) {
				DEBUG("columnid\n");
				struct ROW *r = c->row;
				while(1){
					
					if(r->next_row) r = r->next_row;
					else break;
				}
				if(c->next_column) c = c->next_column;
				else break;
			}
		}
	}

	if (fread->worktables.last_wt) {
		DEBUG("last worktable\n");
		if(fread->worktables.last_wt->last_column)
		{
			DEBUG("columnid\n");
			fread->worktables.last_wt->last_column->next_column = NULL;
			struct COLUMN *c = fread->worktables.last_wt->column;
			while(1) {
				DEBUG("columnid\n");
				struct ROW *r;
				if(c->row)
				{
					r = c->row;
					int wr = 0;
					while(1){
						if (r->As) DEBUG("As<%s>\n", r->As);
						if (!(r->As)){
							if(r->type) Addrow_to_row_byindex(r, 0, wr);
							wr = 1;
						} 
						if(r->next_row) r = r->next_row;
						else break;
					}
				}
				if(c->next_column) c = c->next_column;
				else break;
			}
		}
		DEBUG("done ");
	}
	DEBUG("backuping\n");
}


void Clear_worktables(int which) {
	DEBUG("clearing Table");
	//struct READINGINFO *frtemp = get_reading_table(0);
	struct WORKTABLE *w = task_list_current->holded_info->read->fread->worktables.worktable, *non_delw = NULL;
	int wn = 0;
	for(;w && w->next_wt; wn++) w = w->next_wt;
	if(which < 0) { DEBUG("s "); which = wn; }
	DEBUG("\n");
	non_delw = NULL;
	for(; w; wn--) {
		struct COLUMN *c, *non_delcm = NULL;
		DEBUG("%d|", wn);
		if (!(wn <= which)) continue;
		if(w->column)
		{
			c = w->column;
			int wcn = 0;
			for(;c && c->next_column;wcn++) c = c->next_column;
			c->next_column = NULL;
			non_delcm = NULL;
			for(; c; wcn--) {
				DEBUG("c%d{", wcn);
				struct ROW *r, *non_del = NULL;
				if(c->row)
				{
					r = c->row;
					int wcrn = 0;
					for(;r && r->next_row;wcrn++) r = r->next_row;
					r->next_row = NULL;
					non_del = NULL;
					for(; r; wcrn--) {
						DEBUG("r%d[", wcrn);
						if(r->As && wn > 0) {
							if(!(r->type)) r->type = "";
							DEBUG("<%s>", r->type);
							if(r->on == 6 && r->value.variable.fuwdb && r->value.variable.fuwdb->name) DEBUG("n(%s)", r->value.variable.fuwdb->name);
							else if(r->word) DEBUG("v(%s)", r->word);
							DEBUG("As(%s)", r->As);
						}
						else{
							if(r->type) { DEBUG("<%s>", r->type); free(r->type); r->type = NULL; }
							if(r->on)
							{
								if(r->on == 6) 
								{
									if(r->value.variable.fuwdb){
										if (r->value.variable.fuwdb->name) DEBUG("n(%s)", r->value.variable.fuwdb->name);
										r->value.variable.fuwdb = NULL;
									}
								}
								r->on = 0;
							}
							else if(r->word) { DEBUG("v(%s)", r->word); free(r->word); r->word = NULL; }
						}
						DEBUG("]");
						
						DEBUG("R");
						if(r->prev_row) 
						{
							DEBUG("P");
							r = r->prev_row;
							if(r->next_row && r->next_row->type)
							{
								DEBUG("N");
								if (non_del)
								{
									DEBUG("N");
									r->next_row->next_row = non_del;
									non_del->prev_row = r->next_row;
								}
								non_del = r->next_row;
							}
							else
							{
								free(r->next_row);
								if (non_del)
								{
									DEBUG("O");
									r->next_row = non_del;
									non_del->prev_row = r->next_row;
								}
								else r->next_row = NULL;
							}
							DEBUG("D");
							continue;
						}
						else {
							if(r && r->type){
								DEBUG("M");
								if (non_del)
								{	
									DEBUG("O");
									r->next_row = non_del;
									non_del->prev_row = r;
								}
								else r->next_row = NULL; 
								non_del = r;
							}
							else 
							{
								DEBUG("D");
								free(r);
								if (non_del) {
									DEBUG("O");
									r = non_del;
								}
								else r = NULL;
							}
						} 
						break;
					}
					DEBUG("}");
				}
				DEBUG("C");
				if(c->prev_column) 
				{
					DEBUG("P");
					c = c->prev_column;
					if(non_del)
					{
						DEBUG("R");
						if(non_delcm){
							DEBUG("O");
							c->next_column->next_column = non_delcm;
							non_delcm->prev_column = c->next_column;
						}
						non_delcm = c->next_column;
					}
					else{
						if(non_delcm){
							DEBUG("O");
							c->next_column = non_delcm;
							non_delcm->prev_column = c;
						}
						else c->next_column = NULL;
					}
					continue;
				}
				else {
					if(non_del){
						DEBUG("M");
						if (non_delcm)
						{	
							DEBUG("O");
							c->next_column = non_delcm;
							non_delcm->prev_column = c;
						}
						else c->next_column = NULL;
						non_delcm = c;
					}
					else {
						DEBUG("D");
						free(c);
						if (non_delcm)  {
							DEBUG("O");
							c = non_delcm;
						}
						else c = NULL;
					}
				}
				break;
			}
		}
		DEBUG("|");
		DEBUG("W");
		if(w->prev_wt) 
		{
			DEBUG("P");
			w = w->prev_wt;
			if(non_delcm) {
				DEBUG("C");
				if(non_delw)
				{
					DEBUG("O");
					w->next_wt->next_wt = non_delw;
					non_delw->prev_wt = w->next_wt;
				}
				non_delw = w->next_wt;
			}
			else {
				free(w->next_wt);
				if(non_delw)
				{
					DEBUG("O");
					w->next_wt = non_delw;
					non_delw->prev_wt = w;
				}
				else w->next_wt = NULL;
			}
			continue;
		}
		else{
			DEBUG("M");
			if(non_delcm){
				if (non_delw)
				{	
					DEBUG("O");
					w->next_wt = non_delw;
					non_delw->prev_wt = w;
					non_delw = w;
				}
				else w->next_wt = NULL;
			}
			else  {
				DEBUG("D");
				if(wn > 0){
					DEBUG(">");
					if (non_delw) w = non_delw;
					else{
						DEBUG("O");
						if(w->column) free(w->column);
						w->column = NULL;
						w->last_column = NULL;
						w->focused_column = NULL;
						w->prev_wt = NULL;
						w->next_wt = NULL;
					}
				}
				else {
					DEBUG("0");
					if(w->column) free(w->column);
					w->column = NULL;
					w->last_column = NULL;
					w->focused_column = NULL;
					w->prev_wt = NULL;
				}
			}
		} 
		break;
	}
	DEBUG("\n");

	struct WORKTABLE *w0 = task_list_current->holded_info->read->fread->worktables.worktable;
	for(int wn = 0; w0; wn++) {
		DEBUG("%d|", wn);
		struct COLUMN *c0 = w0->column;
		for(int wcn = 0; c0; wcn++) {
			DEBUG("c0%d{", wcn);
			struct ROW *r0 = c0->row;
			for(int wcrn = 0; r0; wcrn++) {
				DEBUG("r0%d[", wcrn);
				if(r0->type) DEBUG("<%s>", r0->type);
				if(r0->As) DEBUG("As(%s)", r0->As);
				if(r0->on == 6 && r0->value.variable.fuwdb && r0->value.variable.fuwdb->name) DEBUG("n(%s)", r0->value.variable.fuwdb->name);
				else if(r0->word) DEBUG("v(%s)", r0->word);
				DEBUG("]");
				if(r0->next_row) r0 = r0->next_row;
				else break;
			}
			DEBUG("}");
			if(c0->next_column) c0 = c0->next_column;
			else break;
		}
		DEBUG("|");
		if(w0->next_wt) w0 = w0->next_wt;
		else break;
	}
	DEBUG("\n");

	if(non_delw)
	{
		task_list_current->holded_info->read->fread->worktables.last_wt = non_delw;
		task_list_current->holded_info->read->fread->worktables.focused_wt = non_delw;
	}
	else if(!w)
	{
		task_list_current->holded_info->read->fread->worktables.last_wt = NULL;
		task_list_current->holded_info->read->fread->worktables.focused_wt = NULL;
	}
}


/*
*
*	struct COLUMN
*/
void Create_column(struct COLUMN *n) 
{ // this will return given pointed with new column
	if(!n) n = malloc(sizeof(struct COLUMN));
}

void Create_newcolumn()
{ // this will create new main column
	DEBUG("Create_newcolumn\n");
	struct READINGINFO *frtemp = get_reading_table(0);
	frtemp->fread->worktables.focused_wt->column = malloc(sizeof(struct COLUMN));
	frtemp->fread->worktables.focused_wt->last_column = frtemp->fread->worktables.focused_wt->column;
	frtemp->fread->worktables.focused_wt->last_column->next_column = NULL;
	frtemp->fread->worktables.focused_wt->last_column->prev_column = NULL;
	frtemp->fread->worktables.focused_wt->focused_column = frtemp->fread->worktables.focused_wt->last_column;
}

void Get_column_byindex(int cid)
{ // this will get column by index or the last column
	DEBUG("getting focused column\n");
	struct READINGINFO *frtemp = get_reading_table(0);
	if(cid == 0) frtemp->fread->worktables.focused_wt->focused_column = frtemp->fread->worktables.focused_wt->column;
	else {
		DEBUG("getting columen ");
		struct COLUMN *f = frtemp->fread->worktables.focused_wt->column;
		for(int i = 0; f->next_column && f->row->type && cid != i; i++) f = f->next_column;
		frtemp->fread->worktables.focused_wt->focused_column = f;
		DEBUG("\n");
	}
}

void Create_lastcolumn_byindex() 
{ // this will create new last column in a given wtid indexs for table
	struct READINGINFO *frtemp = get_reading_table(0);
	DEBUG("Create_lastcolumn_byindex\n");
	if(frtemp->fread->worktables.focused_wt)
	{
		DEBUG("have focused_wt\n");
		if (frtemp->fread->worktables.focused_wt->column && frtemp->fread->worktables.focused_wt->last_column)
		{
			DEBUG("have last_column\n");
			frtemp->fread->worktables.focused_wt->last_column->next_column = malloc(sizeof(struct COLUMN));
			frtemp->fread->worktables.focused_wt->last_column->next_column->next_column = NULL;
			frtemp->fread->worktables.focused_wt->last_column->next_column->prev_column = frtemp->fread->worktables.focused_wt->last_column;
			frtemp->fread->worktables.focused_wt->last_column = frtemp->fread->worktables.focused_wt->last_column->next_column;
			frtemp->fread->worktables.focused_wt->focused_column = frtemp->fread->worktables.focused_wt->last_column;
		}
		else Create_newcolumn();
	}
	else Get_worktable_byindex(-1);
}

/*
*
*	struct ROW
*/

void Create_row(struct ROW *n) 
{ // this will create new main row in a given pointed row
	if(!n) n = malloc(sizeof(struct ROW));
}

void Create_newrow()
{ // this will create new main row in a given pointed row
	struct READINGINFO *frtemp = get_reading_table(0);
	frtemp->fread->worktables.focused_wt->focused_column->row = malloc(sizeof(struct ROW));
	frtemp->fread->worktables.focused_wt->focused_column->row->next_row = NULL;
	frtemp->fread->worktables.focused_wt->focused_column->row->prev_row = NULL;
	frtemp->fread->worktables.focused_wt->focused_column->row->on = 0;
	frtemp->fread->worktables.focused_wt->focused_column->last_row = frtemp->fread->worktables.focused_wt->focused_column->row;
	frtemp->fread->worktables.focused_wt->focused_column->focused_row = frtemp->fread->worktables.focused_wt->focused_column->row;
}

struct ROW *Create_nextrow(struct ROW *n) 
{ // this will creates new row next to pointed row
	struct ROW *out = malloc(sizeof(struct ROW));
	out->prev_row = n;
	out->next_row = n->next_row;
	out->on = 0;
	n->next_row = out;
	return out;
}

//void Create_nextrow_byindex(int wtid, int wcid);

void Create_lastrow_byindex(int wtid, int wcid) 
{ // this will create new last row in a given indexs wtid for table and wcid for table column
	struct READINGINFO *frtemp = get_reading_table(0);
	if (wtid != -1) Get_worktable_byindex(wtid);
	if (wcid != -1) Get_column_byindex(wcid);
	if (frtemp->fread->worktables.focused_wt->focused_column->last_row)
	{
		struct ROW *out = malloc(sizeof(struct ROW));
		out->prev_row = frtemp->fread->worktables.focused_wt->focused_column->last_row;
		out->next_row = NULL;
		out->on = 0;
		frtemp->fread->worktables.focused_wt->focused_column->last_row->next_row = out;
		frtemp->fread->worktables.focused_wt->focused_column->last_row = out;
		frtemp->fread->worktables.focused_wt->focused_column->focused_row = out;
	}
	else Create_newrow();
}

void Get_row_byindex(int rid)
{ // this will get worktable by index
	//DEBUG("getting focused row\n");
	struct READINGINFO *frtemp = get_reading_table(0);
	if(rid == 0) frtemp->fread->worktables.focused_wt->focused_column->focused_row = frtemp->fread->worktables.focused_wt->focused_column->row;
	else {
		struct ROW *r = frtemp->fread->worktables.focused_wt->focused_column->row;
		for(int i = 0; r->next_row && rid != i; i++) r = r->next_row;
		frtemp->fread->worktables.focused_wt->focused_column->focused_row = r;
	}
}

char *get_row_type(struct ROW *row)
{
	if(row && row->value.istype) {
		if(row->on == 2 && row->value.number.rational || 
			 row->on == 3 && row->value.hex.hex_32 /*|| row->value.hex.hex_16*/ || 
			 row->on == 3 && row->value.hex.hex_8) return "NUMBER";
		else if(row->on == 6 && row->value.variable.fuwdb && row->value.variable.fuwdb->name) return "VARIABLE";
		
		else if (row->on == 1 && issame(row->type, "CHARACTER") || issame(row->type, "LETTER")) {
			//if(issame(row->type, "CHARACTER")) { new->value[0] = row->charcter; new->value[1] = s->charcter;  }
			//if(issame(row->type, "LETTER")) { new->word[0] = row->letter; new->word[1] = s->letter; }
		}
		else if (row->word) return "WORD";
	}
	return row->type;
}


//void Add_rowvalue(struct ROW *n, char *type, void *value);
_Bool fix_varprop(struct USER_WDB *var, char *dowhat, struct ROW *value)
{
	DEBUG("\ngoing to %s with value(", dowhat);
	if(value->type) DEBUG("value->type|%s| ", value->type);
	if(value->on) DEBUG("value->on[%d] ", value->on);
	if(value->word) DEBUG("value->word|%s| ", value->word);
	DEBUG(") TO var (");
	if(var->name) DEBUG("var->name|%s| ", var->name);
	if(var->value.on) DEBUG("var->value.on[%d] ", var->value.on);
	if(var->type) DEBUG("var->type|%s| ", var->type);
	if(var->value.type) DEBUG("var->value.type|%s| ", var->value.type);
	DEBUG(")");

	if(issame(value->type, "NAME")) {
		DEBUG(" NAME|%s|And|%s| = ", var->name, value->word);
		if(issame(dowhat, "ADD")) {	
			if (!var->name || issame(var->name, "")) var->name = strdup(value->word);
			else var->name = stradd(var->name, value->word, 0);
		}
		
		else if(issame(dowhat, "SET")) {
			var->name = strdup(value->word);
		}
		DEBUG("var name(%s)\n", var->name);
		return 1;
	}

	else if(issame(value->type, "WITH")) {
		DEBUG(" WITH|%s|And|%s| = ", var->withcodes, value->word);
		if(issame(dowhat, "ADD")) {	
			var->withcodes = stradd(var->withcodes, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->withcodes = strdup(value->word);
		}
		DEBUG("%s", var->withcodes);
		return 1;
	}
	else if(issame(value->type, "VALUE") || issame(value->type, "DO")) {
		DEBUG("%s ", dowhat);
		if(var->value.on == 2 && value->word){
			DEBUG("|%s|And|%s| = ", var->value.word, value->word);
			if(issame(dowhat, "ADD")) {	
				var->value.word = stradd(var->value.word, value->word, 0);
			}
			else if(issame(dowhat, "SET")) {
				var->value.word = strdup(value->word);
			}
			var->value.type = strdup(value->type);
			DEBUG("%s type %s", var->value.word, var->value.type);
		}
		else if(var->value.on == 4 && value->word){
			DEBUG(" = %s >> ", value->word);
			if(issame(dowhat, "ADD")) {	
				if(var->value.value.image.text) var->value.value.image.text = stradd(var->value.value.image.text, value->word, 0);
				else var->value.value.image.text = strdup(value->word);
			}
			else if(issame(dowhat, "SET")) {
				var->value.value.image.text = strdup(value->word);
			}
			var->value.on = 4;
			if(!var->value.type || issame(var->value.type, "")) var->value.type = strdup(value->type);
			DEBUG("%s type %s", var->value.value.image.text, var->value.type);
		}
		return 1;
	}
	
	else if(issame(value->type, "SHAPE")) {
		DEBUG(" = %s >> ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.shap_code = stradd(var->value.value.image.shap_code, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->value.value.image.shap_code = strdup(value->word);
		}
		//
		if(!var->value.type || issame(var->value.type, "")) var->value.type = strdup(value->type);
		var->value.on = 4;
		DEBUG("%s var name(%s)", var->value.value.image.shap_code, var->name);
		return 1;
	}
	
	else if(issame(value->type, "X")) {
		DEBUG(" = %s >> ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.x_code = stradd(var->value.value.image.x_code, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->value.value.image.x_code = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s var name(%s)", var->value.value.image.x_code, var->name);
		return 1;
	}
	else if(issame(value->type, "Y")) {
		DEBUG(" = %s >> ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.y_code = stradd(var->value.value.image.y_code, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->value.value.image.y_code = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s var name(%s)", var->value.value.image.y_code, var->name);
		return 1;
	}
	else if(issame(value->type, "WIDTH")) {
		DEBUG(" = %s >> ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.w_code = stradd(var->value.value.image.w_code, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->value.value.image.w_code = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s var name(%s)", var->value.value.image.w_code, var->name);
		return 1;
	}
	else if(issame(value->type, "HEIGHT")) {
		DEBUG(" = %s >> ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.h_code = stradd(var->value.value.image.h_code, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {
			var->value.value.image.h_code = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s", var->value.value.image.h_code);
		return 1;
	}
	else if(issame(value->type, "FRONT_COLOR")) {
		DEBUG(" %s FRONT_COLOR ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.fg_color = stradd(var->value.value.image.fg_color, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {	
			var->value.value.image.fg_color = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s", var->value.value.image.fg_color);	
		return 1;
	}
	else if(issame(value->type, "BACK_COLOR")) {
		DEBUG(" %s BACK_COLOR ", value->word);
		if(issame(dowhat, "ADD")) {	
			var->value.value.image.bg_color = stradd(var->value.value.image.bg_color, value->word, 0);
		}
		else if(issame(dowhat, "SET")) {	
			var->value.value.image.bg_color = strdup(value->word);
		}
		var->value.on = 4;
		DEBUG("%s", var->value.value.image.bg_color);	
		return 1;
	}
	else if(issame(value->type, "NUMBER")){
		
	}
	else if(issame(value->type, "HEX")){
		if(value->on == 31){
			DEBUG("|uint8_c|And|%c| = ",  value->value.hex.hex_8);
			if(issame(dowhat, "ADD")) {	
				if(value->value.hex.hex_8) var->value.value.hex.hex_8 = var->value.value.hex.hex_8 + value->value.hex.hex_8;
				else var->value.value.hex.hex_8 = value->value.hex.hex_8;
			}
			else if(issame(dowhat, "SET")) {
				var->value.value.hex.hex_8 = value->value.hex.hex_8;
			}
			var->value.type = strdup(value->type);
			var->value.on = 31;
			DEBUG("%d type %s", var->value.value.hex.hex_8, var->value.type);
			Chack_all_events(var, "SETTED", &var->value);
		}
	}
	return 0;
}

_Bool compar(char *comp, struct ROW *v1, struct ROW *v2)
{ // this will compar two rows
	_Bool result = false;
	comp = get_compar_type(comp);
	DEBUG("going to compar\n");
	for (int i = 0; *(comp + i); i++){
		char c = *(comp + i);
		if (c == '|' && result == 0) continue;
		else if (c == '|' && result == 1) break;
		if (c == '&' && result == 1) continue;
		else if (c == '&' && result == 0) break;
		if (c == '!') { result = result ? 0 : 1;  continue; }
		if (v1->type && v2->type)
		{
			DEBUG("chack if [%d](%s) (%c) [%d]%s\n", v1->on, v1->type, c,  v2->on, v2->type);
			if(c == '='){
				// hex_8 by hex_8 
				if(v1->on == 31 && v2->on == 31) result = v1->value.hex.hex_8 == v2->value.hex.hex_8;
				// hex_8 by hex_32 
				else if(v1->on == 31 && v2->on == 33) result = v1->value.hex.hex_8 == v2->value.hex.hex_32;
				
				// hex_32 by hex_8 
				else if(v1->on == 33 && v2->on == 31) result = v1->value.hex.hex_32 == v2->value.hex.hex_8;
			}
			DEBUG("DONE Chacking\n");
		}
	}

	DEBUG(" compered by(%s) result(%d)\n", comp, result);
	return result;
}

struct ROW *Get_var_prop(struct ROW *v)
{
	if(v->on == 6 && v->value.variable.prop && v->value.variable.fuwdb)
	{
		// TODO: WHAT IF HAVE MORE THAN ONE PROP TO GET
		DEBUG("going to get prop %s in var %s\n", v->value.variable.prop, v->value.variable.fuwdb->name);
		struct ROW *result = malloc(sizeof(struct  ROW));
		if(issame(v->value.variable.prop, "X"))
		{
			result->type = "WORD";
			result->on = 2;
			result->word = stradd(v->value.variable.fuwdb->value.value.image.x_code, "", 0);
			return result;
		}
		else if(issame(v->value.variable.prop, "Y"))
		{
			result->type = "WORD";
			result->on = 2;
			result->word = stradd(v->value.variable.fuwdb->value.value.image.y_code, "", 0);
			return result;
		}
		else if(issame(v->value.variable.prop, "WIDTH"))
		{
			result->type = "WORD";
			result->on = 2;
			result->word = stradd(v->value.variable.fuwdb->value.value.image.w_code, "", 0);
			return result;
		}
		else if(issame(v->value.variable.prop, "HEIGHT"))
		{
			if(v->value.variable.fuwdb->value.value.image.h_code)
			{
				result->type = "WORD";
				result->on = 2;
				result->word = stradd(v->value.variable.fuwdb->value.value.image.h_code, "", 0);
			}
			return result;
		}
		else {
			DEBUG("NOT DEFINED HOW TO RETURN\n");
			free(result);
			return NULL;
		}
		
	}
	return NULL;
}

struct ROW *marge_rows(char *op, struct ROW *v1, struct ROW *v2)
{ // this will marge two rows
	struct ROW *result = malloc(sizeof(struct ROW));
	DEBUG("going to marge %s\n", op);
	for (int i = 0; *(op + i); i++){
		char c = *(op + i);
		if(v2->on && (v1->on == 6)) v1 = Get_var_prop(v1);
		if(v2->on && (v2->on == 6)) v2 = Get_var_prop(v2);

		DEBUG("marg ");
		if (v1->on) DEBUG("[%d] ", v1->on);
		if (v1->type) DEBUG("(%s) ", v1->type);
		DEBUG(" %c ", c);
		if (v2->on) DEBUG("[%d] ", v2->on);
		if (v2->type) DEBUG("(%s) ", v2->type);
		DEBUG("\n");
		if (v1->type && v2->type && issame(v1->type, v2->type) || v1->on && v2->on && v1->on == v2->on)
		{
			result->type = strdup(v1->type);
			if(c == '-'){
				// hex_8 by hex_8 
				if(v1->on == 31 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_8 == v2->value.hex.hex_8;
				// hex_8 by hex_32 
				else if(v1->on == 31 && v2->on == 33) result->value.number.rational = v1->value.hex.hex_8 == v2->value.hex.hex_32;
				
				// hex_32 by hex_8 
				else if(v1->on == 33 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_32 - v2->value.hex.hex_8;

				else if(v1->on == 33 && v2->on == 33) 
				{
					result->on = 33;
					result->value.number.rational = v1->value.number.rational - v2->value.number.rational;
					DEBUG("resualt %d\n", result->value.number.rational);
				}
			}
			else if(c == '+'){
				// hex_8 by hex_8 
				if(v1->on == 31 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_8 + v2->value.hex.hex_8;
				// hex_8 by hex_32 
				else if(v1->on == 31 && v2->on == 33) result->value.number.rational = v1->value.hex.hex_8 + v2->value.hex.hex_32;
				
				// hex_32 by hex_8 
				else if(v1->on == 33 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_32 + v2->value.hex.hex_8;

				else if(v1->on == 33 && v2->on == 33) 
				{
					result->on = 33;
					result->value.number.rational = v1->value.number.rational + v2->value.number.rational;
					DEBUG("resualt %d\n", result->value.number.rational);
				}
			}
			else if(c == '/'){
				// hex_8 by hex_8 
				if(v1->on == 31 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_8 == v2->value.hex.hex_8;
				// hex_8 by hex_32 
				else if(v1->on == 31 && v2->on == 33) result->value.number.rational = v1->value.hex.hex_8 == v2->value.hex.hex_32;
				
				// hex_32 by hex_8 
				else if(v1->on == 33 && v2->on == 31) result->value.number.rational = v1->value.hex.hex_32 - v2->value.hex.hex_8;

				else if(v1->on == 33 && v2->on == 33) 
				{
					result->on = 33;
					result->value.number.rational = v1->value.number.rational / v2->value.number.rational;
					DEBUG("resualt %d\n", result->value.number.rational);
				}
			}
			DEBUG("DONE Chacking\n");
		}
		else 
		{
			if(c == '+'){
				if(v1->on == 2)
				{
					if(v2->on == 33) {
						result->type = "WORD";
						result->on = 2;
						char t[100]; 
						sprintf(t,"%d",  v2->value.number.rational);
						result->word = stradd(v1->word, "+", ' ');
						result->word = stradd(result->word, t, ' ');
						DEBUG("resualt %s\n", result->word);
					}
				}
			}
			else if(c == '-'){
				if(v1->on == 2)
				{
					if(v2->on == 33) {
						result->type = "WORD";
						result->on = 2;
						char t[100]; 
						sprintf(t,"%d",  v2->value.number.rational);
						result->word = stradd(v1->word, "-", ' ');
						result->word = stradd(result->word, t, ' ');
						DEBUG("resualt %s\n", result->word);
					}
				}
			}
		}
	}
	DEBUG(" marged by(%s) on(%d)\n", op, result->on);
	return result;
}

struct ROW *marge_row(struct ROW *f, struct ROW *s){
	struct ROW *new = malloc(sizeof(struct ROW));
	DEBUG("marging f->on %d && s->on %d\n", f->on, s->on);
	while(1);
	if(f->on == 1 && s->on == 1) {} // char by char = str
	//if(f->on == 1 && s->on == 21) {} // char by number = hex or name
	if(f->on == 1 && s->on == 22) {} // char by Rational = name
	if(f->on == 1 && s->on == 31) {} // char by hex = hex or name
	if(f->on == 1 && s->on == 33) {} // char by hex = hex or name
	if(f->on == 1 && s->on == 7) {} // char by str = hex or name

	if(f->on == 22 && s->on == 1) {} // Rational by char = name
	if(f->on == 22 && s->on == 22) {} // Rational by Rational = ratinal
	if(f->on == 22 && s->on == 31) {} // Rational by hex_8 = name or hex
	if(f->on == 22 && s->on == 33) {} // Rational by hex_32 = name or hex
	if(f->on == 22 && s->on == 7) {} // Rational by str = name

	if(f->on == 31 && s->on == 1) {} // hex_8 by char = name or hex
	if(f->on == 31 && s->on == 22) {} // hex_8 by Rational = ratinal
	if(f->on == 31 && s->on == 31) {} // hex_8 by hex_8 = hex_32
	if(f->on == 31 && s->on == 33) {} // hex_8 by hex_32 = hex_32
	if(f->on == 31 && s->on == 7) {} // hex_8 by str = name

	if(f->on == 33 && s->on == 1) {} // hex_32 by char = name or hex
	if(f->on == 33 && s->on == 22) {} // hex_32 by Rational = ratinal
	if(f->on == 33 && s->on == 31) {} // hex_32 by hex_8 = hex_32
	if(f->on == 33 && s->on == 33) {} // hex_32 by hex_32 = hex_64
	if(f->on == 33 && s->on == 7) {} // hex_32 by str = name

	if(f->on == 7 && s->on == 1) {} // hex_32 by char = name or hex
	if(f->on == 7 && s->on == 22) {} // hex_32 by Rational = ratinal
	if(f->on == 7 && s->on == 31) {} // hex_32 by hex_8 = hex_32
	if(f->on == 7 && s->on == 33) {} // hex_32 by hex_32 = hex_64
	if(f->on == 7 && s->on == 7) {} // str by str = paragraf

	/*	else if(issame(row->type, "NUMBER") || issame(row->type, "NATURAL") || issame(row->type, "RATIONAL") || 
						issame(row->type, "WHOLE")){
			//"NATURAL"
			
			//"RATIONAL"
			dest->value.number.rational = row->value.number.rational;
			dest->on = 22;
		}


	if(f->type && s->type && issame(f->type, s->type)) {

		if(f->type && (issame(f->type, "NUMBER") || issame(f->type, "NATURAL") ||
     issame(f->type, "RATIONAL") || issame(f->type, "WHOLE"))){	

			new->value.number.rational = f->value.number.rational + s->value.number.rational;
			new->value.hex.hex_32 = f->value.hex.hex_32 + s->value.hex.hex_32;
			//new->value.hex.hex_16 = (uint16_t)new->value.hex.hex_32;
			new->value.hex.hex_8 = (uint8_t)new->value.hex.hex_32;
		}
		else if (issame(f->type, "WORD")) new->word = stradd(f->word, s->word, ' ');
		else if (issame(f->type, "NAME")) new->word = stradd(f->word, s->word, 0);
	}

		


		else if(issame(row->type, "HEX")){
			if(row->on == 31){
				dest->value.hex.hex_8 = row->value.hex.hex_8; 
				dest->on = 31;
			}
			else {
			//row->value.hex.hex_16 = (uint16_t)row->value.hex.hex_32;
				dest->value.hex.hex_32 = row->value.hex.hex_32;
				dest->on = 33;
			}
		}
		
		else if(row->on == 6 && (issame(row->type, "VARIABLE"))){
			dest->value.variable.fuwdb = row->value.variable.fuwdb;
			if(dest->value.variable.fuwdb->name) DEBUG("name(%s) ", dest->value.variable.fuwdb->name);
			dest->on = 6;
		}
		else if (row->on == 7){
			dest->word = strdup(row->word);
			dest->on = 7;
		}



	

	if(f->type && (issame(f->type, "NUMBER") || issame(f->type, "NATURAL") ||
								issame(f->type, "RATIONAL") || issame(f->type, "WHOLE"))){	
		new->value.number.rational = f->value.number.rational + s->value.number.rational;
		new->value.hex.hex_32 = f->value.hex.hex_32 + s->value.hex.hex_32;
		//new->value.hex.hex_16 = (uint16_t)new->value.hex.hex_32;
		new->value.hex.hex_8 = (uint8_t)new->value.hex.hex_32;
	}
	else if (f->type && issame(f->type, "CHARACTER") || issame(f->type, "LETTER")) {
		//if(issame(f->type, "CHARACTER")) { new->value[0] = f->charcter; new->value[1] = s->charcter;  }
		//if(issame(f->type, "LETTER")) { new->word[0] = f->letter; new->word[1] = s->letter; }
	}
	else if (issame(f->type, "WORD")) new->word = stradd(f->word, s->word, ' ');
	else if (issame(f->type, "NAME")) new->word = stradd(f->word, s->word, 0);
	*/
	return new;
}

struct ROW *Convert_text(char *type, char *value)
{
  struct ROW *row = malloc(sizeof(struct ROW));
  if(issame(type, "")) type = get_value_type(value);
  if(issame(type, "")) { free(row); return NULL; }
  row->type = strdup(type); 
  DEBUG("converting text to row type %s\n", row->type);
  if (issame(type, "CHARACTER") || issame(type, "LETTER")) {
    row->value.charcter = *value;
    row->on = 1;
  }

  else if(issame(type, "NUMBER") || issame(type, "NATURAL") || issame(type, "RATIONAL") || 
          issame(type, "WHOLE")){
    //"NATURAL"
    //"RATIONAL"
    row->value.number.rational = atoi(value);
    row->on = 33;
  }

  else if(issame(type, "HEX")){
    uint16_t a = strtohex(value);
    if(a <= 127){
      row->value.hex.hex_8 = (uint8_t)a; 
      row->on = 41;
    }
    else {
    //row->value.hex.hex_16 = (uint16_t)row->value.hex.hex_32;
      row->value.hex.hex_32 = a;
      row->on = 43;
    }
  }

	else if(issame(type, "LOCATION")){
    uint16_t a = strtohex(value);
    if(a <= 127){
      row->value.hex.hex_8 = (uint8_t)a; 
      row->on = 51;
    }
    else {
    //row->value.hex.hex_16 = (uint16_t)row->value.hex.hex_32;
      row->value.hex.hex_32 = a;
      row->on = 53;
    }
  }

  else {
		row->word = strdup(value);
		if(row->word) DEBUG(" value(%s) ", row->word);
		row->on = 2;
  }
	
  return row;
}

void copy_row(struct ROW *dest, struct ROW *row)
{
	if(row->type && dest) {
		dest->type = strdup(row->type);
		if (row->As) dest->As = strdup(row->As);
  	DEBUG(" copying row values to new row = type %s on %d ", row->type, row->on);
		if (row->on == 1 && (issame(row->type, "CHARACTER") || issame(row->type, "LETTER"))) {
			dest->value.charcter = row->value.charcter;
			dest->on = 1;
		}

		else if(row->on == 22 && (issame(row->type, "NUMBER") || issame(row->type, "NATURAL") || issame(row->type, "RATIONAL") || 
						issame(row->type, "WHOLE"))){
			//"NATURAL"
			
			//"RATIONAL"
			dest->value.number.rational = row->value.number.rational;
			dest->on = 22;
		}

		else if(issame(row->type, "HEX")){
			if(row->on == 31){
				dest->value.hex.hex_8 = row->value.hex.hex_8; 
				dest->on = 31;
			}
			else if (row->on == 33) {
			//row->value.hex.hex_16 = (uint16_t)row->value.hex.hex_32;
				dest->value.hex.hex_32 = row->value.hex.hex_32;
				dest->on = 33;
			}
		}
		
		else if(issame(row->type, "LOCATION")){
			if(row->on == 51){
				dest->value.hex.hex_8 = row->value.hex.hex_8; 
				dest->on = 51;
			}
			else if (row->on == 52){
			//row->value.hex.hex_16 = (uint16_t)row->value.hex.hex_32;
				dest->value.hex.hex_32 = row->value.hex.hex_32;
				dest->on = 52;
			}
		}
		
		else if(row->on == 6 && (issame(row->type, "VARIABLE"))){
			dest->on = 6;
			dest->value.variable.fuwdb = row->value.variable.fuwdb;
			if(dest->value.variable.fuwdb->name) DEBUG(" name(%s) ", dest->value.variable.fuwdb->name);
		}
		
		else {
			dest->word = stradd(row->word, "", 0);
			if(dest->word) DEBUG(" value(%s) ", dest->word);
			if(!row->on || row->on == 2) dest->on = 2;
		}
	}
	DEBUG("\n");
}

void Addrow_to_row_byindex(struct ROW *row, int wt, int wr)
{ // this will give value to a given indexs wtid for table and wcid for table column
	// this will create next row on focesed column if wr is 1 or on new column of wr is 0
	struct READINGINFO *frtemp = get_reading_table(0);
	if(frtemp->fread->getas && !issame(frtemp->fread->getas, "")||
		 row->As && !issame(row->As, "")) wt = wt ? wt : 1;

	Get_worktable_byindex(wt);

	DEBUG("Going Add(%d) new value on table[%d](%s) by index ", wr, wt, frtemp->fread->reading_for[frtemp->fread->rfor_id]);
	if(wr == 0 || !frtemp->fread->worktables.focused_wt->focused_column){
		Create_lastcolumn_byindex();
		Create_newrow();
	}
	if(wr == 1 || wr == 1 && !frtemp->fread->worktables.focused_wt->focused_column->last_row->next_row)
	{
		Create_lastrow_byindex(wt, -1);
	}
	DEBUG("t%dcr", wt);
	copy_row(frtemp->fread->worktables.focused_wt->focused_column->focused_row, row);
	if(row->type)	DEBUG("<%s>", frtemp->fread->worktables.focused_wt->focused_column->last_row->type);
	/*if(frtemp->fread->getas && !issame(frtemp->fread->getas, "")){
		frtemp->fread->worktables.focused_wt->focused_column->focused_row->As = strdup(frtemp->fread->getas);
		frtemp->fread->getas = "";
	}*/
	//if(frtemp->fread->worktables.focused_wt->focused_column->focused_row->As) DEBUG("And AS(%s) ", frtemp->fread->worktables.focused_wt->focused_column->focused_row->As);
	DEBUG("\n");
}

struct ROW *get_as_row(char *name)
{
	DEBUG("Gatting As row %s \n", name);
	int wid = 0;
	struct WORKTABLE *w = task_list_current->holded_info->read->fread->worktables.worktable;
	for(; wid < 1 && w && w->next_wt; wid++) w = w->next_wt;
	while(wid >= 1){
		if(w){
			DEBUG("found first workingtabloe %d\n", wid);
			wid++;
			struct COLUMN *c = w->column;
			if(w->column){
				DEBUG("found first colomen\n");
				c = w->column;
				while(1) {
					DEBUG("found first row\n");
					struct ROW *r = c->row;
					while(1) {
						if(c){
							if(r->type) DEBUG("<%s>", r->type);
							if(r->value.variable.fuwdb && r->value.variable.fuwdb->name) DEBUG("n(%s)", r->value.variable.fuwdb->name);
							else if(r->word) DEBUG("v(%s)", r->word);
							if(r->As) DEBUG("AS<%s>", r->As);
							if(r->As && issame(r->As, name)) return r; 
							DEBUG("\n");

						}
						DEBUG("going to next row\n");
						if(r->next_row) r = r->next_row;
						else break;
					}
					DEBUG("going to next colomen\n");
					if(c->next_column) c = c->next_column;
					else break;
				}
			}
		}
		DEBUG("going to next workingtable\n");
		if(w->next_wt) w = w->next_wt;
		else break;
	}
	DEBUG("\n");
	return NULL;
}



struct ROW *get_worktable(list_t *read, int isret){
  isret=isret;
  DEBUG("in get tabel");
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
  while(1){
  	DEBUG(" reading |%s|\n", readword->value);
    if (issame(readword->value, "TABLE")) {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG(" table{%s}", readword->value);
      if (issame(readword->value, "0")) {
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				int wtsid = -1, wtcid = -1, wtrid = -1;
        for(int w = 0; w < 3; w++)
        {
          readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
          convertto.integer = -1;
          convert(readword->value);
          char *q = readword->value;
          if (!(*q >= '0' && *q <= '9'))
          {
            DEBUG("\n");
            char *got = ReadResivers(read, "", "", 'R', 1)->word;
            readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
            convert(got);
            DEBUG("in[%s] ", got);
          }
          else task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          DEBUG("[%s>>%d]", readword->value, convertto.integer);
          if (convertto.integer > -1){
            if(wtsid == -1) wtsid = convertto.integer;
						else if(wtcid == -1) wtcid = convertto.integer;
						else if(wtrid == -1) {
              wtrid = convertto.integer;
              sysinfo.fids++;
              sysinfo.fswid[sysinfo.fids] = wtsid;
              sysinfo.fscid[sysinfo.fids] = wtcid;
              sysinfo.fsrid[sysinfo.fids] = wtrid;
              DEBUG("saved sysinfo.fids[%d] table[0][%d][%d][%d] on>> table[%d][%d][%d]\n", sysinfo.fids, wtsid, wtcid, wtrid, sysinfo.fswid[sysinfo.fids], sysinfo.fscid[sysinfo.fids], sysinfo.fsrid[sysinfo.fids]);
            }
          }
        }
      }
      else if (issame(readword->value, "1")) {
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				DEBUG("searching table 1\n");
        while (1){
          readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
          if (issame(readword->value, "NEW")){
            task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
            readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						DEBUG("G createing new table 1 %s \n", readword->value);
						Create_vartable(read, NULL, 0); 
            return NULL;
          }
          //else task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        }
      }
      else // or if index was't given
      {

      }
      return NULL;
    }
  }
  DEBUG(" got focsed vars\n");
  return NULL;
}

























void Create_workingtable() {
	/*if (!task_list_current->holded_info->read->fread->worktables.worktable)
	task_list_current->holded_info->read->fread->worktables.worktable = malloc(sizeof(struct WORKTABLE));
	else{
		//struct WORKTABLES *last_wts
	}*/
}
