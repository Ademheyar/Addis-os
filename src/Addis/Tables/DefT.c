#include <Addis/Tables/DefT.h>
#include <Addis/Process.h>
#include <Kernel.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Tables/WorkingT.h> // for defining struct READINGINFO and other table-related functions

SYSTEMINFO sysinfo;

void read_sub_def()
{
	DEBUG("going to Chack if main has sub chiled\n");
	if(sysinfo.focused_def->non_def && !issame(sysinfo.focused_def->non_def, "")){
		//DEBUG("found non_def sub chiled{\n%s\n}\n", sysinfo.focused_def->non_def);
		unsigned int *iu = 0;
  	int countwords = 0, countdef=0;
  	//char *newtext = "";
		list_t *ret_list = str_splitL(sysinfo.focused_def->non_def, " ", iu);
		for(countwords = 0; countwords < (int)ret_list->length; countwords++) {
			listnode_t *word = list_get_node_by_index(ret_list, countwords);
			//DEBUG("l = %d countwords = %d countdef = %d word %s ", (int)ret_list->length, countwords, countdef, word->value);
			//if(word->next && word->next->value) DEBUG("next %s\n", word->next->value);
			//else DEBUG("next ----\n");
			
			if(countdef){
				if (issame(word->value, "END") && issame(word->next->value, "DEFINE")) {
					countwords++; // for define
					countdef--;
					if(countdef != 0){
						sysinfo.focused_def->non_def = stradd(sysinfo.focused_def->non_def, "END DEFINE", ' ');	
						continue;
					}
					//DEBUG("ending define\n");
					//DEBUG("done create [%d] name[%d](%s) value(%s)\n", countdef, sysinfo.focused_def->name_id, sysinfo.focused_def->name[sysinfo.focused_def->name_id], sysinfo.focused_def->value);
					sysinfo.focused_def = sysinfo.focused_def->parent;
					//DEBUG("ending define\n");
				}
				else{
					if(issame(word->value, "DEFINE")) countdef++;
					if(countdef == 1) {
						sysinfo.focused_def->value = stradd(sysinfo.focused_def->value, word->value, ' ');
						//DEBUG("sysinfo.focused_def->value(%s)\n", sysinfo.focused_def->value);
					}
					else {
						sysinfo.focused_def->non_def = stradd(sysinfo.focused_def->non_def, word->value, ' ');
						//DEBUG("sysinfo.focused_def->non_def(%s)\n", sysinfo.focused_def->non_def);
					}
				}
				continue;
			}

			if(countdef == 0 && issame(word->value, "DEFINE"))
			{
				if(!sysinfo.focused_def->main_chaild_def)
				{
					//DEBUG("NEW CHAILED\n");
					sysinfo.focused_def->last_chaild_def = sysinfo.focused_def->main_chaild_def = malloc(sizeof(struct DEF_LIST));
					sysinfo.focused_def->last_chaild_def->parent = sysinfo.focused_def;
					sysinfo.focused_def->last_chaild_def->prev_def = NULL;
					sysinfo.focused_def->last_chaild_def->next_def = NULL;
					sysinfo.focused_def->last_chaild_def->name[0] = NULL;
					sysinfo.focused_def = sysinfo.focused_def->last_chaild_def;
				}
				else if(!sysinfo.focused_def->last_chaild_def->next_def) 
				{
					//DEBUG("NEXT CHAILED\n");
					sysinfo.focused_def->last_chaild_def->next_def = malloc(sizeof(struct DEF_LIST));
					sysinfo.focused_def->last_chaild_def->next_def->parent = sysinfo.focused_def;
					sysinfo.focused_def->last_chaild_def->next_def->prev_def = sysinfo.focused_def->last_chaild_def;
					sysinfo.focused_def->last_chaild_def->next_def->next_def = NULL;
					sysinfo.focused_def->last_chaild_def->next_def->name[0] = NULL;
					sysinfo.focused_def->last_chaild_def = sysinfo.focused_def->last_chaild_def->next_def;
					sysinfo.focused_def = sysinfo.focused_def->last_chaild_def;
					
				}

				sysinfo.focused_def->value = "";
				sysinfo.focused_def->non_def = "";
				while(1){
					if(!sysinfo.focused_def->name[0]) sysinfo.focused_def->name_id = 0;
					else ++sysinfo.focused_def->name_id;
					do
					{
						if (word->next){
							word = word->next;
							countwords++;
						} 
						else break;
					} while(issame(word->value, "") && word == word->next);
					
					//if(sysinfo.focused_def->parent) DEBUG("parent name[0](%s) ", sysinfo.focused_def->parent->name[0]);
					sysinfo.focused_def->name[sysinfo.focused_def->name_id] = strdup(word->value); // get name
					sysinfo.focused_def->name[sysinfo.focused_def->name_id+1] = NULL;
					//DEBUG(" name[%d](%s)\n", sysinfo.focused_def->name_id, sysinfo.focused_def->name[sysinfo.focused_def->name_id]);
					if (issame(word->next->value, "AND")) { countwords++; word = word->next; continue; }
					else break;
				}
				countdef++;
			}
			//sysinfo.focused_def->focused_chaild_def = NULL;
		} // loop
		sysinfo.focused_def->non_def = "";
		//DEBUG("ending define\n");
		//while(1);
	}	
  //DEBUG("\n\n");
}

