#include "ast/number.h"

#include <fmt/core.h>

namespace kaleidoscope::ast
{
Number::Number(double value) : Value(value) {}

void Number::PrintToString(std::string& out_str, size_t indent_level,
                           char space_char, size_t indent_size)
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str += fmt::format("double={}\n", Value);
}

}  // namespace kaleidoscope::ast
