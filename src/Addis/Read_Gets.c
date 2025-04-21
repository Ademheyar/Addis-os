//#include <Addis/Read_Gets.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>

char *get_value_type(char *value) {
  int len = 0, hasline = 0, hasspace = 0, hassymbol = 0, hasletter = 0, hasnum = 0, hassign = 0, hasdont = 0;
  for(; *(value + len); len++){
    if (*(value + len) == '\n') hasline = 1;
    else if (isspace(*(value + len))) hasspace = 1;
    else if (isgraphsymbol(*(value + len))) hassymbol = 1;
    else if (isalpha(*(value + len))) hasletter = 1;
    else if (isdigit(*(value + len))) hasnum = 1;
    if (*(value + len) == '+' || *(value + len) == '-') hassign = 1;
    if (*(value + len) == '.') hasdont = 1;
  }
  if (len == 0) return "";
  if (len == 1 && (hasletter == 1 || hassymbol == 1)) {
    if (hassymbol == 1) return "CHARACTER"; // get charcters
    else if (hasletter == 1) return "LETTER"; // get latters
    
  }
  else 
  {
    if(hasline == 1)  return "PARAGRAF";// get paragraf
    else 
    {
      if(hasspace == 1) return "SENTENCE"; // get sentens
      else 
      {
        if(hasletter == 1 && hasnum == 0 && hassign == 0 && hasdont == 0){
          if (hassymbol == 1) return "NAME"; // get name
          else return "WORD"; // get word
        }
        if(hasnum == 1)
        {
          if(hasletter == 1) {
            uint16_t a = strtohex(value);
            if(a != 0) return "HEX";
            else if(a == 0) return "NAME";
          }
          else
          {
            if(hassign == 0) return "NATURAL"; // get natural num
            if(hassign == 1){
              if(hasdont) return "RATIONAL"; // get rational num
              else return "WHOLE"; // get whole num
            }
          }
        }
      }
    }
  }
  return "";
}

























































































struct ROW *get_varinfo(int id, char *id_addrs, char from, char *getwhat){
	DEBUG("getting varinfo ");
	struct ROW *out = NULL;
	DEBUG("in %c get var GET(%s) ", from, getwhat);
  switch (from) {
    case 'B':
			DEBUG("id(%d) id_addrs(%s) name(-) ", id, id_addrs);
			if (id <= (int)task_list_current->holded_info->read->fread->bodys->length){
				listnode_t *body = list_get_node_by_index(task_list_current->holded_info->read->fread->bodys, id); 
				if(body->value && body->type){
					out = Convert_text(body->type, body->value);
					DEBUG("valusetype(%s)", out->type);
					//body->value = 0;
					//body->type = 0;
					break;
				}
				else{
					DEBUG("not found :(\n");
					DEBUG("main_readcode\n{\n%s\n}\n", task_list_current->holded_info->read->fread->main_readcode);
					char *rbcode = read_bodys(stradd(task_list_current->holded_info->read->fread->main_readcode, "", 0));//
					task_list_current->holded_info->read->fread->main_readcode = stradd(rbcode," ", 0);
					DEBUG("fixed Code(%s)....\n", rbcode);
					// splitting will not be nessasery for now
					//task_list_current->holded_info->read->fread->reading_value = str_splitL(stradd(rbcode," ", 0), " ", 0);
				}
			}
			else DEBUG(" id is out reang :(\n");
    break;
  }
  DEBUG("End getting\n");
	return out;
}











// find type or var specifide in code
char *get_vartype(list_t *read){
  struct ROW *gettype = malloc(sizeof(struct ROW));
  char *retype = "";
  DEBUG("getting var type ");
  while ((int)read->length){
		listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); 
		DEBUG("%d|%s|\n", task_list_current->holded_info->read->fread->reading_on, readword->value);
    // TODO: chack if it is type or NOT
    // TODO: if it't writen nearr find it
    gettype->word = strdup(readword->value);
    gettype->type = "STRING";
    //Add_Vale_TO_WTable(gettype); // adding found var type
    task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on); // removeing found var type from reading text
    DEBUG("variables |%s| ", readword->value);
    // if there is other types spacified
    if (issame(readword->value, "AND")) {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("geting other type\n");
      continue;
    }
    // if it is writen twice this will chack
    if(!issame(retype, "FANCTION")) retype = gettype->word;
    DEBUG("end getvartype()\n");
    break;
  }
  DEBUG("");
  return retype;
}

