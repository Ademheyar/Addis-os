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
#include <Addis/Tables/WorkingT.h> // for defining struct READINGINFO and other table-related functions
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
	task_list_current->holded_info->reading_on = task_list_current->holded_info->reading_stoped;
  //DEBUG("reading on %d\n", task_list_current->holded_info->reading_on);
	listnode_t *readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
	DEBUG("in ReadDo |%s|\n", readword->value);
  if (strlen(readword->value) == 0 || issame(readword->value, "")){ task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped; return NULL; }
  int hold_on = task_list_current->holded_info->reading_on;
  

  // Check if it is CoMmAnD or NORMAL code
  // chack if there is any code to read or if it is empty else preant the keyword
  if(readword && readword->value && strstr(readword->value, "->") != NULL)
  {
    DEBUG("Going to red CoMmAnD keyword (%s)\n", readword->value);
    // this will be used to handle the command code
    // and do what it says to do
    char **split_list = str_split(readword->value, '-');
    
    if(split_list[0] && split_list[0] != NULL && issame(split_list[0], "CoMmAnD"))
    {
      // if it is CoMmAnD ?
    }
    else{
      // Else We Will set it as Foucused and set first list CoMmAnD->Variable->Fucesde->split_list[1]....
      struct ROW *focvar = Find_Variable_by(split_list[0]);
      if(focvar){
        int size1 = 0;
        char *list2[] = {"CoMmAnD", ">Variables", ">Focused"};
        int size2 = 3;

        // Count list 1
        char** ptr1 = split_list;
        while (*ptr1 != NULL) { size1++; ptr1++; }
        
        // 1. Allocate memory for the new array (size1 + size2)
        char** combined = (char**)malloc((size1 + size2) * sizeof(char*));
        
        // 2. Copy pointers from the second array
        for (int i = 0; i < size2 && list2[i]; i++) combined[i] = list2[i]; 
        // 3. Copy pointers from the first array
        int i = 1;
        for (i = 1; i < size1; i++) combined[2 + i] = split_list[i];
        combined[i+2] = '\0';
        // Clean up
        free(split_list);
        split_list = combined;
      }
      // 
    }
    
    char *command = split_list[0];
    int iscontinue = -1;
    // this loop needed if there is pointer to varible to get and continue
    while(1){
      if(iscontinue == 0) break; // if it is finshed iscontinue will be setted 0 to go out from loop
      else iscontinue = -1;

      // this is por dev only no need
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
          // We have to jump CoMmAnD->File->Load 
          task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
          // then Get File To read
          struct WORKTABLE *retworktable = ReadDo();
          if(retworktable) {
            char *path = NULL;
            if(issame(retworktable->focused_column->focused_row->type, "String") ||
              issame(retworktable->focused_column->focused_row->type, "PATH") ||
              issame(retworktable->focused_column->focused_row->type, "FILE") ){
              path = retworktable->focused_column->focused_row->value.Buffer;
            }
            if(path){
              DEBUG("command(%s) value(Load) Path(%s)\n", command, path);
              if (strchr(path, '\\') || strchr(path, '/')) {
                // chacking if it is path or string if it has \ or / it is path else it is string
                // this will be used to load the file that is found in path form and read it
                char *Buffer = get_code(path);
                // make sure the space is cleard
                free(retworktable->focused_column->row);

                DEBUG("Done getting code from file\n");
                if (Buffer) {
                  // if it get the path then we will jump the path text and continue readintg the rest of the code and then we will read the file and save it in the reading info to be used in the reading code
                  //task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
                  
                  Create_newworktable();
                  task_list_current->holded_info->worktables.worktable->focused_column->focused_row->type = "FILE";
                  task_list_current->holded_info->worktables.worktable->focused_column->focused_row->on = 5;
                  
                  DEBUG("going to save file buffer on row\n");
                  task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer = Buffer;
                  
                  DEBUG("Successfully Done Reading File (%s)\n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer);
                  DEBUG("end task_list_current->holded_info->reading_on = %d\n", task_list_current->holded_info->reading_on);
                  return task_list_current->holded_info->worktables.worktable;
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
          }
          else {
            DEBUG("Failed To Get File Path \n");
          }
        }
        
      } 
      
      /*
      *
      *
      * 
      *                             Variables
      * 
      * 
      */ 
      else if (split_list[1] && split_list[1] != NULL && (issame(split_list[1], ">Variables") || strstr(split_list[1], ">Variables["))) {
          DEBUG("--->Variables \n");
          if(strstr(split_list[1], ">Variables[")){
            char **splited=str_split(split_list[1], '\"');
            struct ROW *focvar = Find_Variable_by(splited[1]);
            DEBUG("Done find varible %s\n", splited[1]);
            if(focvar){
              int size1 = 0;
              char *list2[] = {"CoMmAnD", ">Variables", ">Focused"};
              int size2 = 3;

              // Count list 1
              char** ptr1 = split_list;
              while (*ptr1 != NULL) { size1++; ptr1++; }
              
              // 1. Allocate memory for the new array (size1 + size2)
              char** combined = (char**)malloc((size1 + size2) * sizeof(char*));
              
              // 2. Copy pointers from the second array
              for (int i = 0; i < size2 && list2[i]; i++) combined[i] = list2[i]; 
              // 3. Copy pointers from the first array
              for (int i = 2; i < size1; i++) combined[1 + i] = split_list[i];
              // Clean up
              free(split_list);
              split_list = combined;
            }

          }

          /*
          *
          *
          * 
          *                             Variables New 
          *     for creating new varibles it can get direct 
          *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
          * 
          */ 
        if (split_list[2] && split_list[2] != NULL && (issame(split_list[2], ">New") || strstr(split_list[2], ">New["))){
          // this will get a variable by its index or name and focus its worktable and focused row to be used in the reading code and processing code
          // than retuns the worktable of the variable to be used in the reading code and processing code
          DEBUG("command(%s) value(Variable New)\n", command);
          char *get_name = "";
          struct WORKTABLE *newvar = NULL;
          if(strstr(split_list[2], ">New[")){
            char **splited=str_split(split_list[2], '\"');
            if(splited) get_name = splited[1];

            // after here we will set the raset of the code if it is continuing 
            // after the CoMmAnD->Variable->New["name"]->...
            // removestrlist_at_index(split_list, 2); do we need to remove this 
            // or chage it to ->Foucese->...
            free(split_list[2]);
            char *newvalue = ">Focused";
            split_list[2] = strdup(newvalue);
            iscontinue = 1;
          }

          if(issame(get_name, "")){
            // this if it is given as 
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            newvar = ReadDo();
            if(newvar) {
              if(issame(newvar->focused_column->focused_row->type, "String") ||
                issame(newvar->focused_column->focused_row->type, "PATH") ||
                issame(newvar->focused_column->focused_row->type, "FILE") ){
                get_name = newvar->focused_column->focused_row->value.Buffer;
                newvar->focused_column->focused_row->As = get_name;
                iscontinue = 0; // get out
              }
            }
          }
          DEBUG("command(%s) value(Varible Name)\n", get_name); 
          
          Create_newworktable();
          
          task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As = get_name;
          DEBUG("Secsessfuly Created New command(%s) \n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As); 
          iscontinue = 1; // get out
        } 
        
        
          /*
          *
          *
          * 
          *                             Variables Get
          *     for creating new varibles it can get direct 
          *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
          * 
          */ 
        else if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">Get")){
          // this will get a variable by its index or name and focus its worktable and focused row to be used in the reading code and processing code
          // than retuns the worktable of the variable to be used in the reading code and processing code
          DEBUG("command(%s) value(Variable Focused As)\n", command);
          task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
          readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
          char *varadd = readword->value;
          
          // chacke if it is index or name
          if (varadd[0] <= '9' && varadd[0] >= '0' ) {
            // this is index
            char *wt = varadd;
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
            char *c = readword->value;
            
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
            char *r = readword->value;
            DEBUG("command(%s) value(Variable Focused As) index(%s) column(%s) row(%s)\n", command, wt, c, r);
            struct WORKTABLE *found = Find_worktable_by_indexs(atoi(wt), atoi(c), atoi(r));
            if (found) {
              task_list_current->holded_info->worktables.focused_wt = found;
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
              task_list_current->holded_info->worktables.focused_wt = found;
              DEBUG("Successfully Done Getting Variable By Index and Focusing it (%s)\n", command);
              return found;
            }
            else {
              DEBUG("Failed To Get Variable By Index \n");
              return NULL;
            }
          }
        } 
        
        /*
        *
        *
        * 
        *                             Variables Focused
        *     for creating new varibles it can get direct 
        *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
        * 
        */ 
        else if (split_list[2] && split_list[2] != NULL && issame(split_list[2], ">Focused")){
          if (split_list[3] && split_list[3] != NULL && issame(split_list[3], ">As")){
            DEBUG("command(%s) value(Variable Focused As)\n", command);
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
            char *Asname = readword->value;
            DEBUG("command(%s) Variable Focused As (%s)\n", command, Asname);
            task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As = Asname;
            DEBUG("Successfully Done Saving Asname (%s)\n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As);
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            return NULL;
          } 
          
          /*
          *
          *
          * 
          *                             Variables Focused Extern
          *     for setting focused varible to external varible
          *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
          * 
          */ 
          else if (split_list[3] && split_list[3] != NULL && issame(split_list[3], ">Extern")){
            DEBUG("command(%s) value(Variable Focused Extern)\n", command);
            // chacke if there is focuesd varible
            if(task_list_current->holded_info->worktables.worktable->focused_column->focused_row){
              if(task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As) DEBUG("With Variable Focused As %s\n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->As);
              
              // now set the varible to extrnal 
              DEBUG("going to Setting varible to external or global)\n");
              struct ROW *retextrnvar = Set_varible_extern(task_list_current->holded_info->worktables.worktable->focused_column->focused_row);
              DEBUG("Done Setting varible to external or global)\n");
              
              DEBUG("Successfully Done Saving Asname (%s)\n", retextrnvar->As);
              // go to next code
              task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
              return task_list_current->holded_info->worktables.worktable;
            }
          }
          /*
          *
          *
          * 
          *                             Variables Focused Do
          *     for setting focused varible to external varible
          *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
          * 
          */ 
          else if (split_list[3] && split_list[3] != NULL && issame(split_list[3], ">Do")){
            DEBUG("command(%s) value(Variable Focused Do)\n", command);
            // chacke if there is focuesd varible
            struct ROW *focusedrow = task_list_current->holded_info->worktables.worktable->focused_column->focused_row;
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            struct WORKTABLE *newvar = ReadDo();
            char *get_do = "";
            if(newvar) {
              if(issame(newvar->focused_column->focused_row->type, "String") ||
                issame(newvar->focused_column->focused_row->type, "Do") ||
                issame(newvar->focused_column->focused_row->type, "FILE") ){
                get_do = newvar->focused_column->focused_row->value.Buffer;
              }
            }

            if(focusedrow){
              if(focusedrow->As) DEBUG("With Variable Focused As %s setting Do\n", focusedrow->As);
              // set do
              focusedrow->value.Buffer = get_do;
              DEBUG("Successfully Done setted Do ");
              if(focusedrow->value.Buffer) DEBUG(" (%s)\n", focusedrow->value.Buffer);
              else DEBUG("\n");
              // go to next code
              task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
              return task_list_current->holded_info->worktables.worktable;
            }
          }
          /*
          *
          *
          * 
          *                             Variables Focused
          *     for setting focused varible to external varible
          *     CoMmAnD->Variable->New "variable name" or CoMmAnD->Variable->New["variable name"]
          * 
          */
          else {
            DEBUG("command(%s) value(Main Focused)\n", command);

            if (!task_list_current->holded_info->worktables.worktable) {
              DEBUG("creating worktable for SENDING AS FOCUSED file\n");
              task_list_current->holded_info->worktables.worktable = malloc(sizeof(struct WORKTABLE));
            }
            
            if (!task_list_current->holded_info->worktables.worktable->focused_column && !task_list_current->holded_info->worktables.worktable->column) {
              DEBUG("creating column for SENDING AS FOCUSED file\n");
              task_list_current->holded_info->worktables.worktable->column = malloc(sizeof(struct COLUMN));  
              task_list_current->holded_info->worktables.worktable->column->row = NULL;
            }
            if (task_list_current->holded_info->worktables.worktable->column && !task_list_current->holded_info->worktables.worktable->focused_column) {
              DEBUG("creating focused column for SENDING AS FOCUSED file\n");
              task_list_current->holded_info->worktables.worktable->focused_column = task_list_current->holded_info->worktables.worktable->column;
            }

            if (!task_list_current->holded_info->worktables.worktable->focused_column->row) {
              DEBUG("creating row for SENDING AS FOCUSED file\n");
              task_list_current->holded_info->worktables.worktable->focused_column->row = malloc(sizeof(struct ROW));
              task_list_current->holded_info->worktables.worktable->focused_column->focused_row = task_list_current->holded_info->worktables.worktable->focused_column->row;
              
              task_list_current->holded_info->worktables.worktable->focused_column->focused_row->type = "";
              task_list_current->holded_info->worktables.worktable->focused_column->focused_row->on = 5;
            }
            struct WORKTABLE *found = task_list_current->holded_info->worktables.worktable;
            DEBUG("command(%s) value(Main Focused)\n", found->focused_column->focused_row->value.Buffer);
            return found;
          }

        } else {
          DEBUG("command(%s) value(Main Variables)\n", command);
        }
        // Done Explaning Variable Keyword
      } 
      /*
      *
      *
      * 
      *                             Tasks
      * 
      * 
      */
      else if (split_list[1] && split_list[1] != NULL && issame(split_list[1], ">Tasks")){
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
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            DEBUG("going to get knowlege\n");
            struct WORKTABLE *getworingvar = ReadDo();
            DEBUG("Done getting knowlege\n");
            if (getworingvar) {
              DEBUG("Successfully Done Getting Knowlege Now Loding To read it\n");
              if (getworingvar->focused_column->focused_row->As != NULL) DEBUG(" Asnamed (%s)\n", getworingvar->focused_column->focused_row->As);
              char *inctext = "";
              if(issame(getworingvar->focused_column->focused_row->type, "FILE") || 
                issame(getworingvar->focused_column->focused_row->type, "String")) {
                // if the focused row is file then we will read the file and save it in the reading info to be used in the reading code
                inctext = getworingvar->focused_column->focused_row->value.Buffer;
              }

              if(inctext) Create_working_place(inctext, 'L');
              task_list_current->holded_info->state = 'L';
              DEBUG("Successfully Loding Knowlege\n");
            } else {
              DEBUG("Failed To Get Knowlege \n");
            }
            return NULL;
          } else {
            DEBUG("command(%s) value(This Task)\n", command);
          }

        } else {
          DEBUG("command(%s) value(This)\n", command);
        }

      } 
      
      /*
      *
      *
      * 
      * 
      * 
      * 
      */
      else {
        DEBUG("command(%s) value(Enableing CoMmAnD)\n", command);
      }

      if(iscontinue != 1) iscontinue = 0;
    }
  } 

  /*
  *
  *
  * 
  *                             Get funtion holder
  * 
  * 
  */
  // getting function holders
  // { <- strts with   end with -> }
  else if(readword && readword->value && ((strstr(readword->value, "\{") != NULL || issame(readword->value, "\{")))){
    DEBUG("Getting Function \n");
    DEBUG("on %d\n", task_list_current->holded_info->reading_on);
    int capacity = 1024; // 1kb spcae
    char *strcop = (char *)malloc(capacity);
    int isreding = 0;
    int strindex = 0;
    while (true)
    {
      // if we run out of space we will incrice it
      if (strindex >= capacity-1){
        DEBUG("adding space \n");
        capacity *= 2;
        char *newbuffer = (char *)malloc(capacity);
        for(int j = 0; j < strindex; j++){
          newbuffer[j] = strcop[j];
        }
        // free the old space
        free(strcop);
        // point to the new space
        strcop = newbuffer;
        DEBUG("Done adding space \n");
      }
      
      if(!(readword && readword->value)) { break;}
      char *temp_keyword = readword->value;
      
      //DEBUG("going to copy |%s| \n", temp_keyword);
      int keylen = strlen(temp_keyword);
      for(int i = 0; i < keylen; i++) {
        if(temp_keyword[i] == '{') { isreding += 1; if(isreding == 1) continue;}
        else if (temp_keyword[i] == '}') { isreding -= 1; if(isreding == 0) break; }
        if(isreding >= 1) {
          //DEBUG("copy char |%c| \n", temp_keyword[i]);
          strcop[strindex++] = temp_keyword[i];
        }
      }
      if(isreding){
        if(strindex != 0) strcop[strindex++] = ' ';
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
        readword =  list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
      }
      if(isreding == 0) break;
    }
    strcop[strindex++] = '\0';

    DEBUG("on %d\n", task_list_current->holded_info->reading_on);
    DEBUG("{} found = %s\n", strcop);
Create_newworktable();
    DEBUG("going to save {} on row\n");
    task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer = strcop;
    DEBUG("Successfully Done Reading {} (%s)\n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer);
    return task_list_current->holded_info->worktables.worktable;    
  }

  /*
  *
  *
  * 
  *                             Tasks
  * 
  * 
  */
  // for getting strings " <- strts and ends with -> "
  // If Keyword is "" if the word is starting with " or the word it self is " this will be used to handle the string and save it in the reading info to be used in the reading code
  
  else if(readword && readword->value && ((strstr(readword->value, "\"") != NULL || issame(readword->value, "\"")))){
    task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
    int started = 0;
    int capacity = 1024; // 1kb spcae
    char *strcop = (char *)malloc(capacity);
    int isreding = 1;
    int strindex = 0;
    DEBUG("Getting Function \n");
    while (isreding)
    {
      // if we run out of space we will incrice it
      if (strindex >= capacity-1){
        DEBUG("adding space \n");
        capacity *= 2;
        char *newbuffer = (char *)malloc(capacity);
        for(int j = 0; j < strindex; j++){
          newbuffer[j] = strcop[j];
        }
        // free the old space
        free(strcop);
        // point to the new space
        strcop = newbuffer;
        DEBUG("Done adding space \n");
      }

      char *temp_keyword = readword->value;
      int keylen = strlen(temp_keyword);
      for(int i = 0; i < keylen; i++) {
        if(temp_keyword[i] == '"') { if(started == 0) { started = 1; continue; } else  isreding = 0; break; }
        strcop[strindex++] = temp_keyword[i];
      }
      if(isreding){
        if(strindex != 0) strcop[strindex++] = ' ';
        readword =  list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on); 
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
        DEBUG("task_list_current->holded_info->reading_on = %d\n", task_list_current->holded_info->reading_on);
      }
    }
    strcop[strindex++] = '\0';
    DEBUG("end task_list_current->holded_info->reading_on = %d\n", task_list_current->holded_info->reading_on);
    DEBUG("strcoped = %s\n", strcop);

    Create_newworktable();
    DEBUG("going to save string on row\n");
    task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer = strcop;
    DEBUG("Successfully Done Reading string (%s)\n", task_list_current->holded_info->worktables.worktable->focused_column->focused_row->value.Buffer);
    return task_list_current->holded_info->worktables.worktable;   
  }

  /*
  *
  *
  * 
  * 
  * 
  * 
  */
  // or we will chack if the word is deffined or is varible
  else if(readword && readword->value) {
      DEBUG("Find Unknown Word (%s)\n", readword->value);
      struct ROW *focvar = Find_Variable_by(readword->value);
      if(focvar){
        DEBUG("Successfully Done Getting Knowlege Now Loding To read it\n");
        if (focvar->As != NULL) DEBUG(" named (%s)\n", focvar->As);
        if (focvar->type != NULL) DEBUG(" typed (%s)\n", focvar->type);
        char *inctext = "";
        if(issame(focvar->type, "FILE") || 
          issame(focvar->type, "String") || 
          issame(focvar->type, "WORD")) {
          // if the focused row is file then we will read the file and save it in the reading info to be used in the reading code
          inctext = focvar->value.Buffer;
          if(inctext) Create_working_place(inctext, 'L');
          task_list_current->holded_info->state = 'L';
          DEBUG("Successfully Found Deffined and laoded the Knowlege\n");
        }
      }
      return NULL; // try to get the Worktable
  }
  











  
  Resive(task_list_current->holded_info->reading_value, 1, 0);

	// chack if this word is KEYWORD or most be risive
  if(hold_on == task_list_current->holded_info->reading_on)
  {
    

	  DEBUG("reading KEYWORD |%s|\n", readword->value);
    if(issame(readword->value, "WHEN")) {
      _event(task_list_current->holded_info->reading_value);
      return NULL;
    }
    
    if(issame(readword->value, "GET")) {
      task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
      DEBUG("GOING TO GET\n");
      // get what type
      
      // get as if there is
      // TODO: AND UESES FOR NEW ADD AND 
      while(issame(readword->value, "AS")) 
      {
        DEBUG("getting variable AS\n");
        task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
        readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
        struct ROW *row = Resive(task_list_current->holded_info->reading_value, 1, 1);
        if(row)
        {
          task_list_current->holded_info->getas = strdup(row->word);
          free(row);

          if (issame(readword->value, "AND")) 
          {
            task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
            readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
            continue;
          }
        }
        break;
      }

      int rfid = ++task_list_current->holded_info->rfor_id;
      task_list_current->holded_info->reading_for[rfid] = "GETTING";
      DEBUG("name(%s)\n", task_list_current->holded_info->name);
      DEBUG("Done going to get\n");
      return NULL;
    }
    
    else if(issame(readword->value, "OPERATE") || issame(readword->value, "CASE")){
      DEBUG("in %s\n", readword->value);
      char *word = strdup(readword->value);
      task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
      struct ROW *rowvalue = malloc(sizeof(struct ROW));
      rowvalue->type = "OPERATOR";
      rowvalue->word = readword->value;
      if(issame(word, "CASE")) {
        rowvalue->word = word;
        Addrow_to_row_byindex(rowvalue, 0, 0);
        char *var = read_unknownvars(task_list_current->holded_info->reading_value, "TAKE", 0)->type;
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
      task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
      readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
      DEBUG("\nDone adding new working table going to oprate_value\n");
      oprate_value();
      return NULL;
    }
    // if code not readen in here will try in Selection_Logic
    else {
      DEBUG("0>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
      ReadResivers(task_list_current->holded_info->reading_value, "", "", 'R', 0);
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
    task_list_current->holded_info->reading_on =  ++task_list_current->holded_info->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
    //char *ret = read_unknownvars(task_list_current->holded_info->reading_value, "", 0);
    //task_list_current->holded_info->ret.text = ret; // it will return goted text
    //task_list_current->holded_info->returned = task_list_current->holded_info->worktables; // it will return the working tabel
    /// add other that can be returned here
		//task_list_current->holded_info->reading_value = str_splitL(code, " ", 0);
    //task_list_current->holded_infoing[task_list_current->holded_info_id] = " ";
    DEBUG(" RETURNED\n");
    return NULL;
  }
  
  
  // divise refistering  codes starts here
  else if(issame(readword->value, "REGISTER_DRIVER")){
    task_list_current->holded_info->reading_on =  ++task_list_current->holded_info->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
    DEBUG(" in REGISTER_DRIVER on = %d line = %d  \n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    Clear_worktables(0);
    /*char *name=" ", *handler="";
    uint8_t hexa=0;
    read_unknownvars(task_list_current->holded_info->reading_value, "TAKE", 0);
    read_unknownvars(task_list_current->holded_info->reading_value, "WITH", 0);
    //task_list_current->holded_info->table[0] = &task_list_current->holded_info->table[0];
    char *rettake = stradd("", task_list_current->holded_info->worktables.worktable[wtid].focused_column->row->value, 0);
    DEBUG(" rettake(%s)\n", rettake);
    if(thisvar("HEXA", rettake)) {
      convert(rettake);
      hexa = convertto.hexa_8;
      DEBUG(" hexa = 0x%x int = %d\n", hexa, hexa);
    }
    name = task_list_current->holded_info->name;
    handler = stradd("", task_list_current->holded_info->worktables.worktable[wtid].focused_column->row1].value, 0);
    register_driver(hexa, name, handler, rettake);
    drivers[hexa].onwdb = task_list_current->holded_info;
    DEBUG(" end REGISTER_DRIVER on = %d line = %d\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    */return NULL;
  }
  // enavleing drivers codes starts here
  else if(issame(readword->value, "ENABLE_DRIVER")){
    task_list_current->holded_info->reading_on =  ++task_list_current->holded_info->reading_stoped;
					readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
    DEBUG(" in ENABLE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length, readword->value);
    Clear_worktables(0);
    read_unknownvars(task_list_current->holded_info->reading_value, "TAKE", 0);
    /*char *rettake = stradd("", task_list_current->holded_info->worktables.worktable[wtid].focused_column->row->value, 0);
    convert(rettake);
    uint8_t hexa = convertto.hexa_8;
    DEBUG(" rettake(%s) hx(0x%x) int(%d)\n", rettake, hexa, hexa);
    enable_driver(hexa);
    DEBUG(" ENABLE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length, readword->value);
    DEBUG(" end ENABLE_DRIVER on = %d line = %d\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    */return NULL;
  }
  // enavleing drivers codes starts here
  else if(issame(readword->value, "ACKNOWLEDGE_DRIVER")){
    task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
    DEBUG(" in ACKNOWLEDGE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length, readword->value);
    Clear_worktables(0);
    read_unknownvars(task_list_current->holded_info->reading_value, "TAKE", 0);
    /*char *rettake = stradd("", task_list_current->holded_info->worktables.worktable[wtid].focused_column->row->value, 0);
    convert(rettake);
    uint8_t hexa = convertto.hexa_8;
    DEBUG(" rettake(%s) hx(0x%x) int(%d)\n", rettake, hexa, hexa);
    pic_acknowledge(hexa);
    DEBUG(" ACKNOWLEDGE_DRIVER on = %d line = %d code %s\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length, readword->value);
    DEBUG(" end ACKNOWLEDGE_DRIVER on = %d line = %d\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    */return NULL;
  }
  // output codes starts here
  else if(issame(readword->value, "OUTPUT")){
    DEBUG("\nin OUTPUT on = %d line = %d ", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    // clears all previus list codes
    task_list_current->holded_info->reading_on =  ++task_list_current->holded_info->reading_stoped;
		readword = list_get_node_by_index(task_list_current->holded_info->reading_value, task_list_current->holded_info->reading_on);
    //task_list_current->holded_info->ret.retable.column[task_list_current->holded_info->ret.retable.countcolumn].row=0;
    DEBUG("getting info ");
    //char *rettake = ReadResivers(task_list_current->holded_info->reading_value, "TAKE", "GET", 'R');
   // convert(rettake);
    //DEBUG("rettake = %s ", rettake);
    uint8_t hexa = convertto.hexa_16;
    DEBUG("rettake = %s hx = 0x%x int=%d ", "rettake", hexa, hexa);
    READ_DATA_PORT(hexa);
    DEBUG("end OUTPUT on = %d line = %d\n", task_list_current->holded_info->reading_on, task_list_current->holded_info->reading_value->length);
    return NULL;
  }
  DEBUG("");
  return NULL;
}