#include "ast/binary_op.h"

namespace kaleidoscope::ast
{
    BinaryOp::BinaryOp(char op, std::unique_ptr<Expression> lhs_op,
                std::unique_ptr<Expression> rhs_op)
    : Op(op), LhsOp(std::move(lhs_op)), RhsOp(std::move(rhs_op)) {}

}  // namespace kaleidoscope::ast
