#include <Addis/Readcode.h>
#include <Addis/Process.h>
#include <Addis/Libs/String/String.h>
#include <Addis/Libs/List/List.h>
#include <Addis/Tables/WorkingT.h>
#include <Addis/Tables/DefT.h>
#include <Addis/Libs/type/type.h>
#include <Kernel.h>
#include <Addis/Libs/String/Text.h>

DRIVERS drivers[256];

char *read_bodys(char text[]){
  // TODO: make the compilare know if gruop of code not closed
	//DEBUG("IN READ body text{\n%s\n%s}\n", text,);
	typedef struct {
			char *coped;
			char *cwhat;
			char *vtype;
			_Bool copy;
	} Copylists;
	Copylists copylists[10];
  int prevcid = 0, countdef=0;
	unsigned int *iu = 0;
  int countwords = 0;
  char *newtext = "";
  copylists[prevcid].copy = false;
  _Bool commenton=false;
	task_list_current->holded_info->read->fread->bodys = list_create();
	list_t *ret_list = str_splitL(text, " ", iu);
  //DEBUG("\n\n");
	for(countwords = 0; countwords < (int)ret_list->length; countwords++) {
		listnode_t *word = list_get_node_by_index(ret_list, countwords);
    //DEBUG("l = %d countwords = %d iscopy = %d, prevcid =%d  commenton %d word %s ", (int)ret_list->length, countwords, copylists[prevcid].copy, prevcid, commenton, word->value);
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
		

		if(commenton == false && (countdef == 0 && issame(word->value, "TAKE") || issame(word->value, "DEFINE") ) || 
			 countdef == 0 && (commenton == false && issame(word->value, "TAKE") || issame(word->value, "COMMENT"))){
			if (issame(word->value, "DEFINE")) 
			{
				//DEBUG("GETTING DEFINE ");
				if (countdef == 0) {
					if(!sysinfo.main_def)
					{
						//DEBUG("NEXT PARENT\n");
						sysinfo.focused_def = sysinfo.last_def = sysinfo.main_def = malloc(sizeof(struct DEF_LIST));
						sysinfo.focused_def->main_chaild_def = sysinfo.focused_def->prev_def = NULL;
						sysinfo.focused_def->next_def = sysinfo.focused_def->parent = NULL;
						sysinfo.focused_def->name[0] = NULL;
					}
					else if(!sysinfo.last_def->next_def) 
					{
						//DEBUG("NEXT PARENT\n");
						sysinfo.last_def->next_def = malloc(sizeof(struct DEF_LIST));
						sysinfo.focused_def = sysinfo.last_def->next_def;
						sysinfo.focused_def->prev_def = sysinfo.last_def;
						sysinfo.focused_def->next_def = sysinfo.focused_def->parent = NULL;
						sysinfo.focused_def->main_chaild_def = NULL;
						sysinfo.last_def = sysinfo.focused_def;
						sysinfo.focused_def->name[0] = NULL;
					}
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
			else {
				if(copylists[prevcid].copy == true) prevcid++;
				if (issame(word->value, "COMMENT")) commenton = true;
				if(issame(word->value, "TAKE")){
					//TODO: what if commented in Take OR any where
					//TODO: IS IT nessesary to write x type next to TAKE, create,
					countwords++; // this will make it pass the type
					copylists[prevcid].vtype = strdup(word->next->value);
				}
				else copylists[prevcid].vtype = "";
				
				copylists[prevcid].cwhat = strdup(word->value);
				copylists[prevcid].coped = "";
				copylists[prevcid].copy = true;
				//DEBUG("copying started by = %s , prevcid = %d\n", copylists[prevcid].cwhat, prevcid);
			}
			continue;
		}
		
		if(copylists[prevcid].copy == true){
			if (commenton == true && issame(word->value, "END") && issame(word->next->value, "COMMENT")) {
				//DEBUG("going to %s %s \n", word->value, word->next->value);
				countwords++;
				if(prevcid>0) {
					prevcid--;
					if(!issame(copylists[prevcid].cwhat, "COMMENT")) copylists[prevcid].copy = false;
				}
				else{
					commenton = false;
					copylists[prevcid].coped = " ";
				}
				continue;
			}
			else if(commenton == true) continue;

			if(issame(word->value, "END") &&
				(issame(word->next->value, copylists[prevcid].cwhat) || issame(word->next->value, copylists[prevcid-1].cwhat))){
				
				//DEBUG("closeing %s\n",  word->next->value);
				countwords++; 
				
				char text[100];
				list_insert_back(task_list_current->holded_info->read->fread->bodys, copylists[prevcid].vtype, copylists[prevcid].coped);
				sprintf(text,"%d", (int)task_list_current->holded_info->read->fread->bodys->length-1);
				//listnode_t *body = list_get_node_by_index(task_list_current->holded_info->read->fread->bodys, (int)task_list_current->holded_info->read->fread->bodys->length-1);
				//if (body->value) DEBUG("coped on bodyemp %d(%s) = (%s)%s\n", (int)task_list_current->holded_info->read->fread->bodys->length, (char *)text, body->type, body->value);	
				

				
				if(prevcid>0){
					prevcid--;
					copylists[prevcid].coped = stradd(copylists[prevcid].coped, copylists[prevcid+1].cwhat, ' ');
					copylists[prevcid].coped = stradd(copylists[prevcid].coped, (char *)text, ' ');
					//DEBUG("saving chiled(prevcid(%d)(%s on body(%s))) \nhas %s \nin parant %s\n", prevcid, copylists[prevcid+1].cwhat, (char *)text, task_list_current->holded_info->read->fread->bodys[bodysid].value, copylists[prevcid].coped);
				}
				else{
					newtext = stradd(newtext, copylists[prevcid].cwhat, ' ');
					newtext = stradd(newtext, (char *)text, ' ');
					//DEBUG("done getting group(%s) \n\n", newtext);
					copylists[prevcid].copy = false;
					copylists[prevcid].coped = " ";
				}
				continue;
			}
			else {
				//DEBUG("copyting %s\n", copylists[prevcid].coped);
				if(issame(word->prev->prev->value, "TAKE") && !issame(copylists[prevcid].vtype, "")) 
				copylists[prevcid].coped = stradd(copylists[prevcid].coped,  word->value, 0);
				else copylists[prevcid].coped = stradd(copylists[prevcid].coped,  word->value, ' ');
				//DEBUG("copyed %s\n", copylists[prevcid].coped);
				continue;
			}
		}
		else {
			if(!issame(word->value, "") || issame(word->value, " ")){
				if(issame(newtext, "")) newtext = strdup(word->value);
				else newtext = stradd(newtext, word->value, ' ');
				//DEBUG("ADDing others(%s) on place = %s\n", word->value, newtext);
			}
		}
  }
	//while(1);
  //DEBUG("\n\n");
	//DEBUG("DoneDoneDoneDoneDoneDoneDone\n");
  //DEBUG("body ltext = %s\n" , 's');
	//DEBUG("Done read_bodys{\n%s\n}\n", newtext);
  return newtext;
}

void *get_pathlists(char *paths) {
	splited_code *bootinfo =  malloc(sizeof(splited_code) + 520);;
	bootinfo->lines = -1;
	char *text0 = read_text(paths);
	if (text0 == NULL) return bootinfo;
	//DEBUG("mainpaths {\n%s\n}\n", text0);
	list_t *mainpath = str_splitL(stradd(text0, "", '\n'), "\n", 0); // spliting each line
	for (int i = 0; i < (int)mainpath->length; i++) {
		//DEBUG("Geting path lists\n");
		listnode_t *node0 = list_get_node_by_index(mainpath, i); 
		char *text1 = read_text(node0->value);
		if (text1 == NULL) continue;
		//DEBUG(" subtext %s\n", text1);
		list_t *subpath = str_splitL(stradd(text1, "", '\n'), "\n", 0); // spliting each line
		for (int j = 0; j < (int)subpath->length; j++) {
			DEBUG(" j = %d length = %d\n", j, (int)subpath->length);
			// reading all info one by one
			listnode_t *node1 = list_get_node_by_index(subpath, j); 
			// geting name and its path if there is?
			list_t *nandp = str_splitL(node1->value, "=", 0); // spliting name and path
			if(nandp->length == 2){
				listnode_t *name = list_get_node_by_index(nandp, 0);
				listnode_t *path = list_get_node_by_index(nandp, 1);
				bootinfo->lines+=2;
				bootinfo->code_splited1[bootinfo->lines-1] = strdup(name->value);
				bootinfo->code_splited1[bootinfo->lines] = strdup(path->value);
				
				DEBUG("name|%s| & path|%s|\n", bootinfo->code_splited1[bootinfo->lines-1], bootinfo->code_splited1[bootinfo->lines]);
			}
		}
	}
	DEBUG(" Geting path lists\n");
	return bootinfo;
}

// this will get and remove comments and other useless wordes symboles
char *get_code(char *filepath){
	DEBUG("going to get code from file %s\n", filepath);
	char *text1 = read_text(filepath);
	if (text1 == NULL) return NULL;
	int counttext = strlen(text1);
	char out[counttext];
	DEBUG("gottext|%s|%d|\n", text1, counttext);
  	int i = 0, j = 0;
	for (; *(text1 + i); i++){
		if((*(text1 + i+1))=='\0')  out[j+1] = '\0'; 
		if((*(text1 + i))=='\n') { out[j] = ' '; j++; continue; }
		if (*(text1 + i) == '\t') continue;
		if (isprint(*(text1 + i))) 
		{
			out[j] = *(text1 + i);
			j++;
		}
	}
	out[j] = '\0';
	char *text2 = malloc(strlen(out) + 1);
	strcpy(text2, out);
	//DEBUG("out from get_code = %s\n", text2);
	DEBUG("chacked coed = %s\n >>>>>>>out from get_code\n", text2);
	return text2; // strdup(out);
}











































char *onspace="";

void READ_DATA_PORT(uint8_t pic_irq){
  pic_irq =pic_irq;
	//task_list_current->holded_info->read->fread->ret.integer_8 = inp(pic_irq);
  //char text[20];
  //sprintf(text,"0x%x", task_list_current->holded_info->read->fread->ret.integer_8);
  //task_list_current->holded_info->read->fread->ret.text = stradd("", (char *)text, 0);
}

void register_driver(uint8_t pic_irq, char *name, char *handler, char *retbin) {
  // TODO : chack if pic_irq is not there
  DEBUG(" registering driver by name(%s) handler(%s)\n", name, handler);
  DEBUG(" 0 register_driver = %s id = %d port = %x\n", name, pic_irq, pic_irq);
	// adding IRQ_BASE
  //pic_irq = IRQ_BASE + pic_irq;
	drivers[pic_irq].irq_ack = stradd(stradd("ACKNOWLEDGE_DRIVER TAKE BINARY", retbin, ' '), "END TAKE", ' ');
 // drivers[pic_irq].driv_id = pic_irq - IRQ_BASE; // saveing 
  drivers[pic_irq].driv_port = pic_irq; // saveing 
  drivers[pic_irq].driv_name = name;
	drivers[pic_irq].main_name = name;
  drivers[pic_irq].driv_enabled = false;
  drivers[pic_irq].driv_registered = true;
  drivers[pic_irq].driv_handler = handler;
  drivers[pic_irq].onwdb = task_list_current->holded_info->fread;
  DEBUG(" 1 register_driver = %s handler = %s id = %d port = %x\n", drivers[pic_irq].onwdb->name, drivers[pic_irq].driv_handler, drivers[pic_irq].driv_id, drivers[pic_irq].driv_port);
}

void enable_driver(uint8_t pic_irq){
	// TODO : chack if pic_irq is not registered
	// it with out adding IRQ_BASE
  //pic_irq = IRQ_BASE + pic_irq;
  drivers[pic_irq].driv_enabled = true;
  DEBUG(" enabling driver with id(%d) port(%x)\n", drivers[pic_irq].driv_id, drivers[pic_irq].driv_port);
  //irq_enable(drivers[pic_irq].driv_id);
  DEBUG(" enabled driver with id(%d) port(%x)\n", pic_irq, pic_irq);
}

/*C program to split string by space into words.*/
void *split_code(char str1[], char split_by){
  //char code_splited[1000][100]; // TODO : can we use array to split
  //char *text="";
	//DEBUG("going to split_code by(%c) %s\n", split_by, str1);
  splited_code *code_splited = malloc(sizeof(splited_code));
  int j=0, ctr=0;
  for(unsigned int i=0;i<=(strlen(str1));i++)
  {
      // if space or NULL found, assign NULL into newString[ctr]
      if(str1[i]==split_by|| str1[i]=='\0')
      {
        if(j!=0){
          code_splited->code_splited[ctr][j]='\0';
          //text = stradd(text, code_splited[ctr], '\0');
					//DEBUG(" %s\n", code_splited->code_splited[ctr]);
          ctr++;  /* for next word  */ j=0; //for next word, init index to 0
        }
      }
      else
      {
          code_splited->code_splited[ctr][j]=str1[i];
          j++;
      }
  }

	//DEBUG("");
  code_splited->lines = ctr;
  return code_splited;
}
