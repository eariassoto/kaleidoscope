#ifndef AST_FN_H
#define AST_FN_H

#include "ast/fn_prototype.h"
#include "ast/expression.h"

#include <memory>

namespace kaleidoscope::ast
{
struct Fn : public Expression {
    Fn(std::unique_ptr<FnPrototype> proto, std::unique_ptr<Expression> body);

    char Op;
    std::unique_ptr<FnPrototype> Proto;
    std::unique_ptr<Expression> Body;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_H