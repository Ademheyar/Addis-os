#include <Addis/Read_Resivers.h>
#include <Addis/Libs/List/List.h> // for defining list_t, listnode_t, and other list-related functions
#include <Addis/Tables/WorkingT.h> // for defining struct USER_WDB and other table-related functions
#include <Addis/Read_Gets.h> // for defining get_value_type and other functions
#include <Addis/Fix_vars.h> // for defining fix_varprop and other functions
#include <Addis/Drivers/Screen/Vesa/Vesa.h> // for defining VESA_DRIVER and other functions
#include <Addis/Process.h> // for defining task_list_current and other process-related functions
#include <Addis/Readcode.h> // for defining convert and other readcode-related functions
#include <Addis/Libs/String/String.h> // for defining string-related functions
#include <Addis/Drivers/Screen/Vga/Vga.h> // for defining set_screen_color and other functions
#include <Addis/Interrupt/Isr.h> // for defining isr_ctx_t and other interrupt-related functions
#include <Addis/Drivers/Keyboard/Keyboard.h>
#include <Addis/Tables/DefT.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>

#include <Addis/Libs/String/Text.h>
#include <Addis/Drivers/Filse_system/Ata/Ata.h>
#include <Addis/Drivers/Filse_system/Fat/Fat.h>
#include <Addis/Drivers/Filse_system/hd_Driver.h>



struct ROW *Resive_from_Keyword(list_t *read, int isret)
{
	struct ROW *ret = NULL;
	//int hold = task_list_current->holded_info->read->fread->reading_on;
	//char *prev_type = ""; // for take
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	while(readword->value){
		//hold = task_list_current->holded_info->read->fread->reading_on;
		isret = isret? isret : isret+!(!ret); // this will chack if ret is true 

		if (issame(readword->value, "TABLE")) 
		{
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG(" table{%s}", readword->value);
      if (issame(readword->value, "0")) {
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				get_worktable(read, 0);
			}
			else if (issame(readword->value, "1")) {
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      	get_table(read, 0);
			}
			DEBUG("DONE table{%d}", task_list_current->holded_info->read->fread->reading_on);
      break;
    }

		// when codes starts here
    else if(issame(readword->value, "DRIVER")){
      DEBUG("in %s ", readword->value);
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
      readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      if (issame(readword->value, "0")){ // BIOS INFO GETING
        DEBUG("%s ", readword->value);
      }
      else if (issame(readword->value, "1")){ // EXPCEPTION
        DEBUG("%s ", readword->value);
      }
      else if (issame(readword->value, "2")){ // INTERRUPT
        DEBUG("%s ", readword->value);
      }
      else if (issame(readword->value, "3")){ // VGA SCREEN DISPLAY
        DEBUG("%s ", readword->value);
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
        ret = VESA_DRIVER(read, isret);
      }
      else if (issame(readword->value, "4")){ // Keybord
        DEBUG("in %s\n", readword->value);
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
        Keyboard_Driver(read);
      }
      
      else if (issame(readword->value, "5")){ // Hard_disk
        DEBUG("in %s\n", readword->value);
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
        hd_Driver(read);
      }
      return ret;
    }

		else if(issame(readword->value, "TAKE")) {
			convertto.integer = -1;
			convert(readword->next->value);
			task_list_current->holded_info->read->fread->reading_on = task_list_current->holded_info->read->fread->reading_stoped+=2;
			DEBUG("reading[%d]|%s[%s]| bodys[%d]~[%d] ", task_list_current->holded_info->read->fread->reading_on, readword->value, readword->next->value, (int)task_list_current->holded_info->read->fread->bodys->length, convertto.integer);
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			if (convertto.integer != -1) {
				struct ROW *gotvalue = get_varinfo(convertto.integer, "", 'B', "");
				if(gotvalue->type && gotvalue->value.istype){
					DEBUG("Found Take[%d]<%s> Return is (%d) ", convertto.integer, gotvalue->type, isret);
					// chack if ret is NULL or not and is gotvalue isret or not
					if(isret == 1) {
						if (!ret) ret = gotvalue;
						return ret;
						//TODO: else
					}
					
					else{
						//TODO: if AND OR HOW IT WILL SAVE
						//if(issame(prev_type, "") || issame(prev_type, gotvalue->type))
						Addrow_to_row_byindex(gotvalue, 0, 0);
						//else Addrow_to_row_byindex(gotvalue, 0, 1);
						free(gotvalue);
					}
				}
				if (readword->value && issame(readword->value, "AND")) {
					DEBUG("AND\n");
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					//prev_type = strdup(gotvalue->type);
					//prev_type = "";
				}
				else if (readword->value && issame(readword->value, "OR")) {
					//prev_type = "";
					DEBUG("OR\n");
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				}
				else break;
				continue;
			}
		}
		else break;
	}
	return ret;
}