_Bool chack_def_names(struct DEF_LIST *deft, char *name)
{
	for (int n = 0; deft->name[n]; n++) {
		//DEBUG("name[%d]%s == (%s)\n", n, deft->name[n], name);
		if (issame(deft->name[n], name) && deft->value){
			//DEBUG(" define name[%d]|%s|(%s)\n", n, deft->name[n], deft->value);
			return 1; // TRUE
		}
	}
	return 0;
}

void find_defc(char *name) {
	if(sysinfo.focused_def->last_chaild_def && sysinfo.focused_def->main_chaild_def) {
		sysinfo.focused_def->focused_chaild_def = sysinfo.focused_def->main_chaild_def;
		while(sysinfo.focused_def->focused_chaild_def) {
			// chack if in same name
			if (chack_def_names(sysinfo.focused_def->focused_chaild_def, name)) return;
			if(sysinfo.focused_def->focused_chaild_def->next_def) sysinfo.focused_def->focused_chaild_def = sysinfo.focused_def->focused_chaild_def->next_def;
			else break;
		}
	}
	sysinfo.focused_def->focused_chaild_def = NULL;
}

void find_defl(char *name) {
	if(sysinfo.last_def && sysinfo.main_def) {
		sysinfo.focused_def = sysinfo.main_def;
		while(sysinfo.focused_def) {
			// chack if in same name
			if (chack_def_names(sysinfo.focused_def, name)) return;
			if(sysinfo.focused_def->next_def) sysinfo.focused_def = sysinfo.focused_def->next_def;
			else break;
		}
	}
	sysinfo.focused_def = NULL;
}



















_Bool get_defco(list_t *read, int isret)
{
	char *name = "", *values = "";
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	DEBUG("Find(%s)isret[%d] \n", readword->value, isret);
	sysinfo.focused_def = NULL;
	find_defl(readword->value);
	if (sysinfo.focused_def){
		if(isret >= 1) return true;
		task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		name = strdup(sysinfo.focused_def->name[sysinfo.focused_def->name_id]);
		if(sysinfo.focused_def->value) values = strdup(sysinfo.focused_def->value); 
		DEBUG("main(%d) name(%s) found value (%s)\n", sysinfo.focused_def->name_id, name, values);
		read_sub_def(); // this will chack if main has  sub def or not and is it deffined
		DEBUG("going to search chaild (%s)\n", readword->value);
		// chack if next valu also deff chailed
		sysinfo.focused_def->focused_chaild_def = NULL;
		find_defc(readword->value);
		// TODO: chack if there is chiled of chiled
		if (sysinfo.focused_def->focused_chaild_def){
			name = strdup(sysinfo.focused_def->focused_chaild_def->name[sysinfo.focused_def->focused_chaild_def->name_id]);
			values = stradd(values, sysinfo.focused_def->focused_chaild_def->value, ' ');
			DEBUG(" chaild(%d) name(%s) found value (%s)\n", sysinfo.focused_def->focused_chaild_def->name_id, name, values);	
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		}

		if(values) {
			// save this tabel as gatting mode
			int rfid = ++task_list_current->holded_info->read->fread->rfor_id;
			task_list_current->holded_info->read->fread->reading_for[rfid] = "WDEF";
			Create_readtable_bn(); // create new tabel next to this table
			task_list_current->holded_info->read->read_next->fread->read_new = strdup(values);
			task_list_current->holded_info->read->read_next->fread->name = strdup(name);
			DEBUG("unknown var(%s)(%s)\n", task_list_current->holded_info->read->read_next->fread->name, 
			task_list_current->holded_info->read->read_next->fread->read_new);
			DEBUG("from(%s)(%s)\n", task_list_current->holded_info->read->read_next->read_prev->fread->name, 
			task_list_current->holded_info->read->read_next->read_prev->fread->main_readcode);

			// make new table a giver
			task_list_current->holded_info->read->read_next->fread->rfor_id = 0;
			task_list_current->holded_info->read->read_next->fread->reading_for[0] = "DEFFING";
			DEBUG(" stoped reading_for[0] = %s\n", task_list_current->holded_info->read->read_next->fread->reading_for[0]);
			return true;
		}
	}
	return false;
}