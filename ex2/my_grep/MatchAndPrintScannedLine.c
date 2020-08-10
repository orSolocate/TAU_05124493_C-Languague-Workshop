#include "MatchAndPrintScannedLine.h"
#include "RegularExpressions.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool IslineEndsString(char *line);
void PrintLineMatched(struct Line **Inspectedline, unsigned int bitmask, int *numOfLinesToPrint, bool *EndOfSection);
void PrintLineAFlag(struct Line **Inspectedline, unsigned int bitmask, bool *EndOfSection, int AflagValue);
bool IsMatchInLine(char *line, struct Expression *expNode, bool isHead, const unsigned int bitmask);
bool IsMatchAtChar(const char *lineChar, const char *charToMatch, const unsigned int bitmask,
                   int rangeStart, int rangeEnd);


void InitLineStruct(struct Line *currLine, struct Line *prevLine, char *line, const int lineNum, const int offset) {
    if (prevLine!=NULL) {
        prevLine->nextLine = currLine;
    }
    currLine->nextLine = NULL;
    currLine->lineOffset = offset;
    currLine->lineNum = lineNum;
    currLine->IsMatchedToBePrinted = false;
    currLine->AFlagCounter= A_FLAG_NOT_SET;
    if (!IslineEndsString(line)){
        currLine->data = (char*)malloc((strlen(line) * sizeof(char)) + 2); // +2 for "\n\0"
        strcpy(currLine->data, line);
        strcat(currLine->data,"\n");
    }
    else {
        currLine->data = (char *) malloc((strlen(line) * sizeof(char)) + 1); // +1 for '\0'
        strcpy(currLine->data, line);
    }
}


bool IslineEndsString(char *line){
    int len=strlen(line);
    if (line[len-1]=='\n'){
        return true;
    }
    return false;
}

bool IsLineMatchedToBePrinted(char *line, const unsigned int bitmask, struct Expression *expression) {
    if (HAS_OPTION(bitmask, v_FLAG)) {
        return !(IsMatchInLine(line, expression, true, bitmask));
    }
    else {
        return IsMatchInLine(line, expression, true, bitmask);
    }
}


bool IsMatchInLine(char *line, struct Expression *expNode, bool isHead, const unsigned int bitmask) {
    struct Expression *expCurr = expNode;
    bool nodeMatched;
    int lineOffset = 0, lineOffsetLastIter = 0;
    while (expCurr) {
        if (*(line + lineOffset) == '\n' || *(line + lineOffset) == '\0') {
            return IsEndOfExpression(expCurr);
        }
        if (expCurr->type==CHAR) {
            nodeMatched = IsMatchAtChar((line + lineOffset), &((expCurr->node)->regChar), bitmask, 0, 0);
        }
        else if (expCurr->type==SPECIAL_CHAR) {
            if (expCurr->node->specialChar.subType == DOT) {
                nodeMatched = true;
            }
            else {
                nodeMatched = IsMatchAtChar((line + lineOffset), &((expCurr->node)->specialChar.sChar), bitmask, 0, 0);
            }
        }
        else if (expCurr->type==BRACKET) {
            if (IsMatchInLine((line + lineOffset),
                    expCurr->node->brackets.splitAHead, expCurr->node->brackets.isHeadNode,bitmask)) {
                return true;
            }
            if (IsMatchInLine((line + lineOffset),
                    expCurr->node->brackets.splitBHead, expCurr->node->brackets.isHeadNode,bitmask)) {
                return true;
            }
            nodeMatched = false;
        }
        else if (expCurr->type==SQUARE_BRACKET) {
            nodeMatched = IsMatchAtChar((line + lineOffset), NULL, bitmask,
                    expCurr->node->squareBrackets.startRange, expCurr->node->squareBrackets.endRange);
        }
        if (nodeMatched) {
            lineOffset++;
            expCurr = expCurr->next;
        }
        else {
            if (HAS_OPTION(bitmask, x_FLAG) || !isHead) {
                return false;
            }
            expCurr = expNode;
            lineOffsetLastIter++;
            lineOffset = lineOffsetLastIter;
        }
    }
    if (HAS_OPTION(bitmask, x_FLAG)) {
        return (*(line + lineOffset) == '\n' || *(line + lineOffset) == '\0');
    }
    return true;
}


