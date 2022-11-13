#ifndef AST_NUMBER_H
#define AST_NUMBER_H

#include "ast/expression.h"

namespace kaleidoscope::ast
{
struct Number : public Expression
{
    Number(double value);
    
    double Value;
};

} // namespace kaleidoscope::ast

#endif  // AST_NUMBER_H