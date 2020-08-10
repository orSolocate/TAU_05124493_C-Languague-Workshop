#include "InputParser.h"
#include "RegularExpressions.h"
#include <stdbool.h>
#include <stdlib.h>

void ParseFlag(char **inputOptions, unsigned int *flagBitmask, int *iter, int *AFlagValue);

void InitParsedInput(struct ParsedInput *inputStruct)
{
  inputStruct->AFlagValue = A_FLAG_NOT_SET;
  inputStruct->fileName = NULL;
  inputStruct->flagBitmask = 0;
  inputStruct->expression = NULL;
}

void ParseInput(char **inputOptions, int numOfArgs, struct ParsedInput *inputStruct)
{
  char *currOption = NULL, *strToSearch = NULL;
  bool wordParsed = false;
  int i = 1, *AFlagValue = &(inputStruct->AFlagValue);
  unsigned int *flagBitmask = &(inputStruct->flagBitmask);
  while (i < numOfArgs) {
    currOption = *(inputOptions + i);
    if (*currOption != '-' && wordParsed == false) {
      strToSearch = currOption;
      wordParsed = true;
    } else if (*currOption == '-') {
      ParseFlag(inputOptions, flagBitmask, &i, AFlagValue);
    } else if (i == numOfArgs - 1) {  // argument at the end of inputLine
      inputStruct->fileName = *(inputOptions + i);
    }
    i++;
  }
  inputStruct->expression = ParseExpression(strToSearch);
}

void ParseFlag(char **inputOptions, unsigned int *flagBitmask, int *iter, int *AFlagValue)
{
  char option = *(*(inputOptions + *iter) + 1);  // +1 to skip the '-' char
  switch (option) {
    case 'A': {
      *flagBitmask |= A_FLAG;
      *AFlagValue = atoi(*(inputOptions + *iter + 1));  // gets value from next param
      (*iter)++;
      break;
    }
    case 'b': {
      *flagBitmask |= b_FLAG;
      break;
    }
    case 'c': {
      *flagBitmask |= c_FLAG;
      break;
    }
    case 'i': {
      *flagBitmask |= i_FLAG;
      break;
    }
    case 'n': {
      *flagBitmask |= n_FLAG;
      break;
    }
    case 'v': {
      *flagBitmask |= v_FLAG;
      break;
    }
    case 'x': {
      *flagBitmask |= x_FLAG;
      break;
    }
    case 'E': {
      *flagBitmask |= E_FLAG;
      break;
    }
    default:
      break;
  }
}

void FreeInputStruct(struct ParsedInput *inputStruct)
{
  FreeExpression(inputStruct->expression);
  free(inputStruct);
}