#ifndef AST_FN_PROTOTYPE_H
#define AST_FN_PROTOTYPE_H

#include "ast/expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace kaleidoscope::ast
{
struct FnPrototype : public Expression {
    FnPrototype(const std::string_view& name,
                std::vector<std::string_view> args);

    std::string_view Name;
    std::vector<std::string_view> Args;

    void PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size) final;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_PROTOTYPE_H