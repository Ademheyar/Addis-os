
#include <Kernel.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Tables/WorkingT.h>

void hd_Driver(list_t *read)
{
  listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
  // other skiping continuing and returns file
  if(issame(readword->value, "OPEN")) {
    task_list_current->holded_info->reading_on =  ++task_list_current->holded_info->reading_stoped;
    readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
    
    char *name = "", *path = "";
    for(int z = 0; z<= 1;z++){
      struct ROW *r = Resive(read, 1, 1);
      if (r->word) {
        if(issame(path, "")) path = r->word;
        else if(issame(name, "")) name = r->word;
      }
    }

    if(!(issame(name, "") || issame(path, "")))
    {
      DEBUG("\nOPENING \"%s\" As %s\n", path, name);
      FILE *file = fopen(path, "r");
      if (file != NULL) {
        uint32_t size = fsize(file);
        void *buffer = malloc(size + 512);

        fread(buffer, size, 1, file);
        struct READINGINFO *ret = Create_vartable(read, NULL, 1);
        if(ret){
          ret->name = strdup(name);
          ret->value.value.hex.hexs_8 = buffer;
          ret->value.type = "BUFFER";
          ret->type = "BUFFER";
          DEBUG(" OPENED var id = %d name = %s \n", task_list_current->holded_info->varsid, ret->name);
        }
        else DEBUG("worktablecr not OPENED\n");
      }
      else DEBUG(" NOT OPENED %s\n", path);
      name = ""; path = "";
    }
    Clear_worktables(0);
    DEBUG(" DONE OPENING\n");
    return;
  }


}

// read text file and get it length, lines, text ....
char* read_text(char* filepath){
	FILE *file = fopen(filepath, "r"); // read file from dir
	if (file == NULL) return NULL; // if read text fails return null or 0
	uint32_t size = fsize(file); // get file size
  uint32_t *buffer; // create space for the the text using its size
  if (size > 512) buffer = malloc(size + 512); // create space for the the text using its size
  else buffer = malloc(size);
	fread(buffer, size, 1, file); // read text we got the text named by buffer
	char *text = (char *)buffer; // creat text info for text that is readen
  fclose(file);
	free(buffer);
  if (size == 0) text = "";
  //DEBUG("buffer text =|%s| \n", text);
  return text; // returning info
}
