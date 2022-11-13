#include "ast/fn_prototype.h"

namespace kaleidoscope::ast
{
FnPrototype::FnPrototype(const std::string &name, std::vector<std::string> args)
    : Name(name), Args(std::move(args))
{
}

}  // namespace kaleidoscope::ast
