#ifndef LIST_H
#define LIST_H

struct node {
	int value;
	struct node *next;
	struct node *prev;
};

typedef enum { END_OF_LIST, START_OF_LIST, AFTER_VALUE } AddToListPosition_t;

int AddNodeToList(struct node **head, const int value, const AddToListPosition_t position, const int afterValue);
void PrintElementInList(struct node *head, const int value);
int DeleteIndexFromList(struct node **head, const int index);
void PrintList(struct node *head);
void FreeLinkedList(struct node *head);

#endif
