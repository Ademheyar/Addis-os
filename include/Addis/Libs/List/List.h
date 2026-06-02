
#ifndef LIST_H
#define LIST_H
/* vim: tabstop=4 shiftwidth=4 noexpandtab
 * 
 * General-purpose list implementations.
 */

#pragma once

 #ifdef _KERNEL_
	#include <Libs/Kheap.h>
#endif

#include <Libs/Stdio/Stdio.h>


typedef struct listnode {
	struct listnode * next;
	struct listnode * prev;
	void * value;
	void * type;
	void * owner;
} __attribute__((packed)) listnode_t;

typedef struct {
	listnode_t * head;
	listnode_t * tail;
	int on;
	size_t length; // talls how many lists contans
} __attribute__((packed)) list_t;


extern void list_destroy(list_t * list);
extern void list_free(list_t * list);
extern void list_append(list_t * list,listnode_t* item);
extern listnode_t* list_insert(list_t * list, void * item);
extern list_t * list_create(void);
extern listnode_t* list_find(list_t * list, void * value);
extern int list_index_of(list_t * list, void * value);
extern void list_remove(list_t * list, size_t index);
extern void list_delete(list_t * list,listnode_t* node);
extern listnode_t* list_pop(list_t * list);
extern listnode_t* list_dequeue(list_t * list);
extern list_t * list_copy(list_t * original);
extern void list_merge(list_t * target, list_t * source);
extern void * list_index(list_t * list, int index);

extern void list_append_after(list_t * list,listnode_t* before,listnode_t* node);
extern listnode_t* list_insert_after(list_t * list,listnode_t* before, void * item);

extern void list_append_before(list_t * list,listnode_t* after,listnode_t* node);
extern listnode_t* list_insert_before(list_t * list,listnode_t* after, void * item);


/* Known to conflict with some popular third-party libraries. */
#ifndef TOARU_LIST_NO_FOREACH
#  define foreach(i, list) for (listnode_t * i = (list)->head; i != NULL; i = i->next)
#  define foreachr(i, list) for (listnode_t * i = (list)->tail; i != NULL; i = i->prev)
#endif

//#define foreach(t, list) for(listnode_t * t = list->head; t != NULL; t = t->next)

list_t * list_create();

uint32_t list_size(list_t * list);
listnode_t * list_insert_front(list_t * list, void *type, void * val);
void list_insert_back(list_t * list, void *type, void * val);
void * list_remove_node(list_t * list, listnode_t * node);
void * list_remove_front(list_t * list);
void * list_remove_back(list_t * list);
void list_push(list_t * list, void * val);
listnode_t * list_pop(list_t * list);
void list_enqueue(list_t * list, void * val);
listnode_t * list_dequeue(list_t * list);
void * list_peek_front(list_t * list);
void * list_peek_back(list_t * list);
void list_destroy(list_t * list);
void listnode_destroy(listnode_t * node);
int list_contain(list_t * list, void * val);
listnode_t * list_get_node_by_index(list_t * list, int index);
void * list_remove_by_index(list_t * list, int index);
#endif