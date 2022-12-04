#ifndef KALEIDOSCOPE_AST_FN_CALL_H
#define KALEIDOSCOPE_AST_FN_CALL_H

#include "kaleidoscope/ast/base_expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace kaleidoscope::ast
{
struct FnCall : public BaseExpression {
    FnCall(const std::string_view& callee,
           std::vector<std::unique_ptr<BaseExpression>> args);

    std::string_view Callee;
    std::vector<std::unique_ptr<BaseExpression>> Args;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_FN_CALL_H