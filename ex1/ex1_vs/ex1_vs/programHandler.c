#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "programHandler.h"

#define NONE 0
#define OPCODE_IS(STR)  (strcmp(STR, line->opcode) == 0) 
#define MAX_ARG_LEN 11
		
int parseArg(char **lineIter, char arg[]);

void ParseLine(char *lineStr, struct line *parsedLine) {
	char *lineIter = lineStr, *opcode = parsedLine->opcode, arg[MAX_ARG_LEN];
	while (*lineIter != '\0' && *lineIter != ' ') {
		*opcode = toupper(*lineIter);
		opcode++;
		lineIter++;
	}
	*opcode = '\0';
	if (*lineIter == '\0') return;
	lineIter++;	
	parsedLine->firstArg = parseArg(&lineIter, arg);
	if (*lineIter == '\0') return;
	lineIter++;
	parsedLine->secondArg = parseArg(&lineIter, arg);
}

int parseArg(char **lineIter, char arg[])
{
	int i = 0;
	while (**lineIter != '\0' && **lineIter != ' ') {
		arg[i] = **lineIter;
		i++;
		(*lineIter)++;
	}
	arg[i] = '\0';
	return atoi(arg);
}

Progress_t ActAccoirdingToOpcode(const struct line *line, struct node **head) {
	Progress_t status = KEEP_GOING;
	if (OPCODE_IS("EXIT")) {
		status = EXIT;
	}
	else if (OPCODE_IS("ADD_END")) {
		if (AddNodeToList(head, line->firstArg, END_OF_LIST, NONE)) status = EXIT;
	}
	else if (OPCODE_IS("ADD_START")) {
		if (AddNodeToList(head, line->firstArg, START_OF_LIST, NONE)) status = EXIT;
	}
	else if (OPCODE_IS("ADD_AFTER")) {
		if (AddNodeToList(head, line->firstArg, AFTER_VALUE, line->secondArg)) status = EXIT;
	}
	else if (OPCODE_IS("INDEX")) {
		PrintElementInList(*head, line->firstArg);
	}
	else if (OPCODE_IS("DEL")) {
		if (DeleteIndexFromList(head, line->firstArg)) status = EXIT;
	}
	else if (OPCODE_IS("PRINT")) {
		PrintList(*head);
	}
	return status;
}
