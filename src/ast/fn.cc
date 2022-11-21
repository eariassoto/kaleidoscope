#include "kaleidoscope/ast/fn.h"

#include <fmt/core.h>

namespace kaleidoscope::ast
{
Fn::Fn(std::unique_ptr<FnPrototype> proto, std::unique_ptr<Expression> body)
    : Proto(std::move(proto)), Body(std::move(body))
{
}

void Fn::PrintToString(std::string& out_str, size_t indent_level,
                       char space_char, size_t indent_size)
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str += fmt::format("fn=\n");
    Proto->PrintToString(out_str, indent_level + 1, space_char, indent_size);

    out_str += fmt::format("{: >{}}", "", (indent_level + 1) * indent_size);
    out_str += fmt::format("body=\n");
    Body->PrintToString(out_str, indent_level + 1, space_char, indent_size);
}

}  // namespace kaleidoscope::ast
