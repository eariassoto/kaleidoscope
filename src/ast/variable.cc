#include "ast/variable.h"

#include <fmt/core.h>

namespace kaleidoscope::ast
{
Variable::Variable(std::string_view name) : Name(name) {}

void Variable::PrintToString(std::string& out_str, size_t indent_level,
                           char space_char, size_t indent_size)
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str += fmt::format("var={}\n", Name);
}

}  // namespace kaleidoscope::ast
