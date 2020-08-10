#include "RegularExpressions.h"
#include <assert.h>
#include <stdlib.h>

#define BACKSLASH_OFFSET_TERM_B 3

void InitExpression(struct Expression *exp, regularExpType type, char *line);
void InitSquareBracket(struct Expression *exp, const char *str);
int CalcStepRegardType(const char *str, regularExpType type);
void FreeBracketBranch(struct Expression *branch, struct Expression *nextNode);
void InitBrackets(char *str, struct Expression *bracketsExp, bool isHeadNode);
void ConnectBracketBrnach(struct Expression **bracketBranch, struct Expression *nextExp);
int CalcStepBrackets(const char *str, char stopChar);


struct Expression *ParseExpression(char *str)
{
  char *strIter = str;
  regularExpType expType;
  int steps;
  struct Expression *expressionHead = NULL, *expressionTail = NULL;
  while (*strIter != '\n' && *strIter != '\0') {
    struct Expression *iterExp = (struct Expression *)malloc(sizeof(struct Expression));
    if (*strIter == '(') {
      expType = BRACKET;
    } else if (*strIter == '[') {
      expType = SQUARE_BRACKET;
    } else if (*strIter == '.' || *strIter == '\\') {
      expType = SPECIAL_CHAR;
    } else {
      expType = CHAR;
    }
    steps = CalcStepRegardType(strIter, expType);
    InitExpression(iterExp, expType, strIter);
    if (iterExp->type == BRACKET) {
      InitBrackets((strIter + 1), iterExp, !expressionHead);
    }
    strIter += steps;
    if (!expressionTail) {
      expressionTail = iterExp;
      expressionHead = iterExp;
    } else {
      if (expressionTail->type == BRACKET) {
        ConnectBracketBrnach(&(expressionTail->node->brackets.splitAHead), iterExp);
        ConnectBracketBrnach(&(expressionTail->node->brackets.splitBHead), iterExp);
      }
      expressionTail->next = iterExp;
      expressionTail = iterExp;
    }
  }
  return expressionHead;
}


void InitExpression(struct Expression *exp, regularExpType type, char *line)
{
  exp->node = (union regularExp *)malloc(sizeof(union regularExp));
  exp->type = type;
  exp->next = NULL;
  switch (type) {
    case CHAR: {
      (*(exp->node)).regChar = *line;
      break;
    }
    case SPECIAL_CHAR: {
      if (*line == '.') {
        exp->node->specialChar.subType = DOT;
        exp->node->specialChar.sChar = *line;
      } else {
        exp->node->specialChar.subType = ESCAPE;
        exp->node->specialChar.sChar = *(line + 1);
      }
      break;
    }
    case BRACKET: {
      exp->node->brackets.splitAHead = NULL;
      exp->node->brackets.splitBHead = NULL;
      break;
    }
    case SQUARE_BRACKET: {
      InitSquareBracket(exp, line);
      break;
    }
    default:
      return;
  }
}


void InitSquareBracket(struct Expression *exp, const char *str)
{
  assert(exp->type == SQUARE_BRACKET && *str == '[');
  int jump = 0;
  if (*(str + 1) == '\\') {
    jump++;
  }
  exp->node->squareBrackets.startRange = *(str + 1 + jump);
  if (*(str + BACKSLASH_OFFSET_TERM_B + jump) == '\\') {
    jump++;
  }
  exp->node->squareBrackets.endRange = *(str + BACKSLASH_OFFSET_TERM_B + jump);
}


void InitBrackets(char *str, struct Expression *bracketsExp, bool isHeadNode)
{
  int iter = 0, step;
  bool proccessFirstStr = true;
  struct Expression *iterExp = NULL, *tailExp = NULL;
  regularExpType type;
  bracketsExp->node->brackets.isHeadNode = isHeadNode;
  while (*(str + iter) != ')') {
    type = CHAR;
    step = 1;
    if (*(str + iter) == '|') {
      proccessFirstStr = false;
      tailExp = NULL;  // finished first string branch
      iter++;
      continue;  // reached '|'. continue to second string
    }
    if (*(str + iter) == '\\') {
      type = SPECIAL_CHAR;
      step = 2;
    }
    iterExp = (struct Expression *)malloc(sizeof(struct Expression));
    InitExpression(iterExp, type, (str + iter));
    if (!tailExp) {
      if (proccessFirstStr) {
        bracketsExp->node->brackets.splitAHead = iterExp;
      } else {
        bracketsExp->node->brackets.splitBHead = iterExp;
      }
    } else {
      tailExp->next = iterExp;
    }
    tailExp = iterExp;
    iter += step;
  }
}


int CalcStepRegardType(const char *str, regularExpType type)
{
  int steps = 0;
  char stopChar;
  if (type == SPECIAL_CHAR) {
    if (*str == '\\') {
      steps = 2;
    } else {
      steps = 1;
    }
  } else if (type == CHAR) {
    steps = 1;
  } else if (type == SQUARE_BRACKET) {
    stopChar = ']';
    steps = CalcStepBrackets(str, stopChar);
  } else if (type == BRACKET) {
    stopChar = ')';
    steps = CalcStepBrackets(str, stopChar);
  } else {
    assert(false && "incorrect type");
  }
  return steps;
}


int CalcStepBrackets(const char *str, char stopChar)
{
  int steps = 0;
  while (*(str + steps) != stopChar) {
    steps++;
  }
  return steps + 1;
}


void ConnectBracketBrnach(struct Expression **bracketBranch, struct Expression *nextExp)
{
  struct Expression *iterExp = NULL;
  if (!(*bracketBranch)) {
    *bracketBranch = nextExp;
  } else {
    iterExp = *bracketBranch;
    while (iterExp->next) {
      iterExp = iterExp->next;
    }
    iterExp->next = nextExp;
  }
}


bool IsEndOfExpression(struct Expression *expNode)
{
  if (!expNode) {
    return true;
  } else if (expNode->type == BRACKET) {
    return (IsEndOfExpression(expNode->node->brackets.splitAHead) ||
            IsEndOfExpression(expNode->node->brackets.splitBHead));
  }
  return false;
}


void FreeExpression(struct Expression *expHead)
{
  struct Expression *currExp = expHead, *prevExp = NULL;
  while (currExp) {
    if (currExp->type == BRACKET) {
      FreeBracketBranch(currExp->node->brackets.splitAHead, currExp->next);
      FreeBracketBranch(currExp->node->brackets.splitBHead, currExp->next);
    }
    prevExp = currExp;
    currExp = currExp->next;
    free(prevExp->node);
    free(prevExp);
  }
}


void FreeBracketBranch(struct Expression *branch, struct Expression *nextNode)
{
  struct Expression *currExp = branch, *prevExp = NULL;
  while (currExp != nextNode) {
    prevExp = currExp;
    currExp = currExp->next;
    free(prevExp->node);
    free(prevExp);
  }
}