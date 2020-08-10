#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define STATUS_CODE_SUCCESS 0
#define STATUS_CODE_ERROR 1
#define INDEX_NOT_FOUND -1


int AddNodeToList(struct node **head, const int value, const AddToListPosition_t position, const int afterValue) {
	struct node *newNode = (struct node*)malloc(sizeof(struct node));
	newNode->value = value;
	newNode->next = NULL;
	newNode->prev = NULL;
	if (!(*head)) {
		if (position == AFTER_VALUE) {
			printf("Error - list is empty!\n");
			free(newNode);
			return STATUS_CODE_ERROR;
		}
		else {
			*head = newNode;
			return STATUS_CODE_SUCCESS;
		}
	}
	struct node *nodeIter = *head;
	switch (position) {
	case END_OF_LIST: {
		while (nodeIter->next != NULL) nodeIter = nodeIter->next;
		nodeIter->next = newNode;
		newNode->prev = nodeIter;
		break;
	}
	case START_OF_LIST: {
		(*head)->prev = newNode;
		newNode->next = *head;
		*head = newNode;
		break;
	}
	case AFTER_VALUE: {
		while (nodeIter->value != afterValue) {
			if (!(nodeIter->next)) {
				printf("Error - value=%d not found in list!\n", afterValue);
				return STATUS_CODE_ERROR;
			}
			nodeIter = nodeIter->next;
		}
		newNode->next = nodeIter->next ? nodeIter->next : NULL;
		newNode->prev = nodeIter;
		nodeIter->next = newNode;
		if (newNode->next) (newNode->next)->prev = newNode;
		break;
	}
	}
	return STATUS_CODE_SUCCESS;
}

void PrintElementInList(struct node *head, const int value) {
	int indexToPrint = INDEX_NOT_FOUND, currIndex = 0;
	struct node *nodeIter = head;
	while (nodeIter) {
		if (nodeIter->value == value) {
			indexToPrint = currIndex;
			break;
		}
		currIndex++;
		nodeIter = nodeIter->next;
	}
	printf("%d\n", indexToPrint);
}

int DeleteIndexFromList(struct node **head, const int index) {
	int nodesToGo = index;
	struct node *nodeIter = *head;
	while (nodesToGo) {
		if (!nodeIter) break;
		nodeIter = nodeIter->next;
		nodesToGo--;
	}
	if (!nodeIter) {
		printf("Error - index is too large!\n");
		return STATUS_CODE_ERROR;
	}
    else if (!nodeIter->next && !nodeIter->prev) {	  // list with 1 element
        *head = NULL;
    }
	else if (!nodeIter->next) { // tail node
		(nodeIter->prev)->next = NULL;
	}
	else if (!nodeIter->prev) { // head node
		(nodeIter->next)->prev = NULL;
		*head = nodeIter->next;
	}
	else {					 //middle node
		(nodeIter->prev)->next = nodeIter->next;
		(nodeIter->next)->prev = nodeIter->prev;
	}
	free(nodeIter);
	return STATUS_CODE_SUCCESS;
}

void PrintList(struct node *head) {
	struct node *nodeIter = head;
	printf("[");
	while (nodeIter) {
		if (nodeIter != head) printf(", ");
		printf("%d", nodeIter->value);
		nodeIter = nodeIter->next;
	}
	printf("]\n");
}

void FreeLinkedList(struct node *head) {
	struct node *nodeIter = head, *nodeToFree = NULL;
	while (nodeIter) {
		nodeToFree = nodeIter;
		nodeIter = nodeIter->next;
		free(nodeToFree);
	}
}
