#ifndef REGULAR_EXPRESSIONS_H
#define REGULAR_EXPRESSIONS_H

#include <stdbool.h>

typedef enum {
  CHAR,
  SPECIAL_CHAR,
  BRACKET,
  SQUARE_BRACKET,
} regularExpType;

typedef enum {
  DOT,
  ESCAPE,
} SpecialSubType;

struct Brackets {
  struct Expression *splitAHead;
  struct Expression *splitBHead;
  bool isHeadNode;
};

struct SquareBrackets {
  char startRange;
  char endRange;
};

struct SpecialChar {
  char sChar;
  SpecialSubType subType;
};

union regularExp {
  char regChar;
  struct Brackets brackets;
  struct SquareBrackets squareBrackets;
  struct SpecialChar specialChar;
};

struct Expression {
  union regularExp *node;
  struct Expression *next;
  regularExpType type;
};

struct Expression *ParseExpression(char *line);
bool IsEndOfExpression(struct Expression *expNode);
void FreeExpression(struct Expression *expHead);

#endif  // REGULAR_EXPRESSIONS_H