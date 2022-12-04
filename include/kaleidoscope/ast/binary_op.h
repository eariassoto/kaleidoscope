#ifndef KALEIDOSCOPE_AST_BINARY_OP_H
#define KALEIDOSCOPE_AST_BINARY_OP_H

#include "kaleidoscope/ast/base_expression.h"

#include <memory>

namespace kaleidoscope::ast
{
struct BinaryOp : public BaseExpression {
    BinaryOp(char op, std::unique_ptr<BaseExpression> lhs_op,
             std::unique_ptr<BaseExpression> rhs_op);

    char Op;
    std::unique_ptr<BaseExpression> LhsOp;
    std::unique_ptr<BaseExpression> RhsOp;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_BINARY_OP_H