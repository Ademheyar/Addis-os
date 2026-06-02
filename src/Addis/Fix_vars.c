#include <Addis/Fix_vars.h>
#include <Addis/Readcode.h>
#include <Addis/Process.h>
#include <Addis/Libs/String/String.h>
#include <Addis/Drivers/Keyboard/Keyboard.h>
#include <Libs/Stdbool/Stdbool.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Tables/WorkingT.h>



void oprate_value() {
	DEBUG("in oprate value ");
	typedef struct {
		struct ROW *row[50];
		int rowid;
	} comteam;
	char *opretors = "", *dowhat = "";
	comteam first[50], second[50]; //, oldtvalue[100];
	int onto = 1, onor1 = -1, onor2 = -1;//, otv = -1; 
	DEBUG("\n\n\n");
	struct COLUMN *c = task_list_current->holded_info->read->fread->worktables.worktable->column;
	for (int cid = 0; c; cid++) {
		struct ROW *r = c->row;
		if(r && r->type){
			/*DEBUG("{");
			if(r->on) DEBUG("[%d]", r->on);
			if(r->type) DEBUG("%s...", r->type);
			if(r->on == 6 && r->value.variable.fuwdb && r->value.variable.fuwdb->name) DEBUG("|%s|", r->value.variable.fuwdb->name);
			else if(r->word) DEBUG("(%s)", r->word);
			DEBUG("}\n");*/
			if(r && !(r->type) || !c->focused_row) {
				DEBUG(" not given\n", dowhat);
				if (r->next_row && r->next_row->type) r = r->next_row;
				else break;
			}

			if (issame("OR", r->type)){
				if(onto == 1) { onor1++; first[onor1].rowid = 0; }
				else if(onto == 2) { onor2++; second[onor1].rowid = 0; }
			}
			else if (issame("OPERATOR", r->type)){ 
				if(issame("COMPARE", r->type)){
					dowhat = r->word;
					DEBUG("dowhat =	%s\n", dowhat);
				}
				else {
					opretors = r->word;
					DEBUG("OPERATOR =	%s\n", opretors);
				}
			}
			else if (issame("TO", r->type)){ // TO BY 
				onto = 2;	
				DEBUG("onto =	%d\n", onto);
			}
			else {
				for(int rid = 0; ; rid++) {
					if (onto == 1){
						if(onor1 == -1) { onor1 = 0; first[onor1].rowid = 0; }
						DEBUG("setting first[%d] c[%d]r[%d]", first[onor1].rowid, cid, rid);
						first[onor1].row[first[onor1].rowid] = r;
						first[onor1].rowid++;
					}
					else if(onto == 2) {
						if(onor2 == -1) { onor2 = 0; second[onor2].rowid = 0; }
						DEBUG("setting seconde[%d]c[%d]r[%d]", second[onor2].rowid, cid, rid);
						second[onor2].row[second[onor2].rowid] = r;
						second[onor2].rowid++;
					}
					
					DEBUG("{");
					if(r->on) DEBUG("[%d]", r->on);
					if(r->type) DEBUG("%s...", r->type);
					if(r->on == 6 && r->value.variable.fuwdb && r->value.variable.fuwdb->name) DEBUG("|%s|", r->value.variable.fuwdb->name);
					else if(r->word) DEBUG("(%s)", r->word);
					DEBUG("}\n");

					if (r->next_row && r->next_row->type) r = r->next_row;
					else break;
				}
			}
		}
		//DEBUG("going to next column\n");
		if (c->next_column) c = c->next_column;
		else break;
	}

	if (onor1 > -1 && onor2 > -1) {
		DEBUG("first{%d}", onor1);
		for (int fc = 0; fc <= onor1;){// this will count all first columns
			DEBUG("[%d].rowid{%d}", fc, first[fc].rowid);
			_Bool isdone = false;
			for (int fr = 0; first[fc].row[fr] && first[fc].row[fr]->type;) { // this will count in first table ands or rows
				DEBUG("[%d]<%s>", fr, first[fc].row[fr]->type);
				if (first[fc].row[fr]->word) DEBUG("(%s)", first[fc].row[fr]->word);
				DEBUG("\n");
				// cheack if first v is given or have to find it
				char f=0;
				if (iskeywordvaluetype(first[fc].row[fr]->type)) f = 'T'; // for test 
				if (iskeywordvaluestring(first[fc].row[fr]->type)) f = 'T'; // for test 
				if(iskeywordvaluelocater(first[fc].row[fr]->type)) f = 'T';
				if (iskeywordvaluenumber(first[fc].row[fr]->type)) f = 'N'; // for number
				if (iskeywordvarprop(first[fc].row[fr]->type)) f = 'P'; // for P
				if (iskeywordvartype(first[fc].row[fr]->type)) f = 'V'; // for variable
				if (f){
					DEBUG("second{%d}", onor2);
					for (int sc = 0; sc <= onor2;){ // this will count all second columns
						DEBUG("[%d].rowid{%d}\n", sc, second[sc].rowid);
						for (int sr = 0; second[sc].row[sr] && second[sc].row[sr]->type;) {// this will count in secound table ands or rows
							DEBUG("[%d]<%s> oprater(%s)", sr, second[sc].row[sr]->type, opretors);
							char s=0;
							if (iskeywordvaluestring(second[sc].row[sr]->type)) s = 'T'; // for test 
							if (iskeywordvaluelocater(second[sc].row[sr]->type)) s = 'T';
							if (iskeywordvaluenumber(second[sc].row[sr]->type)) s = 'N'; // for number
							if (iskeywordvaluetype(second[sc].row[sr]->type)) s = 'T'; // for test 
							if (iskeywordvarprop(second[sc].row[sr]->type)) s = 'P'; // for P
							if (iskeywordvartype(second[sc].row[sr]->type)) s = 'V'; // for variable
							if (s){
								if(fix_varprop(second[sc].row[sr]->value.variable.fuwdb, opretors, first[fc].row[fr])) {
									isdone = true;
								}
								DEBUG("\n");
								if(f == s) {
									// TODO: chack what kind of var is
									
									DEBUG(" %s And %s = ", first[fc].row[fr]->value.variable.fuwdb->name, second[sc].row[sr]->value.variable.fuwdb->name);
									Add_chiled_to_parent(second[sc].row[sr]->value.variable.fuwdb, first[fc].row[fr]->value.variable.fuwdb);
									
								}
								else if(issame(opretors, "COMPAR")){
									isdone = compar(opretors, first[fc].row[fr], second[sc].row[sr]);
								}
							}
							//if(boolnr == true) break; // if first comparing is true will go back or will chack if there is ors or other columns
							if(isdone && sr+1 < second[sc].rowid) sr++;
							else break;
						} // sr loop
						if(isdone || sc == onor2) break;
						else if (sc+1 <= onor2) sc++;
						else break;
					} // sc loop
				}
				if(isdone && fr+1 < first[fc].rowid) fr++;
				else break;
			} // fr loop
			if(isdone || fc == onor1) break;
			else if (fc+1 <= onor1) fc++;
			else break;
		} // fc loop
	}
	else {
		/////////////////////////////////////////////////////////////////////////////////////
		// this code will chack where will be the result saved
		/*struct READINGINFO *frtemp = task_list_current->holded_info->read->fread;
		int rfid = task_list_current->holded_info->read->fread->rfor_id;
		if (issame(task_list_current->holded_info->read->fread->reading_for[rfid], "DEFFING")){
			frtemp = task_list_current->holded_info->read->read_prev;
		}
		else {
			DEBUG(" HOHOHOHOHOHOOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOHO\n%s", task_list_current->holded_info->read->fread->reading_for[rfid]);	
		}
		/////////////////////////////////////////////////////////////////////////////////////
		if (otv > -1 && onor1 == -1) {
			for (int c = 0; c <= otv; c++){
				DEBUG("counting otv\n");
				for (int r = 0; r <= oldtvalue[c].rowid; r++) {// this will count in first table ands or columns
					if(!(oldtvalue[c].row->word && oldtvalue[c].row->type)) continue;
					DEBUG(" found var so old value with var will be oprated\n");
					// cheack if there is any value pointed
					//int wc = task_list_current->holded_info->read->fread->worktables.worktable->countcolumn;
					//int wr = task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row;
					DEBUG("fp table1[%d][%d<%d]<%s>(%s) %s pointer\n",
					c,r, oldtvalue[c].rowid, oldtvalue[c].row->type,  oldtvalue[c].row->value, opretors);
					DEBUG(" GOING TO %s table->%s = ", opretors, oldtvalue[c].row->type);
					if(issame(opretors, "ADD")) {
						if(issame(oldtvalue[c].row->type, "NAME")) {
							DEBUG(" %s ADD %s = ", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->name, oldtvalue[c].row->value);
							task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->name = stradd(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->name, oldtvalue[c].row->word, 0);
							DEBUG("%s", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->name);
						}
						if(issame(oldtvalue[c].row->type, "WITH")) {
							DEBUG(" %s ADD %s = ", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->withcodes, oldtvalue[c].row->value);
							task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->withcodes = stradd(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->withcodes, oldtvalue[c].row->word, 0);
							DEBUG("%s", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->withcodes);
						}
						if(issame(oldtvalue[c].row->type, "VALUE") || issame(oldtvalue[c].row->type, "DO") || issame(oldtvalue[c].row->type, "SHAPE")) {
							DEBUG(" %s ADD %s = ", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->value.word, oldtvalue[c].row->word);
							task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->value.word = stradd(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->value.word, oldtvalue[c].row->word, 0);
							task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->value.type = strdup(oldtvalue[c].row->type);
							DEBUG("%s", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.variable.fuwdb->value.word);
						}
						DEBUG("\n");		
					}
				}
			}
			//task_list_current->holded_info->read->fread = task_list_current->holded_info->read->read_prev;
			//Clear_worktables(0);
			//task_list_current->holded_info->read->fread = task_list_current->holded_info->read->read_next;
		}*/
	}
	DEBUG("out\n");
	Clear_worktables(-1);
}


















