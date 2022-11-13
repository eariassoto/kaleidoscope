#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

#include "ast/expression.h"

#include <string>

namespace kaleidoscope::ast
{
struct Variable : public Expression
{
    Variable(std::string name);
    
    std::string Name;
};

} // namespace kaleidoscope::ast

#endif  // AST_VARIABLE_H