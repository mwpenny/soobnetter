/* list.c */
/* Barebones linked list implementation */

#include "list.h"

void list_init(List* list)
{
	list->size = 0;
	list->head = list->tail = NULL;
}

void list_push(List* list, void* data)
{
	/* Create a node for the data and add it to the end */
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->data = data;
	node->next = NULL;

	/* Adding to empty list */
	if (list->tail == NULL)
		list->head = node;
	else
		list->tail->next = node;

	list->tail = node;
	++list->size;
}

void list_free(List* list)
{
	/* Free dynamically allocated list elements */
	ListNode *currNode = list->head, *prevNode;
	while (currNode != NULL)
	{
		list->head = currNode->next;
		--list->size;

		free(currNode->data);
		prevNode = currNode;
		currNode = currNode->next;
		free(prevNode);
	}
}