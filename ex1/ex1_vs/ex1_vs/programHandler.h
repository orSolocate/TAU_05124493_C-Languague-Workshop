#ifndef PROGRAM_HANDLER_H
#define PROGRAM_HANDLER_H

#include "list.h"

#define MAX_OPCODE_LEN 10

struct line {
	char opcode[MAX_OPCODE_LEN];
	int firstArg;
	int secondArg;
};

typedef enum { KEEP_GOING, EXIT } Progress_t;


void ParseLine(char *lineStr, struct line *parsedLine);
Progress_t ActAccoirdingToOpcode(const struct line *line, struct node **head);

#endif