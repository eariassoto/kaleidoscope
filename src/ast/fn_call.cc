#include "ast/fn_call.h"

namespace kaleidoscope::ast
{
FnCall::FnCall(const std::string_view &callee,
               std::vector<std::unique_ptr<Expression>> args)
    : Callee(callee), Args(std::move(args))
{
}

}  // namespace kaleidoscope::ast
