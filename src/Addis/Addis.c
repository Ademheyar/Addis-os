/**
 * @file Addis.c
 * @brief This file contains the implementation of the Addis reading function.
 *
 * The Addis reading function is responsible for initializing and managing
 * the reading process within the Addis operating system.
 * It handles the reading of code, including
 * processing keywords, handling events, and managing the reading state.
 * The function also includes error handling and debugging information.
 * @author Addis Team
 * @date 2023-10-01
 * @version 1.0
 * @note This file is part of the Addis operating system.
 *       It is licensed under the GNU General Public License v3.0.
 *       See the LICENSE file for more details.
 * @warning This file is for educational purposes only.
 *          Use at your own risk. The authors are not responsible for any damages
 *          caused by the use of this code.
 */

#include <Addis/Read_Do.h>

// Function to read and process the code.
// This function is called when the reading process is initiated.
// It handles the reading of keywords, events, and other commands.
// It also manages the state of the reading process and updates the task list accordingly.
// It is important to note that this function is part of a larger system
// and relies on other components for its functionality.
// The function is designed to be flexible and extensible,
// allowing for future enhancements and modifications.
// It is recommended to follow best practices for coding and documentation
// when working with this function and its associated components.
void ReadDo(){
  //this will set reading to where it stoped
	task_list_current->holded_info->read->fread->reading_on = task_list_current->holded_info->read->fread->reading_stoped;
  // and we will read from there
  //DEBUG("reading on %d\n", task_list_current->holded_info->read->fread->reading_on);
	listnode_t *readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
	DEBUG("in ReadDo %d", task_list_current->holded_info->read->fread->reading_on);
  // chack if there is any code to read or if it is empty else preant the keyword
  if(readword && !readword->value || readword->value && issame(readword->value, "")) {
		task_list_current->holded_info->read->fread->reading_on = task_list_current->holded_info->read->fread->reading_stoped+=1;
	  DEBUG("|-|\n");
		return;
	}
  else DEBUG("|%s|\n", readword->value);
  // holde where it is reading 
  int hold_on = task_list_current->holded_info->read->fread->reading_on;
  
  // Starting from here We will read keywords the semple ones will be diffined in this code
  // and the rest will be in the there respective files

  // If Keyword is "INCLUDE"
  // this will be used to include the file that are found in path form or string and read it
  if(issame(readword->value, "INCLUDE")) {
    // Going to include the file To Read that is found Src/Addis/Libs/INCLUDE
    // and read it
	  task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
    struct ROW *isInclude_readed = READ_INCLUDE(task_list_current->holded_info->read->fread->reading_value);
    if (isInclude_readed){}
    return;
  }

  Resive(task_list_current->holded_info->read->fread->reading_value, 1, 0);

	// chack if this word is KEYWORD or most be risive
  if(hold_on == task_list_current->holded_info->read->fread->reading_on)
  {
    

	  DEBUG("reading KEYWORD |%s|\n", readword->value);
    if(issame(readword->value, "WHEN")) {
      _event(task_list_current->holded_info->read->fread->reading_value);
      return;
    }
    
    if(issame(readword->value, "GET")) {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("GOING TO GET\n");
      // get what type
      
      // get as if there is
      // TODO: AND UESES FOR NEW ADD AND 
      while(issame(readword->value, "AS")) 
      {
        DEBUG("getting variable AS\n");
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
        struct ROW *row = Resive(task_list_current->holded_info->read->fread->reading_value, 1, 1);
        if(row)
        {
          task_list_current->holded_info->read->fread->getas = strdup(row->word);
          free(row);

          if (issame(readword->value, "AND")) 
          {
            task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
            readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
            continue;
          }
        }
        break;
      }

      int rfid = ++task_list_current->holded_info->read->fread->rfor_id;
      task_list_current->holded_info->read->fread->reading_for[rfid] = "GETTING";
      DEBUG("name(%s)\n", task_list_current->holded_info->read->fread->name);
      DEBUG("Done going to get\n");
      return;
    }
    
    else if(issame(readword->value, "OPERATE") || issame(readword->value, "CASE")){
      DEBUG("in %s\n", readword->value);
      char *word = strdup(readword->value);
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
      struct ROW *rowvalue = malloc(sizeof(struct ROW));
      rowvalue->type = "OPERATOR";
      rowvalue->word = readword->value;
      if(issame(word, "CASE")) {
        rowvalue->word = word;
        Addrow_to_row_byindex(rowvalue, 0, 0);
        char *var = read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0)->type;
        if (issame(var, "")) {
          DEBUG(" case not found\n");
        }
        else {
          rowvalue->type = "WORD";
          rowvalue->word = var;
          DEBUG(" case found %s\n", var);
          Addrow_to_row_byindex(rowvalue, 0, 0);
        }
      }
      else Addrow_to_row_byindex(rowvalue, 0, 0); // tell where it will add on lat table 0
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("\nDone adding new working table going to oprate_value\n");
      oprate_value();
      return;
    }
    // if code not readen in here will try in Selection_Logic
    else {
      DEBUG("0>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
      ReadResivers(task_list_current->holded_info->read->fread->reading_value, "", "", 'R', 0);
      DEBUG("end OUT from other reading in sant to Selection_Logic\n");
      DEBUG("-<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      return;
    }
    DEBUG("\n out from line read \n");
  }
  
  
  
  
  
  
  
  
  
  // other skiping continuing and returns file
  

  

  // other skiping continuing and returns file
  if(issame(readword->value, "RETURN")) {
    DEBUG(" RETURNING valuse\n");
    task_list_current->holded_info->read->fread->reading_on =  ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
    //char *ret = read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "", 0);
    //task_list_current->holded_info->read->fread->ret.text = ret; // it will return goted text
    //task_list_current->holded_info->read->fread->returned = task_list_current->holded_info->read->fread->worktables; // it will return the working tabel
    /// add other that can be returned here
		//task_list_current->holded_info->read->fread->reading_value = str_splitL(code, " ", 0);
    //task_list_current->holded_info->read->freading[task_list_current->holded_info->read->fread_id] = " ";
    DEBUG(" RETURNED\n");
    return;
  }
  
  
  // divise refistering  codes starts here
  else if(issame(readword->value, "REGISTER_DRIVER")){
    task_list_current->holded_info->read->fread->reading_on =  ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
    DEBUG(" in REGISTER_DRIVER on = %d line = %d  \n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    Clear_worktables(0);
    /*char *name=" ", *handler="";
    uint8_t hexa=0;
    read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
    read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "WITH", 0);
    //task_list_current->holded_info->table[0] = &task_list_current->holded_info->table[0];
    char *rettake = stradd("", task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->value, 0);
    DEBUG(" rettake(%s)\n", rettake);
    if(thisvar("HEXA", rettake)) {
      convert(rettake);
      hexa = convertto.hexa_8;
      DEBUG(" hexa = 0x%x int = %d\n", hexa, hexa);
    }
    name = task_list_current->holded_info->fread->name;
    handler = stradd("", task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row1].value, 0);
    register_driver(hexa, name, handler, rettake);
    drivers[hexa].onwdb = task_list_current->holded_info->fread;
    DEBUG(" end REGISTER_DRIVER on = %d line = %d\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    */return;
  }
  // enavleing drivers codes starts here
  else if(issame(readword->value, "ENABLE_DRIVER")){
    task_list_current->holded_info->read->fread->reading_on =  ++task_list_current->holded_info->read->fread->reading_stoped;
					readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
    DEBUG(" in ENABLE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length, readword->value);
    Clear_worktables(0);
    read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
    /*char *rettake = stradd("", task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->value, 0);
    convert(rettake);
    uint8_t hexa = convertto.hexa_8;
    DEBUG(" rettake(%s) hx(0x%x) int(%d)\n", rettake, hexa, hexa);
    enable_driver(hexa);
    DEBUG(" ENABLE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length, readword->value);
    DEBUG(" end ENABLE_DRIVER on = %d line = %d\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    */return;
  }
  // enavleing drivers codes starts here
  else if(issame(readword->value, "ACKNOWLEDGE_DRIVER")){
    task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
    DEBUG(" in ACKNOWLEDGE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length, readword->value);
    Clear_worktables(0);
    read_unknownvars(task_list_current->holded_info->read->fread->reading_value, "TAKE", 0);
    /*char *rettake = stradd("", task_list_current->holded_info->read->fread->worktables.worktable[wtid].focused_column->row->value, 0);
    convert(rettake);
    uint8_t hexa = convertto.hexa_8;
    DEBUG(" rettake(%s) hx(0x%x) int(%d)\n", rettake, hexa, hexa);
    pic_acknowledge(hexa);
    DEBUG(" ACKNOWLEDGE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length, readword->value);
    DEBUG(" end ACKNOWLEDGE_DRIVER on = %d line = %d\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    */return;
  }
  // output codes starts here
  else if(issame(readword->value, "OUTPUT")){
    DEBUG("\nin OUTPUT on = %d line = %d ", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    // clears all previus list codes
    task_list_current->holded_info->read->fread->reading_on =  ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on);
    //task_list_current->holded_info->read->fread->ret.retable.column[task_list_current->holded_info->read->fread->ret.retable.countcolumn].row=0;
    DEBUG("getting info ");
    //char *rettake = ReadResivers(task_list_current->holded_info->read->fread->reading_value, "TAKE", "GET", 'R');
   // convert(rettake);
    //DEBUG("rettake = %s ", rettake);
    uint8_t hexa = convertto.hexa_16;
    DEBUG("rettake = %s hx = 0x%x int=%d ", "rettake", hexa, hexa);
    READ_DATA_PORT(hexa);
    DEBUG("end OUTPUT on = %d line = %d\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_value->length);
    return;
  }
  DEBUG("");
}