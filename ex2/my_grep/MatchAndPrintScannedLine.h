#ifndef SCANNEDLINEPROCESSOR_H
#define SCANNEDLINEPROCESSOR_H

#include <stdbool.h>
#include "InputParser.h"
#include "RegularExpressions.h"

struct Line {
    char *data;
    int lineNum;
    int lineOffset;
    bool IsMatchedToBePrinted;
    int AFlagCounter;
    struct Line *nextLine;
};


void InitLineStruct(struct Line *currLine, struct Line *prevLine, char *line, const int lineNum, const int offset);
bool IsLineMatchedToBePrinted(char *line, const unsigned int bitmask, struct Expression *expression);
void updateAflagInLine(struct Line*currLine, struct Line *prevLine, int AFlagValue);
void PrintLines(struct Line *Inspectedline, const unsigned int bitmask, int AflagValue);
void FreeLines(struct Line *lineListHead);

#endif //SCANNEDLINEPROCESSOR_H

