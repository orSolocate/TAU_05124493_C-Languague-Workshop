#include "InputParser.h"
#include "MatchAndPrintScannedLine.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_CODE_SUCCESS 0
#define STATUS_CODE_FAILURE -1

int main(int argc, char **argv)
{
  struct ParsedInput *inputStruct = (struct ParsedInput *)malloc(sizeof(struct ParsedInput));
  struct Line *prevLine = NULL, *currLine = NULL, *lineListHead = NULL;
  FILE *file = NULL;
  char *ScannedLine = NULL;
  size_t n = 0;
  int lineNum = 1, totalOffset = 0, validInput;
  InitParsedInput(inputStruct);
  ParseInput(argv, argc, inputStruct);
  bool HAS_FILE_TO_SCAN = inputStruct->fileName != NULL;
  if (HAS_FILE_TO_SCAN) {
    file = fopen(inputStruct->fileName, "r");
    if (!file) {
      printf("Error - open file failed!\n");
      return (STATUS_CODE_FAILURE);
    }
    validInput = getline(&ScannedLine, &n, file) != EOF;
  } else {
    validInput = getline(&ScannedLine, &n, stdin) != EOF;
  }
  while (validInput) {
    currLine = (struct Line *)malloc(sizeof(struct Line));
    InitLineStruct(currLine, prevLine, ScannedLine, lineNum, totalOffset);
    currLine->IsMatchedToBePrinted =
        IsLineMatchedToBePrinted(ScannedLine, inputStruct->flagBitmask, inputStruct->expression);
    lineNum++;
    totalOffset += strlen(ScannedLine);
    updateAflagInLine(currLine, prevLine, inputStruct->AFlagValue);
    if (prevLine == NULL) {
      lineListHead = currLine;
    }
    prevLine = currLine;
    if (HAS_FILE_TO_SCAN) {
      if (getline(&ScannedLine, &n, file) == EOF) {
        fclose(file);
        validInput = 0;
      }
    } else {
      validInput = getline(&ScannedLine, &n, stdin) != EOF;
    }
  }
  PrintLines(lineListHead, inputStruct->flagBitmask, inputStruct->AFlagValue);
  FreeInputStruct(inputStruct);
  free(ScannedLine);
  FreeLines(lineListHead);
  return STATUS_CODE_SUCCESS;
}