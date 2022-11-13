#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

#include "ast/expression.h"

#include <string_view>

namespace kaleidoscope::ast
{
struct Variable : public Expression
{
    Variable(std::string_view name);
    
    std::string_view Name;
};

} // namespace kaleidoscope::ast

#endif  // AST_VARIABLE_H