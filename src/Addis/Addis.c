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
#include <Addis/Readcode.h> // for defining read_bodys and other functions
#include <Addis/Process.h> // for defining task_list_current and other process-related functions
#include <Addis/Libs/String/String.h> // for defining issame and other string-related functions
#include <Addis/Drivers/Screen/Vga/Vga.h> // for defining set_screen_color and other functions
#include <Addis/Interrupt/Isr.h> // for defining isr_ctx_t and other interrupt-related functions
#include <Addis/Libs/List/List.h> // for defining list_t, listnode_t, and other list-related functions
#include <Addis/Tables/WorkingT.h> // for defining struct USER_WDB and other table-related functions
#include <Addis/Read_Gets.h> // for defining get_value_type and other functions
#include <Addis/Fix_vars.h> // for defining fix_varprop and other functions
#include <Addis/Drivers/Screen/Vesa/Vesa.h> // for defining VESA_DRIVER and other functions
#include <Addis/Tables/DefT.h> // for defining table types and other related
#include <Kernel.h> // for defining Kernel related functions and variables
#include <Addis/Libs/INCLUDE/INCLUDE.h>

#include <Addis/Libs/String/Text.h>

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
struct WORKTABLE *ReadDo(){
  //this will set reading to where it stoped
	task_list_current->holded_info->read->fread->reading_on = task_list_current->holded_info->read->fread->reading_stoped;
  // and we will read from there
  //DEBUG("reading on %d\n", task_list_current->holded_info->read->fread->reading_on);
	listnode_t *readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
	DEBUG("in ReadDo %d", task_list_current->holded_info->read->fread->reading_on);
  
  // holde where it is reading 
  int hold_on = task_list_current->holded_info->read->fread->reading_on;
  
