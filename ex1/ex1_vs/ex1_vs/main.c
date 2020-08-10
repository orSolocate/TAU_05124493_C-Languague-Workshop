#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"
#include "programHandler.h"

#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LEN 100

int main() {
	struct node *head = NULL;
	char *inputLine = (char*)malloc(MAX_LINE_LEN * sizeof(char));
	Progress_t status = KEEP_GOING;
	while(true){
		scanf(" %[^\n]s", inputLine);
		struct line *currLine = (struct line*)malloc(sizeof(struct line));
                ParseLine(inputLine, currLine);
		status = ActAccoirdingToOpcode(currLine, &head);
		free(currLine);
		if (status == EXIT) break;
	}
	FreeLinkedList(head);
	free(inputLine);
	return 0;
}