char *get_witbmp_h(char *with0, char *with1) {
  char *out="", *retcreatecodes="", *retsetchacodes="", *rettakes="", *newtake="";
  int j = 0, bupbodysid = task_list_current->holded_info->read->fread->bodys->length;
  DEBUG(" get_witbmp_h\n", with1);
  if (with1!=NULL && strlen(with1) > 5) {
    with1 = read_bodys(with1); // reading all with bodys ()
    DEBUG("1 with1 body = %s\n", with1);
		list_t *code = str_splitL(with1, " ", 0);
    char *gettake() {
      char *rettake="";
      /*for (task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.row = 0; task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value; task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.row++) {
				char *taketext = task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value;
				rettake = stradd(rettake, "TAKE", ' ');
				rettake = stradd(rettake, typetext(taketext), ' ');
				rettake = stradd(rettake, taketext, ' ');
				rettake = stradd(rettake, "END TAKE", ' ');
				DEBUG("got this value %s in var\n", rettake);
				return rettake;
      }*/
      return rettake;
    }

    char *conothers(int i) {
      char *rettake="";
      sysinfo.sys_id++;
      for(i =0;i < (int)code->length;i++){
				listnode_t *codeword = list_get_node_by_index(code, i); 
        DEBUG(" in conothers\n");
        if (iskeywordvartype(codeword->next->value)) {
          // Do we have to change main .ukvtid value ????
          read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "VARS", 0);
          rettake = gettake();
          if(iskeywordvartype(codeword->next->value)) { i++; i++;}
          if(!issame(codeword->value, "AND")) break;
        }
        else {
          read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "", 0);
          rettake = gettake();
          break;
        }
      }
      sysinfo.sys_id--;
      return rettake;
    }

    for(int i =0;i <= (int)code->length;i++){
			listnode_t *codeword = list_get_node_by_index(code, i); 
      if(issame(codeword->value, "CREATE")) {
        retcreatecodes = stradd(retcreatecodes, codeword->value, ' ');
        i++;

        while (1) {
          if (iskeywordvartype(codeword->value)) {
            retcreatecodes = stradd(retcreatecodes, codeword->value, ' ');
            i++;
          }

          if (issame(codeword->value, "TAKE")) {
            read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
            retcreatecodes = stradd(retcreatecodes, "TAKE NAME", ' ');
          //  retcreatecodes = stradd(retcreatecodes, task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value, ' ');
            retcreatecodes = stradd(retcreatecodes, "END TAKE", ' ');
          }

          if (issame(codeword->value, "AND")) {
            retcreatecodes = stradd(retcreatecodes, codeword->value, ' ');
            i++;
            continue;
          }
          break;
        }
        if(i != (int)code->length) i--;
      }

      else if(issame(codeword->value, "SET")) {
        retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
        DEBUG("getting Set %s\n", retsetchacodes);
        i++;
        read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
        //DEBUG("get retsetchacodes %s\n", task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value);
        retsetchacodes = stradd(retsetchacodes, "TAKE STRING", ' ');
        //retsetchacodes = stradd(retsetchacodes, task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value, ' ');
        retsetchacodes = stradd(retsetchacodes, "END TAKE", ' ');
        while (1) {
          if(issame(codeword->value, "TO")) {
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
          }
          if (iskeywordvartype(codeword->next->value)) {
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
            if (issame(codeword->value, "AND")) {
              retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
              i++;
              continue;
            }
            break;
          }
        }
        DEBUG(" got Set = %s\n", retsetchacodes);
        if(i != (int)code->length) i--;
      }

      else if(issame(codeword->value, "CHANGE")) {
        retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
        i++;
        while (1) {
          if (iskeywordvartype(codeword->next->value)) {
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
          }
          if (issame(codeword->value, "AND")) {
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
            continue;
          }
          if(issame(codeword->value, "TO")) {
            retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
            i++;
            read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
            ///DEBUG("get retsetchacodes %s\n", task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value);
            retsetchacodes = stradd(retsetchacodes, "TAKE STRING", ' ');
            //retsetchacodes = stradd(retsetchacodes, task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value, ' ');
            retsetchacodes = stradd(retsetchacodes, "END TAKE", ' ');
            break;
          }
        }
        if(i != (int)code->length) i--;
      }

      else {
        if(issame(codeword->value, "TAKE"))
        {
          read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
          //DEBUG("get rettakes %s\n", task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value);
          rettakes = stradd(rettakes, "TAKE STRING", ' ');
					//rettakes = stradd(rettakes, task_list_current->holded_info->read->fread->worktables.worktable[wtid].column.rowtask_list_current->holded_info->read->fread->worktables.worktable[wtid].column[cm].row->value, ' ');
          rettakes = stradd(rettakes, "END TAKE", ' ');
        }

        else if(issame(codeword->value, "GET")) {
          //task_list_current->holded_info->ukvtid = 1;
          char *ret = read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "", 0)->type;
          //task_list_current->holded_info->ukvtid = tid;
          DEBUG(" getting Get %s (+%s)\n", rettakes, ret);
          //retsetchacodes = stradd(retsetchacodes, codeword->value, ' ');
          // DEBUG("get retsetchacodes %s\n", task_list_current->holded_info->table[tid].column.rowtask_list_current->holded_info->table[tid].column.row->value);
          rettakes = stradd(rettakes, "TAKE STRING", ' ');
          rettakes = stradd(rettakes, ret, ' ');
          rettakes = stradd(rettakes, "END TAKE", ' ');
          DEBUG(" got Get = %s\n", rettakes);
          if(i != (int)code->length) i--;
        }

        else {
          char *t = conothers(i);
          if (!issame(t, "") && !issame(t, "NO~NE"))
          {
            rettakes = stradd(rettakes, t, '\n');
            if(i != (int)code->length) i--;
          }
        }
      }
    }
    free(&code);
  }

  DEBUG("1 with0 %s\n", with0);
  DEBUG("retcreatecodes %s\n", retcreatecodes);
  DEBUG("retsetchacodes %s\n", retsetchacodes);
  DEBUG("newtake %s\n", rettakes);
  DEBUG("out %s\n", out);
  if (strlen(rettakes) > 1 && strlen(with0) > 1) {
    splited_code *code1 = split_code(with0, ' ');
    splited_code *takesp = split_code(rettakes, '\n');
    for(int i =0;i <= code1->lines;i++){
      if (issame(code1->code_splited[i], "TAKE") && issame(code1->code_splited[i+1], "NAME")) {
        newtake = stradd(newtake, "CHANGE", ' ');
        newtake = stradd(newtake, code1->code_splited[i+2], ' ');
        newtake = stradd(newtake, "VALUE", ' ');
        newtake = stradd(newtake, "TO", ' ');
        if(j<=takesp->lines && strlen(takesp->code_splited[j]) > 5)
          newtake = stradd(newtake, takesp->code_splited[j], ' ');
        else newtake = stradd(newtake, "TAKE STRING END TAKE", ' ');
        j++;
      }
    }
    if(with0!=NULL) free(code1);
    if(newtake!=NULL) free(takesp);
  }
  DEBUG("2 with0 %s\n", with0);
  DEBUG("retcreatecodes %s\n", retcreatecodes);
  DEBUG("retsetchacodes %s\n", retsetchacodes);
  DEBUG("newtake %s\n", rettakes);
  DEBUG("out %s\n", out);
  if((int)strlen(with0) > 5) out = stradd(out, with0, 0);
  if((int)strlen(retcreatecodes) > 5) out = stradd(out, retcreatecodes, ' ');
  if((int)strlen(retsetchacodes) > 5) out = stradd(out, retsetchacodes, ' ');
  if((int)strlen(newtake) > 5) out = stradd(out, newtake, ' ');
  DEBUG("get with = %s\n", out);
  task_list_current->holded_info->read->fread->bodys->length = bupbodysid;
  return out;
}

uint32_t get_color(char *colorname){
  if(issame(colorname, "WHITE")) { DEBUG(" GOT %s COLOR\n", colorname); return COLOR_WHITE; }
  if(issame(colorname, "BLACK")) { DEBUG(" GOT %s COLOR\n", colorname); return COLOR_BLACK; }
  return -1;
}