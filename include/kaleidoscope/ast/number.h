#ifndef AST_NUMBER_H
#define AST_NUMBER_H

#include "kaleidoscope/ast/expression.h"

namespace kaleidoscope::ast
{
struct Number : public Expression {
    Number(double value);

    double Value;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) final;
};

}  // namespace kaleidoscope::ast

#endif  // AST_NUMBER_H