struct ROW *Resive(list_t *read, int isgroup, int isret)
{
	struct ROW *ret = NULL, *wordtype = NULL;
	int hold = task_list_current->holded_info->read->fread->reading_on;
	//char *prev_type = ""; // for take
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	while(readword->value){
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		DEBUG("in read resicers resiving[%s]\n", readword->value);
		hold = task_list_current->holded_info->read->fread->reading_on;
		isret = isret? isret : isret+!(!ret); // this will chack if ret is true 
		
		struct ROW *keyrow = Resive_from_Keyword(read, isret);
    if (keyrow){
			ret = keyrow;
			hold = task_list_current->holded_info->read->fread->reading_on;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		}
		else if (isKeyword(readword->value))
		{
			if(isret) 
			{
				task_list_current->holded_info->read->fread->reading_on = task_list_current->holded_info->read->fread->reading_stoped = hold;
			}
			
			DEBUG("word is ketword reading_on %d \n", task_list_current->holded_info->read->fread->reading_on);	
			// chack if ret is NULL or not and isret or not
			return NULL;
		}
		
		wordtype = Convert_text("", readword->value); 
		if (wordtype && wordtype->type) DEBUG(" out type %s\n", wordtype->type);
		
		DEBUG("1 reading|%s| ", readword->value);
		if (isgroup == 1 && (wordtype && wordtype->type || ret))
		{
			struct ROW *g = wordtype;
			if (ret) g = ret;
			else 
			{
				DEBUG("ret not found so using wordtype\n");
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			}
			
			struct ROW *math = Read_Math(read, g, isret);
			if (math){
				DEBUG("MAths found \n");
				ret = math;
				hold = task_list_current->holded_info->read->fread->reading_on;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			}
			else {
				if (!ret)
				{
					task_list_current->holded_info->read->fread->reading_on = --task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				}
			}
		}

		DEBUG("3 reading|%s| ", readword->value);
		if ((ret && !isret || !ret) && (!wordtype || wordtype && wordtype->on == 2))
		{
			struct ROW *foundrow = get_as_row(readword->value);
			if (foundrow) {
				struct ROW *copyrow = malloc(sizeof(struct ROW));
				copy_row(copyrow, foundrow);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				DEBUG("found in row As |%s| ", readword->value);
				// chack if ret is NULL or not and if foundrow
				if (isret == 0) {
					copyrow->As = NULL;
					Addrow_to_row_byindex(copyrow, 0, 0);
					free(copyrow);
					hold = task_list_current->holded_info->read->fread->reading_on;
					break;
				}
				else return foundrow;
			}
			task_list_current->holded_info->read->fread->reading_on = hold;

			if(get_defco(read, isret)){
				hold = task_list_current->holded_info->read->fread->reading_on;
				if(!ret) return NULL;
				// chack  if ret is NUll or not  
			}
			task_list_current->holded_info->read->fread->reading_on = hold;

			struct ROW *found = get_vars(read, isret);
			if (found) {
				// chack if ret is NULL or not and if found isret or not 
				if (ret) 
				{
					DEBUG("don gatting name1 id %d var \n", task_list_current->holded_info->read->fread->reading_on);	
					//task_list_current->holded_info->read->fread->reading_on = hold;
				}
				DEBUG("don gatting name2 id %d var \n", task_list_current->holded_info->read->fread->reading_on);	
				///else task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				ret = found;
				continue;
			}
			task_list_current->holded_info->read->fread->reading_on = hold;
			
			
		}
		
		if(ret) {
			if(isret) return ret;
			DEBUG("----%s\n", readword->value);
			struct ROW *n = malloc(sizeof(struct ROW));
			n = Convert_text("", readword->value);
			ret = marge_row(ret, n);
			free(n);
			break;
		}
		else 
		{
			DEBUG("%d|%s\n", task_list_current->holded_info->read->fread->reading_on, readword->value);
			if(wordtype && isret) {
				ret = wordtype;
				continue;
			}
		}


		do
		{
			DEBUG("length[%d] - on[%d]\n", (int)read->length, task_list_current->holded_info->read->fread->reading_on);
			if((int)read->length  - task_list_current->holded_info->read->fread->reading_on <= 1) return ret;
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			//DEBUG("value = %s\n", readword->value);
		}
		while(issame(readword->value, "") || issame(readword->value, " "));

		if (issame(readword->value, "AND")) {
			DEBUG("AND\n");
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		}
		else if (issame(readword->value, "OR")) {
			DEBUG("OR\n");
			task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		}
		else break;
		DEBUG("\n");
	}
	return ret;
}































