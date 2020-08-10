#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "RegularExpressions.h"

#define A_FLAG 1 << 0
#define b_FLAG 1 << 1
#define c_FLAG 1 << 2
#define i_FLAG 1 << 3
#define n_FLAG 1 << 4
#define v_FLAG 1 << 5
#define x_FLAG 1 << 6
#define E_FLAG 1 << 7
#define HAS_OPTION(BITMASK, BIT) (BITMASK & BIT)
#define A_FLAG_NOT_SET -1

struct ParsedInput {
  unsigned int flagBitmask;
  char *fileName;
  int AFlagValue;
  struct Expression *expression;
};

void InitParsedInput(struct ParsedInput *inputStruct);
void ParseInput(char **inputOptions, int numOfArgs, struct ParsedInput *inputStruct);
void FreeInputStruct(struct ParsedInput *inputStruct);

#endif  // INPUT_PARSER_H