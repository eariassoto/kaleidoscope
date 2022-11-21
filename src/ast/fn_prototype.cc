#include "kaleidoscope/ast/fn_prototype.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

namespace kaleidoscope::ast
{
FnPrototype::FnPrototype(const std::string_view& name,
                         std::vector<std::string_view> args)
    : Name(name), Args(std::move(args))
{
}

void FnPrototype::PrintToString(std::string& out_str, size_t indent_level,
                                char space_char, size_t indent_size)
{
    out_str += fmt::format("{: >{}}", "", indent_level * indent_size);
    out_str +=
        fmt::format("proto= name={} args={}\n", Name, fmt::join(Args, ", "));
}

}  // namespace kaleidoscope::ast
