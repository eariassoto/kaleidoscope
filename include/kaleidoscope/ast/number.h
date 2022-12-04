#ifndef KALEIDOSCOPE_AST_NUMBER_H
#define KALEIDOSCOPE_AST_NUMBER_H

#include "kaleidoscope/ast/base_expression.h"

namespace kaleidoscope::ast
{
struct Number : public BaseExpression {
    Number(double value);

    double Value;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_NUMBER_H