struct ROW *read_unknownvars(list_t *read, char *read_what, int ret_resualt){
  DEBUG("In read_unknownvars\n");
  struct ROW *ret = malloc(sizeof(struct ROW));
	ret->type = NULL;
	int on = task_list_current->holded_info->read->fread->reading_on;
  for(;task_list_current->holded_info->read->fread->reading_on < (int)read->length-1; task_list_current->holded_info->read->fread->reading_on++){
		listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
  	DEBUG("reading unknownvars %d|%s|\n", on, readword->value);
		ret->type = NULL;
		if(iskeywordvartype(readword->value)){
			if(issame(read_what, "TYPES") || issame(read_what, "") || issame(read_what, "ALL")){
				ret->type = get_vartype(read);
				DEBUG("");
				return ret;
    	}
		}
    else if(iskeywordvartype(readword->next->value) || issame(readword->value, "TABLE")) {/* TODO :  add chacking if task_list_current->holded_info->read->fread->reading_on is var*/
			if(issame(read_what, "VARS") || issame(read_what, "") || issame(read_what, "ALL")){
				
				DEBUG("");
				return ret;
			}
			ret->type = "Readen";
		}
		/*else if(issame(readword->value, "TAKE")){
			if(issame(read_what, "") || issame(read_what, "ALL") || issame(readword->value, read_what) && issame(read_what, "TAKE")) {
				ret = get_take(read, ret_resualt);
				if (!ret->type || !issame(ret->type, "NONE")) {
					DEBUG("take not found\n");
					return ret;
				}
			}
			DEBUG("take found\n");
			task_list_current->holded_info->read->fread->reading_on++;
			ret->type = "Readen";
		}*/
		
		else if(issame(readword->value, "TRUE") || issame(readword->value, "FALSE")) {
			if(issame(read_what, "") || issame(read_what, "ALL")){
				//if (issame(readword->value, "TRUE")) task_list_current->holded_info->read->fread->ret.booln = true;
				//else task_list_current->holded_info->read->fread->ret.booln = false;
				struct ROW *gettype = malloc(sizeof(struct ROW));
				gettype->word = stradd("", readword->value, 0);
				gettype->type = "STRING";
				//Add_Vale_TO_WTable(gettype);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				ret->type = readword->value;
				DEBUG("");
				return ret;
			}
			ret->type = "Readen";
		}
    
		else if(issame(read_what, "")){ // read other
      ret = ReadResivers(read, "", "", 'G', ret_resualt); 
      break;
    }
		if(!ret->type || (int)task_list_current->holded_info->read->fread->reading_value->length <= task_list_current->holded_info->read->fread->reading_on){ 
			DEBUG(",%s", readword->value); 
			task_list_current->holded_info->read->fread->reading_on = on;
			break; } // this will break if needed info not given
  }
  DEBUG("\n");
  return ret;
}