void read_fanctions(list_t *read) { // working on this
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	readword = readword;
	DEBUG("in read_fanction\n");\
	int wc = 0, wr = 0;
	for (int nc = 0;; nc++) {
		if(!(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word)) break;
		if (issame(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word, "NAME")){
			for(int nr = 1;; nr++){
				if (task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word){
					char *calledfancwith = "", *calledfancname = "";
					calledfancname = task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word;
					DEBUG(" getting on table wtid(0) countcolumn(%d) countrow(%d) fancname(%s) \n", nc, nr, calledfancname);
					// get calledfanc with if there is
					for (;; wc++) {
						if (task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->type && 
							  issame(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->type, "WITH")){
							if(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word && wr > 0){
							calledfancwith = task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word;
							break;
							}
							if (task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->value.istype) wr++;
							else wr = 1;
						}
					}
					
					DEBUG(" going to find fanction\n");
					//int wc = task_list_current->holded_info->read->fread->worktables.worktable->countcolumn++;
					struct USER_WDB *found = find_dt(task_list_current->holded_info->read->fread, "", "", -1, task_list_current->holded_info->fread->name, "", calledfancname, "");
					if (found) {
						//DEBUG(" ukvtid %d got(wdbc%dr%d)", wtid, wc, wr);
						// this will make all var will be created in this new code will be stord in it caller
						task_list_current->holded_info->read->fread = found;
						char *retfancvalue = stradd("", get_varinfo(0, "", 'D', "VALUE")->word, 0);
						char *with0 = stradd("", get_varinfo(0, "", 'D', "WITH")->word, 0);
						// VVVVVVVVVVVV to tall what info do we want TODO: find other way
						// save tabel and clear tabel to use for with code
						//Clear_worktables(0);
						char *retwith = get_witbmp_h(with0, calledfancwith);
						DEBUG("fancvalue = %s\n", retfancvalue);
						// done getting code info for reading fanction save the reast code
						char *readcode = "";
						DEBUG("got reading info fancname = %s\n", calledfancname);
						int bupbodysid = task_list_current->holded_info->read->fread->bodys->length;
						retfancvalue = read_bodys(retfancvalue);
						if(retwith && (int)strlen(retwith) > 5) 
						retwith = read_bodys(retwith); // reading all bodys () inside with
						DEBUG("fancwith = %s\n", retwith);
						DEBUG("fancvalue = %s\n", retfancvalue);
						DEBUG("going to read with and fanction code \n");
						readcode = stradd(retwith, retfancvalue, ' ');
						int rfid = ++task_list_current->holded_info->read->fread->rfor_id;
						task_list_current->holded_info->read->fread->reading_for[rfid] = "WDEF";
						// create new tabel next to this table 
						Create_readtable_bn();
						task_list_current->holded_info->read->read_next->fread->read_new = strdup(readcode);
						DEBUG(" unknown (%s)\n", task_list_current->holded_info->read->read_next->fread->read_new);
						// make new looping table
						task_list_current->holded_info->read->read_next->fread->rfor_id = 0;
						task_list_current->holded_info->read->read_next->fread->reading_for[0] = "DEFFING";
						if (!((int)task_list_current->holded_info->read->fread->bodys->length >= bupbodysid)) task_list_current->holded_info->read->fread->bodys->length = bupbodysid;
						//DEBUG("new code = %s \n", task_list_current->holded_info->read->freading[task_list_current->holded_info->read->fread_id]);
					}
					else {// TODO: if fanc NAme not fount ?
						DEBUG(" function Not found...\n");
						//task_list_current->holded_info->read->fread->worktables.worktable->countcolumn = wc;
						//task_list_current->holded_info->read->fread->reading_value = str_splitL(code, " ", 0);
						//task_list_current->holded_info->read->freading[task_list_current->holded_info->read->fread_id] = 0;
					}

				}
				else break;
			}
		}		
	}
	DEBUG("out from read fanction\n");
}
