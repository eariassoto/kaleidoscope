#ifndef KALEIDOSCOPE_AST_FN_PROTOTYPE_H
#define KALEIDOSCOPE_AST_FN_PROTOTYPE_H

#include "kaleidoscope/ast/base_expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace kaleidoscope::ast
{
struct FnPrototype : public BaseExpression {
    FnPrototype(const std::string_view& name,
                std::vector<std::string_view> args);

    std::string_view Name;
    std::vector<std::string_view> Args;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) const final;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_FN_PROTOTYPE_H