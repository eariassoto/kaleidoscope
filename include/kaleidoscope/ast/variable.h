#ifndef KALEIDOSCOPE_AST_VARIABLE_H
#define KALEIDOSCOPE_AST_VARIABLE_H

#include "kaleidoscope/ast/expression.h"

#include <string_view>

namespace kaleidoscope::ast
{
struct Variable : public Expression {
    Variable(std::string_view name);

    std::string_view Name;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_VARIABLE_H