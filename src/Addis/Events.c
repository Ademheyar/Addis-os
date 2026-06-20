#include <Addis/Read_Do.h>
#include <Addis/Events.h>
#include <Addis/Process.h> // for defining task_list_current and other process-related functions
#include <Kernel.h> // for defining Kernel related functions and variables
#include <Addis/Libs/String/Text.h>
#include <Addis/Tables/WorkingT.h> // for defining struct READINGINFO and other table-related functions

void Create_event_onvar(struct READINGINFO *var)
{
  struct event *new = malloc(sizeof(struct event));
  if(!var->main_event) var->main_event = new;
  else var->last_event->next_event = new;
  var->last_event = new;
  var->foucsed_event = new;
  var->foucsed_event->by = strdup(var->name);
  var->foucsed_event->event_value = NULL;
  var->foucsed_event->do_value = NULL;
  var->foucsed_event->prop = NULL;
  var->foucsed_event->compare = NULL;
}

void _event(list_t *read) {
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on); 
  if(issame(readword->value, "WHEN")) {
    task_list_current->holded_info->reading_on = ++task_list_current->holded_info->reading_stoped;
    readword = list_get_node_by_index(read, task_list_current->holded_info->reading_on);
    struct READINGINFO *var = NULL;
    struct ROW *gotas = NULL, *oprater = NULL, *dovalue = NULL, *value = NULL, *happe = NULL, *prop = NULL;
    while(1) {
      gotas = Resive(read, 1, 1);
      if(gotas) {
        if(gotas->type) {
          DEBUG("got event ");
          if(issame(gotas->type, "VARIABLE")){
            var = gotas->value.variable.fuwdb;
            prop = gotas;
            if (var->name) DEBUG("var name %s ", var->name);
            if (gotas->value.variable.prop) DEBUG("var prop|%s|", gotas->value.variable.prop);
          }
          else if(issame(gotas->type, "OPREATER")) oprater = gotas;
          else if(issame(gotas->type, "DO")) dovalue = gotas;
          else{
            char *type = get_row_type(gotas);
            DEBUG("type %s ", type);
            if(issame(type, "WORD")) {
              if(!oprater) oprater = gotas;
              else if(!happe) happe = gotas;
              else if(!value) value = gotas;
              else if(!dovalue) dovalue = gotas;
            }
            else if(!value) value = gotas;
          }
        }
        DEBUG("\n");
        if(var && prop && oprater && dovalue && happe && value) break;
        if((int)task_list_current->holded_info->reading_value->length - task_list_current->holded_info->reading_stoped <= 2) break;
        continue;
      }
      break;
    }
    if(!var) var = task_list_current->holded_info;
    // create event
    DEBUG("going to create event\n");
    Create_event_onvar(var);
    DEBUG("going to create event\n");
    if (var && var->name) DEBUG("name %s\n", var->name);
    if (happe) { var->foucsed_event->ishappen = happe; DEBUG("ishappen %s\n", var->foucsed_event->ishappen->type); }
    if (value) { var->foucsed_event->event_value = value; DEBUG("event_value %s\n", var->foucsed_event->event_value->type); }
    if (dovalue) { var->foucsed_event->do_value = dovalue; DEBUG("do_value %s\n", var->foucsed_event->do_value->type); }
    if (prop) { var->foucsed_event->prop = prop; DEBUG("prop %s\n", var->foucsed_event->prop->type); }
    if(oprater) { var->foucsed_event->compare = oprater; DEBUG("compare %s\n", var->foucsed_event->compare->type); }
    DEBUG("Done createing and saveing events\n");
  }
}

void Chack_all_events(struct READINGINFO *var, char *happend, struct ROW *value)
{
  if (var && var->name) DEBUG("\nname %s ", var->name);
  if (happend) DEBUG("happen %s ", happend);
  if (value && value->type) DEBUG("value->type %s ", value->type);
  struct event *new = var->main_event;
  for(; new; new = new->next_event){
    if(new->by){
      if(var->name && issame(new->by, var->name)){
        DEBUG("by %s - %s\n", new->by, var->name);
      }
      else continue;
    }
    
    if(new->ishappen){
      if(happend && issame(new->ishappen->word, happend))
      DEBUG("ishappen %s - %s\n", new->ishappen->word, happend);
      else continue;
    }
    if(new->prop && new->prop->value.variable.prop) DEBUG("prop %s\n", new->prop->value.variable.prop);
    
    if(new->event_value->type && new->compare->type){
      DEBUG("event_value %s %s \n", new->event_value->type, new->compare->word);
      if (compar(new->compare->word, new->event_value, value)) DEBUG("value type is same\n");
      else continue;
    } 

    if(new->do_value->word) {
      DEBUG("new->do_value %s\n", new->do_value->word);
      if (!(issame(new->do_value->word, "") || issame(new->do_value->word, " "))) {
        if(var->read_new) var->read_new = stradd(var->read_new, new->do_value->word, '\n'); 
        else var->read_new = stradd(new->do_value->word," ", 0); 
        if(var->reading_task) {
          DEBUG("task is still reading\n");

        } 

        if(!var->reading_task) {
          DEBUG("gatting code\n");
          create_kernel_process_last((void*)Read_focused, 3);
          //Create_readinfo('L'); // TODO: if event is reading make this new event wit
          DEBUG("out from text\n");
          task_list_last->holded_info = var;
          task_list_last->holded_info->reading_task = task_list_last;
          task_list_last->holded_info->rfor_id = 0;
          task_list_last->holded_info->reading_for[0] = "READING";
          DEBUG(" done creating(task-id%d) and fixing new->do_value %s\n", task_list_last->id, task_list_last->holded_info->read_new);
        }
        //task_list_last = NULL;
      }
      DEBUG("done puting event\n");
    }
  }
}