struct ROW *ReadResivers(list_t *read, char *read_what, char *dowhat, char from, int isret){
  DEBUG("In Read_Resivers %s , %s, %c \n", read_what, dowhat, from);
	struct ROW *ret = NULL;
  for(;task_list_current->holded_info->read->fread->reading_on < (int)read->length;task_list_current->holded_info->read->fread->reading_on++){
		listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
  	DEBUG("reading[%d] |%s|\n", task_list_current->holded_info->read->fread->reading_stoped, readword->value);

    if(issame(readword->value, "READ")){
      DEBUG("Reading Code \"READ\"\n");
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			char *gstate = "";
			if(iskeywordvarpos(readword->value)){
				gstate = strdup(readword->value);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      	DEBUG("gstate %s\n", gstate);
			}
			int isnexton = 0; // this will chack if next is given so 0 is not given but will read 1 is given and will be readen 2 given but will not be readen
			if(issame(readword->value, "NEXT")) {
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				isnexton = task_list_current->holded_info->read->fread->rnextcode + 1;
				DEBUG(" rnextcode %d isnexton %d\n", task_list_current->holded_info->read->fread->rnextcode, isnexton);
			}
			/*if(issame(readword->value, "BREAK")) {
				DEBUG(" \"BREAK\"\n");
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				if(task_list_current->holded_info->read->fread->loopon==true){
					DEBUG(" loopon\n");
						//task_list_current->holded_info->read->fread->reading_value = str_splitL(code, " ", 0);
						//task_list_current->holded_info->read->freading[task_list_current->holded_info->read->fread_id] = "";
						//task_list_current->holded_info->read->fread_id -= 1;
						task_list_current->holded_info->read->fread->Break=true;
				}
			}

			// CONTINUE code
			if(issame(readword->value, "CONTINUE")) {
				DEBUG(" \"CONTINUE\"\n");
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				if(task_list_current->holded_info->read->fread->loopon==true){
					DEBUG(" loopon\n");
					//task_list_current->holded_info->read->fread->reading_value = str_splitL(code, " ", 0);
					//task_list_current->holded_info->read->freading[task_list_current->holded_info->read->fread_id] = "";
				}
			}*/
			//STOP
			//CONTINUE
			//BREAK
			//AGEAN

			if(issame(readword->value, "DO")) {
				DEBUG("Reading Code \"%s\"\n", readword->value);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				
				char *dos = "";
				for (int i = 0; task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row; i++){
					// DEBUG("c i = %d\n", i);
					if(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->type
						&& issame(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->type, "DO")){
						for (int j = 1; task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row; j++){
							// DEBUG("r j = %d\n", j);
							if(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word){
								if(issame(dos, ""))
								dos = strdup(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word);
								else
								dos = stradd(dos, task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word, ' ');
							}
						}
						break;
					}
				}
				if (issame(dos, "")) {
					dos = read_unknownvars(read, "TAKE", 0)->type;
					if (issame(dos, "")) {
						task_list_current->holded_info->read->fread->rnextcode = 0;
						return NULL;	
					}
				}
				if (isnexton < 2) {
					int rfid = ++task_list_current->holded_info->read->fread->rfor_id;
					task_list_current->holded_info->read->fread->reading_for[rfid] = "WDEF";
					// create new tabel next to this table 
					Create_readtable_bn();
					task_list_current->holded_info->read->read_next->fread->read_new = strdup(dos);
					DEBUG(" unknown var(%s)(%s)\n", readword->value, task_list_current->holded_info->read->read_next->fread->read_new);
					// make new looping table
					task_list_current->holded_info->read->read_next->fread->rfor_id = 0;
					task_list_current->holded_info->read->read_next->fread->reading_for[0] = "DEFFING";

					task_list_current->holded_info->read->fread->rnextcode = 1;
				}
				else return NULL;
			}
			else if(issame(readword->value, "LOOP")) {
				DEBUG("Reading Code \"%s\"\n", readword->value);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				char *loopcode = "";
				for (int i = 0; task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word; i++){
					//DEBUG("c i = %d\n", i);
					if(issame(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word, "LOOP")){
						for (int j = 1; task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row || task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word; j++){
						//DEBUG("r j = %d\n", j);
						if(issame(loopcode, ""))
						loopcode = strdup(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word);
						else
						loopcode = stradd(loopcode,  task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row->word, ' ');
						}
						break;
					}
				}
				if (issame(loopcode, "")) {
					loopcode = read_unknownvars(read, "TAKE", 0)->type;
					if (issame(loopcode, "")) {
						task_list_current->holded_info->read->fread->rnextcode = 0;
						return NULL;	
					}
				}
				if (isnexton < 2) {
					int rfid = ++task_list_current->holded_info->read->fread->rfor_id;
					task_list_current->holded_info->read->fread->reading_for[rfid] = "WLOOP";
					task_list_current->holded_info->read->read_prev->fread->read_loopoint = task_list_current->holded_info->read->read_prev->fread->reading_on;
					// create new tabel next to this table 
					Create_readtable_bn();
					task_list_current->holded_info->read->read_next->fread->read_new = strdup(loopcode);
					DEBUG(" unknown var(%s)(%s)\n", readword->value, task_list_current->holded_info->read->read_next->fread->read_new);

					// make new looping table
					task_list_current->holded_info->read->read_next->fread->rfor_id = 0;
					task_list_current->holded_info->read->read_next->fread->reading_for[0] = "LOOPING";

					task_list_current->holded_info->read->fread->rnextcode = 1;
				}
				else return NULL;
			}
			
			else if(iskeywordvaluetype(readword->value)) {
				DEBUG("Reading Code \"%s\"\n", readword->value);
				char *type = strdup(readword->value);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				if(iskeywordvarpos(readword->value)){
					gstate = strdup(readword->value);
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				}
				int isname = 0, iswith = 0;
				char *name0 = "", *name1 = "";
				Clear_worktables(0);
				if(issame(type, "IMAGE") || issame(type, "TEXT")){
					DEBUG(" Going to read %s\n", type);
					for(isname = 0;isname < 2; isname++){
						struct ROW *r = Resive(read, 1, 1);
						if(issame(name0, "")) name0 = strdup(r->word);
						else name1 = strdup(r->word);
					}
				}
				DEBUG(" Got read value\n");

				if(issame(type, "IMAGE") || issame(type, "TEXT")){
					DEBUG(" Got %s name0 = %s name1 = %s\n", gstate, name0, name1);
					if (!issame(name0, "")){
						list_t *r = str_splitL(stradd(name0," ", 0), " ", 0);
						read->on = task_list_current->holded_info->read->fread->reading_on;
						task_list_current->holded_info->read->fread->reading_on = 0;
						struct USER_WDB *found = Get_main_dt(task_list_current->holded_info->read->fread, r);
						task_list_current->holded_info->read->fread->reading_on = read->on;
						if (found) {
							uint8_t *buffer = found->value.value.hex.hexs_8;
							// this will make all var will be created in this new code will be stord in it caller
							Create_vartable(read, NULL, 0);
							DEBUG(" Going to read %s\n", type);
							if(buffer){
								if(found){
									if(issame(type, "IMAGE")) {
										if(!(issame(name1, ""))) found->name = strdup(name1);
										else found->name = "IMAGE";
										found->value.value.image.header = (bmp_header_t*)buffer;
										found->value.value.image.bmp = (uint32_t*)(((uint8_t*)buffer) + found->value.value.image.header->offset);
										found->value.type = "IMAGE";
										found->type = "IMAGE";
									}
									else if(issame(type, "TEXT")){
										if(!(issame(name1, ""))) found->name = strdup(name1);
										else found->name = "TEXT";
										found->value.value.hex.hexs_8 = buffer;
										found->value.word = (char *)buffer;
										found->value.type = "TEXT";
										found->type = "TEXT";
									}
									DEBUG(" Saved %s As %s\n", found->value.type, found->name);
								}
								else {
									//task_list_current->holded_info->read->fread->worktables.worktable->countcolumn = wc;
									//DEBUG("worktablec[%d]r[%d] not saved ", wc);
								}
								DEBUG("\n");
								//DEBUG(" trying to draw image\n");
      					//gfx_blit_v(&screen_info, 1, 1, 1024, 768, found->value.hexs_32);
							}
						}
						else {
							//task_list_current->holded_info->read->fread->worktables.worktable->countcolumn = wc;
						}
					}
				}
				else if(issame(type, "FANCTION")){
						if(isname + iswith == 0 && task_list_current->holded_info->read->read_prev){
						for (int c = 0; task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word; c++) {
							if(!(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word && 
								task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type)) 
							continue;
							if(issame(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word, "NAME") ||
								issame(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word, "WITH"))
							{
								int tc = 0;//++task_list_current->holded_info->read->read_prev->fread->worktables.worktable->countcolumn;
								DEBUG("copying c %d to c %d\n", c, tc);
								for(int r = 0; task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row && task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type; r++){
									
									if(issame(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word, "NAME")) isname = 1;
									if(issame(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word, "WITH")) iswith = 1;
									
									task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type = strdup(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type);
									task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word = strdup(task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type);
									DEBUG("r %d t %s v %s TO t %s v %s ", r, task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type, task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word, task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->type, task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word);
									task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word = "";
									task_list_current->holded_info->read->read_prev->fread->worktables.worktable->focused_column->row->word = "";
								}
							}
						}	
					}
					if(isname == 0){
						read_unknownvars(read, "TAKE", 0); // Get name on countcolumn 1
					}

					if(iswith == 0){
						read_unknownvars(read, "WITH", 0); // Get WITH on countcolumn 1
					}
					DEBUG("got all info\n");
					read_fanctions(read);
					task_list_current->holded_info->read->fread->rnextcode = 1;
				}
			}

      return NULL;
    }

    else if(issame(readword->value, "INPUT")){
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
			
      DEBUG("\nin INPUT on = %d line = %d ", task_list_current->holded_info->read->fread->reading_on, (int)read->length);
      read_unknownvars(read, "TAKE", 0);
      //char *rettake = stradd("", task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->word, 0);
      //task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->word = NULL;
      //convert(rettake);
      uint8_t hexa = convertto.hexa_8;
      //DEBUG("rettake = %s hx = 0x%x int=%d ", rettake, hexa, hexa);
      READ_DATA_PORT(hexa);
      //DEBUG("end INPUT on = |%x|%s| \n", task_list_current->holded_info->read->fread->ret.integer_8, task_list_current->holded_info->read->fread->ret.text);
      struct ROW *gettype = malloc(sizeof(struct ROW));
      //gettype->value = stradd("", task_list_current->holded_info->read->fread->ret.text, 0);
      gettype->type = "STRING";
      //Add_Vale_TO_WTable(gettype);
      return NULL;
    }

    else if(issame(readword->value, "GET")){
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      //Clear_worktables("");
			if (issame(readword->value, "TABLE")) {
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				/*struct READINGINFO *frtemp = task_list_current->holded_info->read->fread;
				int rfid = task_list_current->holded_info->read->fread->rfor_id;
				if (issame(task_list_current->holded_info->read->fread->reading_for[rfid], "D")){
					frtemp = task_list_current->holded_info->read->read_prev;
				}*/
				// the first num give will detormin which table to use
				// 0 mains working table and 1 mains saveing table
				DEBUG("in get table{%s} ", readword->value);
				if (issame(readword->value, "0")) {
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					int wtsid = -1, wtcid = -1, wtrid = -1;
					for(int w = 0; w <= 3; w++)
					{
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						convertto.integer = -1;
						convert(readword->value);
						char *q = readword->value;
						if (!(*q >= '0' && *q <= '9')){
							DEBUG("going to get \"%s\"\n", readword->value);
							if (issame(readword->value, "GET")) 
							{	
            		DEBUG("in get other get ");
								DEBUG("table[0][%d][%d][%d]\n", wtsid, wtcid, wtrid);
								char *got = ReadResivers(read, "", "", 'R', 1)->word;
            		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
            		convert(got);
            		DEBUG("in get back[%s] ", got);
								DEBUG("table[0][%d][%d][%d]\n", wtsid, wtcid, wtrid);
							}
							else 
							{
					
								char *prop = readword->value; 
								task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
								readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						   	if(wtsid != -1) 
								{
									if(wtcid != -1) 
									{
										if(wtrid != -1) 
										{
											if (issame(prop, "VALUE")) 
											{	
												// /frtemp->fread->worktables.worktable[wtsid].focused_column->row;
											}
										}
										else 
												DEBUG("getting %s all table0 index found [%d][%d][%d]\n", prop, wtsid, wtcid, wtrid);
										{
											if (issame(prop, "INDEX")  || issame(readword->value, "INDEX")) 
											{	
												char text[10];
												if (issame(prop, "NEXT")){
													task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
													readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
													//sprintf(text,"%d", frtemp->fread->worktables.worktable[wtsid].focused_column->row);
												}
												else{
													//sprintf(text,"%d", frtemp->fread->worktables.worktable[wtsid].focused_column->row);
												}
												DEBUG("thered index[%s]\n", text);
												return NULL; //return stradd(text);
											}
										}
									}
									else 
									{
										if (issame(prop, "INDEX")  || issame(readword->value, "INDEX")) 
										{	
											char text[10];
											if (issame(prop, "NEXT")){
												task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
												readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
												//sprintf(text,"%d", frtemp->fread->worktables.worktable[wtsid].countcolumn+1);
											}
											else{
												//sprintf(text,"%d", frtemp->fread->worktables.worktable[wtsid].countcolumn);
											}
											DEBUG("second index[%s]\n", text);
											return NULL; // return stradd(text);
										}
									}
								}
								else 
								{
									if (issame(prop, "INDEX")  || issame(readword->value, "INDEX")) 
									{	
										char text[10];
										if (issame(prop, "NEXT")){
											task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
											readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
											//sprintf(text,"%d", frtemp->fread->worktables.counttable+1);
										}
										else{
											//sprintf(text,"%d", frtemp->fread->worktables.counttable);
										}
										DEBUG("first index[%s]\n", text);
										return NULL; // return stradd(text);
									}
								}
							}
						}
						else task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						if (convertto.integer > -1){
							if(wtsid == -1) wtsid = convertto.integer;
							else if(wtcid == -1) wtcid = convertto.integer;
							else if(wtrid == -1) wtrid = convertto.integer;
							DEBUG("saveing one value[%d] table[0][%d][%d][%d]\n", convertto.integer, wtsid, wtcid, wtrid);
						}
					}
				}
				else if (issame(readword->value, "1")) {
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					DEBUG("R createing  %s ", readword->value);
					if (issame(readword->value, "NEW")){
						DEBUG("new \n");
						task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						Create_vartable(read, NULL, 0);
						list_allin_table();
						DEBUG(" next word %s\n", readword->value);
						return NULL;
					}
				}
				else // or if index was't given
				{
					while(1) 
					{
						DEBUG(" ", readword->value);
						if (issame(readword->value, "VALUE")){
							task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
							readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
							//char *t = task_list_current->holded_info->read->fread->worktables.worktable[task_list_current->holded_info->read->fread->worktables.counttable].focused_column->rowsinfo.fRwdb->fread->frw].value;
							//DEBUG("(%s) ",t);
							//DEBUG(" wtid %d cm %d ", wtid, cm);
							//task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->word = strdup(t);
						}
						else if (issame(readword->value, "X")){
							
						}
						
						if (issame(readword->value, "AND")){
							task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
							readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
							continue;
						}
						else break;
					}
				}
				DEBUG("\n%s doen geting TABLE \n");
				return NULL;
			}
			//else 
			if (issame(readword->value, "DRIVER")){
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				if(issame(readword->value, "0")) {
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					if (issame(readword->value, "SCREEN")){
						DEBUG(" going to get Screen ");
						task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						while(1) {
							char *word = strdup(readword->value);
							DEBUG(" ", readword->value);
							struct ROW *rowvalue = malloc(sizeof(struct ROW));
							task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
							readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
							if (issame(word, "WIDTH")){
								char sx[100];
								sprintf(sx,"%d", screen_info.width);
								rowvalue->type = "WIDTH";
								rowvalue->word = sx;
								Addrow_to_row_byindex(rowvalue, 0, 0);
								/*if(issame(readword->value, "VALUE")) {
									task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
									readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
								}*/
							}
							else if (issame(word, "HEIGHT")){
								char sx[100];
								sprintf(sx,"%d", screen_info.height);
								rowvalue->type = "HEIGHT";
								rowvalue->word = sx;
								Addrow_to_row_byindex(rowvalue, 0, 0);
								/*if(issame(readword->value, "VALUE")) {
									task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
									readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
								}*/
							}
							else if (issame(word, "X")){
								char sx[100];
								sprintf(sx,"%d", screen_info.x);
								rowvalue->type = "X";
								rowvalue->word = sx;
								Addrow_to_row_byindex(rowvalue, 0, 0);
								/*if(issame(readword->value, "VALUE")) {
									task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
									readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
								}*/
							}
							else if (issame(word, "Y")){
								char sx[100];
								sprintf(sx,"%d", screen_info.y);
								rowvalue->type = "Y";
								rowvalue->word = sx;
								Addrow_to_row_byindex(rowvalue, 0, 0);
								/*if(issame(readword->value, "VALUE")) {
									task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
									readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
								}*/
							}
							else if (issame(word, "FG")){

							}
							else if (issame(word, "BG")){

							}
							else {
								//task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->type = stradd("SCREEN");
							}

							if (issame(readword->value, "AND")){
								task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
								readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
								continue;
							}
							else break;
						}
						DEBUG("\ndoen geting screen \n");
					}
				}
				return NULL;
			}
			else if (issame(readword->value, "RETURN")) {
				//task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->word = strdup(readword->value);
				task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
				readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
				if (task_list_current->holded_info->read->fread->reading_on != (int)read->length-1 &&
					(issame(readword->value, "AND") || issame(readword->value, "OR"))) {
					//task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->type = strdup(readword->value);
					task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					continue;
				}
				while (true) {
					if((task_list_current->holded_info->read->fread->reading_on != (int)read->length-1 || !(task_list_current->holded_info->read->fread->reading_on <= -1)) && iskeywordvartype(readword->value)) {
						struct ROW *gettype = malloc(sizeof(struct ROW));
						gettype->word = stradd("", readword->value, 0);
						gettype->type = "STRING";
						//Add_Vale_TO_WTable(gettype);
						task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						if (issame(readword->value, "AND") || issame(readword->value, "OR")) {
							//if (issame(readword->value, "OR"))
							//task_list_current->holded_info->read->fread->worktables.worktable[wtid].countcolumn+=1;
							//if (issame(readword->value, "AND"))
						task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						continue;
						}
					}
					break;
				}
			}
      DEBUG(" done reading GET\n");
			if(isret == 1) return ret;
      else return NULL;
    }
		else {
			DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			if(!get_defco(read, 0))// if codeword is not known chack if it is deffeand
			{
				// if code not deffend save and report 
				if (issame(read_what, "") && issame(dowhat, "")) {
					DEBUG("going to save unknown var 1\n");
					// this will chack if it gives a value
					struct ROW *ret = read_unknownvars(read, "ALL", 0);
					readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
					DEBUG("going to save unknown var 2 %s\n", readword->value);
					if(!ret || ret->type && issame(ret->type, "NONE")){
						//task_list_current->holded_info->read->fread->erorr_id++;
						//DEBUG("gong to save var 3\n");
						//if (task_list_current->holded_info->read->fread->erorr[task_list_current->holded_info->read->fread->erorr_id].erorr)
						//	task_list_current->holded_info->read->fread->erorr[task_list_current->holded_info->read->fread->erorr_id].erorr =  stradd(task_list_current->holded_info->read->fread->erorr[task_list_current->holded_info->read->fread->erorr_id].erorr, readword->value, ',');
						//else task_list_current->holded_info->read->fread->erorr[task_list_current->holded_info->read->fread->erorr_id].erorr =  stradd("", readword->value, ',');
						//task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
						//DEBUG(" The code or word[%d](%s) not knowen|%s|\n", task_list_current->holded_info->read->fread->reading_on, readword->value, task_list_current->holded_info->read->fread->erorr[task_list_current->holded_info->read->fread->erorr_id].erorr);
						//readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
						// TODO: show erorr as msg
					}
					else return NULL;
				}
			}	
			DEBUG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
			return NULL;
		}
  }
  DEBUG("");
 return NULL;
}
