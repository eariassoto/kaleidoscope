#ifndef AST_BINARY_OP_H
#define AST_BINARY_OP_H

#include "ast/expression.h"

#include <memory>

namespace kaleidoscope::ast
{
struct BinaryOp : public Expression {
    BinaryOp(char op, std::unique_ptr<Expression> lhs_op,
             std::unique_ptr<Expression> rhs_op);

    char Op;
    std::unique_ptr<Expression> LhsOp;
    std::unique_ptr<Expression> RhsOp;
};

}  // namespace kaleidoscope::ast

#endif  // AST_BINARY_OP_H