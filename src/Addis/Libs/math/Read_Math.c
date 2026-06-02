#include <Addis/Tables/WorkingT.h> // for defining struct READINGINFO and other table-related functions
#include <Kernel.h>
#include <Addis/Libs/String/Text.h>

struct ROW *Read_Math(list_t *read, struct ROW *frow, int isret)
{
  int found = 0;
  if (!frow) return NULL;
  isret=isret;
  listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
  while(1)
  {
    readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
    DEBUG("Read_Math %s\n", readword->value);
    //DEBUG("issigne %s \n", readword->value);
    if (issame(readword->value, "+")) 
    {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("ADD\n");
      struct ROW *srow = Resive(read, 0, 1);
      found = 1;
      //DEBUG("ADD f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      frow = marge_rows("+", frow, srow);
      //DEBUG("ADD f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      continue;
    }
    else if (issame(readword->value, "-")) 
    {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      DEBUG("Mines\n");
      struct ROW *srow = Resive(read, 0, 1);
      found = 1;
      //DEBUG("Mines f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      frow = marge_rows("-", frow, srow);
      //DEBUG("Mines f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      continue;
    }
    else if (issame(readword->value, "*")) 
    {
      //struct ROW *got = Resive(read, 1, 1);
        
    }
    else if (issame(readword->value, "/")) 
    {
      task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      struct ROW *srow = Resive(read, 0, 1);
			readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      found = 1;
      //DEBUG("Mines f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      frow = marge_rows("/", frow, srow);
      //DEBUG("Mines f%d  s%d\n", frow->value.number.rational, srow->value.number.rational);
      readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
      continue;
    }
    else
    {
      if(found){
        if (isret) return frow;
        // else TODO:
      }
      else return NULL;
    }
  }
  //Clear_worktables(0);
  return  NULL;
}

