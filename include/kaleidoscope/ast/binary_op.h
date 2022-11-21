#ifndef AST_BINARY_OP_H
#define AST_BINARY_OP_H

#include "kaleidoscope/ast/expression.h"

#include <memory>

namespace kaleidoscope::ast
{
struct BinaryOp : public Expression {
    BinaryOp(char op, std::unique_ptr<Expression> lhs_op,
             std::unique_ptr<Expression> rhs_op);

    char Op;
    std::unique_ptr<Expression> LhsOp;
    std::unique_ptr<Expression> RhsOp;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) final;
};

}  // namespace kaleidoscope::ast

#endif  // AST_BINARY_OP_H