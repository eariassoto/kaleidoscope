#ifndef AST_FN_CALL_H
#define AST_FN_CALL_H

#include "ast/expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace kaleidoscope::ast
{
struct FnCall : public Expression {
    FnCall(const std::string_view &callee,
           std::vector<std::unique_ptr<Expression>> args);

    std::string_view Callee;
    std::vector<std::unique_ptr<Expression>> Args;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_CALL_H