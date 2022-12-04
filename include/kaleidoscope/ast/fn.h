#ifndef KALEIDOSCOPE_AST_FN_H
#define KALEIDOSCOPE_AST_FN_H

#include "kaleidoscope/ast/base_expression.h"
#include "kaleidoscope/ast/fn_prototype.h"

#include <memory>

namespace kaleidoscope::ast
{
struct Fn : public BaseExpression {
    Fn(std::unique_ptr<FnPrototype> proto, std::unique_ptr<BaseExpression> body);

    char Op;
    std::unique_ptr<FnPrototype> Proto;
    std::unique_ptr<BaseExpression> Body;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_FN_H