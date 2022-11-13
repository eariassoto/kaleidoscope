#include "ast/fn_call.h"

namespace kaleidoscope::ast
{
FnCall::FnCall(const std::string &callee,
               std::vector<std::unique_ptr<Expression>> args)
    : Callee(callee), Args(std::move(args))
{
}

}  // namespace kaleidoscope::ast
