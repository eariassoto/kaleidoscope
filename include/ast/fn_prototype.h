#ifndef AST_FN_PROTOTYPE_H
#define AST_FN_PROTOTYPE_H

#include "ast/expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace kaleidoscope::ast
{
struct FnPrototype : public Expression {
    FnPrototype(const std::string_view &name,
                std::vector<std::string_view> args);

    std::string_view Name;
    std::vector<std::string_view> Args;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_PROTOTYPE_H