  // Starting from here We will read keywords the semple ones will be diffined in this code
  // and the rest will be in the there respective files

  
  // Check if it is CoMmAnD or NORMAL code
  // chack if there is any code to read or if it is empty else preant the keyword
  if(readword && readword->value && strstr(readword->value, "->") != NULL && strstr(readword->value, "CoMmAnD->") != NULL)
  {
    // this will be used to handle the command code
    // and do what it says to do
    char **split_list = str_split(readword->value, '-');
    char *command = split_list[0];
    for (int i = 1; split_list[i] != NULL; i++) {
      DEBUG("pinting command(%s)  i(%d) value(%s)\n", command, i, split_list[i]);
    }
    // FILE HANDLING
    // this will be used to handle the file command code
    if (split_list[1] && split_list[1] != NULL && issame(split_list[1], ">Files")){
      DEBUG("command(%s) value(File)\n", command);
      
      // FILE LOADING WILL BE HANDLED BY THIS
      // this will be used to load the file that is found in path form or string
      if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">Load")){
        // TODO: this will be used to load the file that is found in path form or string and read it
        // TODO: What if it is ask to get the path from valurble or somthin else this will be used to handle it and get the path from it and then read the file

        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_stoped); 
        char *path = readword->value;
        DEBUG("command(%s) value(Load) Path(%s)\n", command, path);
        if (strchr(path, '\\') || strchr(path, '/')) {
          // Here we will Load a File that is found in path form and read it
          // TODO: this will be used the file that are found in path form and Holde it 
          // TODO: It has to Read and File from here and holde the buffer and the info of the file in the reading info to be used in the
          char *Buffer = get_code(path);
          DEBUG("Done getting code from file\n");
          if (Buffer) {
            // if it get the path then we will jump the path text and continue readintg the rest of the code and then we will read the file and save it in the reading info to be used in the reading code
            task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
            
            
            if (!task_list_current->holded_info->read->fread->worktables.worktable) {
              DEBUG("creating worktable for reading file\n");
              task_list_current->holded_info->read->fread->worktables.worktable = malloc(sizeof(struct WORKTABLE));
            }
            
            if (!task_list_current->holded_info->read->fread->worktables.worktable->focused_column && !task_list_current->holded_info->read->fread->worktables.worktable->column) {
              DEBUG("creating column for reading file\n");
              task_list_current->holded_info->read->fread->worktables.worktable->column = malloc(sizeof(struct COLUMN));  
              task_list_current->holded_info->read->fread->worktables.worktable->column->row = NULL;
            }
            if (task_list_current->holded_info->read->fread->worktables.worktable->column && !task_list_current->holded_info->read->fread->worktables.worktable->focused_column) {
              DEBUG("creating focused column for reading file\n");
              task_list_current->holded_info->read->fread->worktables.worktable->focused_column = task_list_current->holded_info->read->fread->worktables.worktable->column;
            }

            if (!task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row) {
              DEBUG("creating row for reading file\n");
              task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row = malloc(sizeof(struct ROW));
              task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row = task_list_current->holded_info->read->fread->worktables.worktable->focused_column->row;
              
              task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->type = "FILE";
              task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->on = 5;
            }
            DEBUG("going to save file buffer on row\n");
            task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->value.Buffer = Buffer;
            
            DEBUG("Successfully Done Reading File (%s)\n", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->value.Buffer);
            return NULL;
          }
          else {
            DEBUG("Failed To Read File \n");
          }
        }
        else {
          // this will be used to include the file that are found in string form
          // and read it
          DEBUG(" include string c code(%s)\n", path);
        }
      }
      
    } else if (split_list[1] && split_list[1] != NULL && issame(split_list[1], ">Variables")){
      if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">Get")){
        // this will get a variable by its index or name and focus its worktable and focused row to be used in the reading code and processing code
        // than retuns the worktable of the variable to be used in the reading code and processing code
        DEBUG("command(%s) value(Variable Focused As)\n", command);
        task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
        readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
        char *varadd = readword->value;
        
        // chacke if it is index or name
        if (varadd[0] <= '9' && varadd[0] >= '0' ) {
          // this is index
          char *wt = varadd;
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
          char *c = readword->value;
          
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
          char *r = readword->value;
          DEBUG("command(%s) value(Variable Focused As) index(%s) column(%s) row(%s)\n", command, wt, c, r);
          struct WORKTABLE *found = Find_worktable_by_indexs(atoi(wt), atoi(c), atoi(r));
          if (found) {
            task_list_current->holded_info->read->fread->worktables.focused_wt = found;
            DEBUG("Successfully Done Getting Variable By Index and Focusing it (%s)\n", command);
            return found;
          }
          else {
            DEBUG("Failed To Get Variable By Index \n");
            return NULL;
          }
        }
        else {
          // this is name
          char *name = varadd;
          DEBUG("command(%s) value(Variable Focused As) name(%s)\n", command, name);
          struct WORKTABLE *found = Find_worktable_by_name(name);
          if (found) {
            task_list_current->holded_info->read->fread->worktables.focused_wt = found;
            DEBUG("Successfully Done Getting Variable By Index and Focusing it (%s)\n", command);
            return found;
          }
          else {
            DEBUG("Failed To Get Variable By Index \n");
            return NULL;
          }
        }
      } else if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">Focused")){
        if (split_list[3] && split_list[3] != NULL && issame(split_list[3], ">As")){
          DEBUG("command(%s) value(Variable Focused As)\n", command);
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          readword = list_get_node_by_index(task_list_current->holded_info->read->fread->reading_value, task_list_current->holded_info->read->fread->reading_on); 
          char *Asname = readword->value;
          DEBUG("command(%s) Variable Focused As (%s)\n", command, Asname);
          task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->As = Asname;
          DEBUG("Successfully Done Saving Asname (%s)\n", task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->As);
          char *inctext = "";
          if(issame(task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->type, "FILE")) {
            // if the focused row is file then we will read the file and save it in the reading info to be used in the reading code
            inctext = task_list_current->holded_info->read->fread->worktables.worktable->focused_column->focused_row->value.Buffer;
          }
          
          task_list_current->holded_info->fread->state = 'L';
          char *incode = read_bodys(inctext);
          task_list_current->holded_info->read->fread->reading_value = str_splitL(incode, " ", 0);
          task_list_current->holded_info->read->fread->read_new = incode;
          
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          return NULL;
        } else {
          DEBUG("command(%s) value(Main Focused)\n", command);
        }

      } else {
        DEBUG("command(%s) value(Main Variables)\n", command);
      }

    } else if (split_list[1] && split_list[1] != NULL && issame(split_list[1], ">Tasks")){
      // for now we will not support this but in the future it will be used to handle the task command code and do what it says to do
      if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">This")){
        // to point to the current task that is holded in the reading info to be used in the reading code and processing code
        // it is toking about the current task
        if (split_list[3] && split_list[3] != NULL && issame(split_list[3], ">Know")){
          // this will add the knowlege to the task that is holded in the reading info to be used in the reading code and processing code
          // it reades it befor continueing the next code 
          DEBUG("command(%s) value(This Task Knowing )\n", command);

          // get the knowlege from the code and save it in the task that is holded in the reading info to be used in the reading code and processing code
          // TODO: this will be used to get the knowlege from the code and save it in the task that is holded in the reading info to be used in the reading code and processing code
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          DEBUG("going to get knowlege\n");
          struct WORKTABLE *getworingvar = ReadDo();
          DEBUG("Done getting knowlege\n");
          DEBUG("Successfully Done GettingAsname (%s)\n", getworingvar->focused_column->focused_row->As);
          task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
          return NULL;
        } else {
          DEBUG("command(%s) value(Main Focused)\n", command);
        }

      } else {
        DEBUG("command(%s) value(Main Variables)\n", command);
      }

    } else {
      DEBUG("command(%s) value(Enableing CoMmAnD)\n", command);
    }
  } 



  // If Keyword is "INCLUDE"
  // this will be used to include the file that are found in path form or string and read it
  if(issame(readword->value, "INCLUDE")) {
    // Mark this INCLUDE keyword as readen and move to the next word that is the path or string of the file to include
    task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;

    // this command will be handeld by code found in Src/Addis/Libs/INCLUDE
    // Going to include the file To Read that is found in path form or string and read it
    // this will be used to include the file that are found in path form or string and
    // and read it
    struct ROW *isInclude_readed = READ_INCLUDE(task_list_current->holded_info->read->fread->reading_value);
    
    // If the INCLUDE keyword is successfully read and processed, we can proceed with the reading process
    if (isInclude_readed){}
    return NULL;

  }

  Resive(task_list_current->holded_info->read->fread->reading_value, 1, 0);

	// chack if this word is KEYWORD or most be risive
  if(hold_on == task_list_current->holded_info->read->fread->reading_on)
  {
    

	  DEBUG("reading KEYWORD |%s|\n", readword->value);
    if(issame(readword->value, "WHEN")) {
      _event(task_list_current->holded_info->read->fread->reading_value);
      return NULL;
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
      return NULL;
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
      return NULL;
    }
    // if code not readen in here will try in Selection_Logic
    else {
      DEBUG("0>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
      ReadResivers(task_list_current->holded_info->read->fread->reading_value, "", "", 'R', 0);
      DEBUG("end OUT from other reading in sant to Selection_Logic\n");
      DEBUG("-<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      return NULL;
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
    return NULL;
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
    */return NULL;
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
    */return NULL;
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
    */return NULL;
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
    return NULL;
  }
  DEBUG("");
  return NULL;
}