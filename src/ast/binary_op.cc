#include "ast/binary_op.h"

#include <fmt/core.h>

namespace kaleidoscope::ast
{
BinaryOp::BinaryOp(char op, std::unique_ptr<Expression> lhs_op,
                   std::unique_ptr<Expression> rhs_op)
    : Op(op), LhsOp(std::move(lhs_op)), RhsOp(std::move(rhs_op))
{
}

void BinaryOp::PrintToString(std::string& out_str, size_t indent_level,
                             char space_char, size_t indent_size)
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str += fmt::format("bin op={}\n", Op);

    out_str += fmt::format("{: >{}}", "", (indent_level + 1) * indent_size);
    out_str += fmt::format("lhs=\n");
    LhsOp->PrintToString(out_str, indent_level + 2, space_char, indent_size);

    out_str += fmt::format("{: >{}}", "", (indent_level + 1) * indent_size);
    out_str += fmt::format("rhs=\n");
    RhsOp->PrintToString(out_str, indent_level + 2, space_char, indent_size);
}

}  // namespace kaleidoscope::ast
