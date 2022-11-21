#ifndef AST_FN_H
#define AST_FN_H

#include "kaleidoscope/ast/fn_prototype.h"
#include "kaleidoscope/ast/expression.h"

#include <memory>

namespace kaleidoscope::ast
{
struct Fn : public Expression {
    Fn(std::unique_ptr<FnPrototype> proto, std::unique_ptr<Expression> body);

    char Op;
    std::unique_ptr<FnPrototype> Proto;
    std::unique_ptr<Expression> Body;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) final;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_H