/* list.h */
/* Barebones linked list definition */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct Node
{
	void* data;
	struct Node* next;
} ListNode;

typedef struct
{
	unsigned int size;
	ListNode *head, *tail;
} List;

void list_init(List* list);
void list_push(List* list, void* data);
void list_free(List* list);

#endif