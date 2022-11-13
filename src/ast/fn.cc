#include "ast/fn.h"

namespace kaleidoscope::ast
{
Fn::Fn(std::unique_ptr<FnPrototype> proto, std::unique_ptr<Expression> body)
    : Proto(std::move(proto)), Body(std::move(body))
{
}

}  // namespace kaleidoscope::ast
