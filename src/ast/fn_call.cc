#include "kaleidoscope/ast/fn_call.h"

#include <fmt/core.h>

namespace kaleidoscope::ast
{
FnCall::FnCall(const std::string_view& callee,
               std::vector<std::unique_ptr<BaseExpression>> args)
    : Callee(callee), Args(std::move(args))
{
}

void FnCall::PrintToString(std::string& out_str, size_t indent_level,
                           char space_char, size_t indent_size) const
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str += fmt::format("fn call= callee={}\n", Callee);

    out_str += fmt::format("{: >{}}", "", (indent_level + 1) * indent_size);
    out_str += fmt::format("args=\n");

    for (const auto& e : Args) {
        e->PrintToString(out_str, indent_level + 1, space_char, indent_size);
    }
}

}  // namespace kaleidoscope::ast
