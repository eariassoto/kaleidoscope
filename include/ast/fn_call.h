#ifndef AST_FN_CALL_H
#define AST_FN_CALL_H

#include "ast/expression.h"

#include <memory>
#include <string>
#include <vector>

namespace kaleidoscope::ast
{
struct FnCall : public Expression {
    FnCall(const std::string &callee,
              std::vector<std::unique_ptr<Expression>> args);

    std::string Callee;
    std::vector<std::unique_ptr<Expression>> Args;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_CALL_H