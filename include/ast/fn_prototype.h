#ifndef AST_FN_PROTOTYPE_H
#define AST_FN_PROTOTYPE_H

#include "ast/expression.h"

#include <memory>
#include <string>
#include <vector>

namespace kaleidoscope::ast
{
struct FnPrototype : public Expression {
    FnPrototype(const std::string &name, std::vector<std::string> args);

    std::string Name;
    std::vector<std::string> Args;
};

}  // namespace kaleidoscope::ast

#endif  // AST_FN_PROTOTYPE_H