bool IsMatchAtChar(const char *lineChar, const char *charToMatch, const unsigned int bitmask,
                   int rangeStart, int rangeEnd) {
    if (HAS_OPTION(bitmask, i_FLAG)) {
        if (HAS_OPTION(bitmask, E_FLAG) && !charToMatch) {
	    if (tolower(*lineChar) < tolower(rangeStart) || tolower(*lineChar) > tolower(rangeEnd)) {
                return false;
            }
        }
        else {
            if (tolower(*charToMatch) - tolower(*lineChar) != 0) {
                return false;
            }
        }
    }
    else {
        if (HAS_OPTION(bitmask, E_FLAG) && !charToMatch) {
	        if (*lineChar < rangeStart || *lineChar > rangeEnd) {
		    return false;
	        }
	    }
	    else {
            if (*charToMatch - *lineChar != 0) {
                return false;
            }
	    }
    }
    return true;
}


void updateAflagInLine(struct Line *currLine, struct Line *prevLine, int AFlagValue) {
    if (AFlagValue == A_FLAG_NOT_SET) {
        return;
    }
    if (currLine->IsMatchedToBePrinted) {
        currLine->AFlagCounter = 0;
        return;
    }
    if (!prevLine) {
        return;
    }
    if ((prevLine->AFlagCounter > AFlagValue) || (prevLine->AFlagCounter == A_FLAG_NOT_SET)) {
        //no match but no need to print current line
        return;
    }
    //implied PrintLineAFlag
    currLine->AFlagCounter = prevLine->AFlagCounter + 1;
}


void PrintLines(struct Line *Inspectedline, const unsigned int bitmask, int AflagValue) {
    int numOfLinesToPrint = 0;
    bool EndOfSection=false;
    while (Inspectedline != NULL) {
        if (Inspectedline->IsMatchedToBePrinted) {
            PrintLineMatched(&Inspectedline, bitmask, &numOfLinesToPrint, &EndOfSection);
        }
        else if (HAS_OPTION(bitmask, A_FLAG)){
            PrintLineAFlag(&Inspectedline, bitmask, &EndOfSection, AflagValue);
        }
        Inspectedline = Inspectedline->nextLine;
    }
    if (HAS_OPTION(bitmask, c_FLAG)) {
        printf("%d\n", numOfLinesToPrint);
        return;
    }
}

void PrintLineMatched(struct Line **Inspectedline, unsigned int bitmask, int *numOfLinesToPrint, bool *EndOfSection) {
    if (HAS_OPTION(bitmask, c_FLAG)) {
        (*numOfLinesToPrint)++;
        return;
    }
    if ((*EndOfSection == true) && ((*Inspectedline)->AFlagCounter != A_FLAG_NOT_SET)) {
        printf("--\n");
        *EndOfSection = false;
    }
    if (HAS_OPTION(bitmask, n_FLAG)) {
        printf("%d:", (*Inspectedline)->lineNum);
    }
    if (HAS_OPTION(bitmask, b_FLAG)) {
        printf("%d:", (*Inspectedline)->lineOffset);
    }
    printf("%s", (*Inspectedline)->data);
    return;
}

void PrintLineAFlag(struct Line **Inspectedline, unsigned int bitmask, bool *EndOfSection, int AflagValue) {
    if (HAS_OPTION(bitmask, c_FLAG)) {
       return;
    }
    if ((*Inspectedline)->AFlagCounter == AflagValue + 1) {
        *EndOfSection = true;
        return;
    }
    if (HAS_OPTION(bitmask, n_FLAG)) {
        if ((*Inspectedline)->AFlagCounter > 0) { // separation for input '-A 0'
            if ((*Inspectedline)->AFlagCounter <= AflagValue) {
                printf("%d-", (*Inspectedline)->lineNum);
            }
        }
    }
    if (HAS_OPTION(bitmask, b_FLAG)) {
        if ((*Inspectedline)->AFlagCounter > 0) {
            if ((*Inspectedline)->AFlagCounter <= AflagValue) {
                printf("%d-", (*Inspectedline)->lineOffset);
            }
        }
    }
    if ((*Inspectedline)->AFlagCounter != A_FLAG_NOT_SET) {
        printf("%s", (*Inspectedline)->data);
    }
}


void FreeLines(struct Line *lineListHead) {
    while (lineListHead != NULL) {
        struct Line *lineToFree = lineListHead;
        lineListHead = lineListHead->nextLine;
        free(lineToFree->data);
        free(lineToFree);
    }
}