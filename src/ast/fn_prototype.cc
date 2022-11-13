#include "ast/fn_prototype.h"

namespace kaleidoscope::ast
{
FnPrototype::FnPrototype(const std::string_view &name,
                         std::vector<std::string_view> args)
    : Name(name), Args(std::move(args))
{
}

}  // namespace kaleidoscope::ast
