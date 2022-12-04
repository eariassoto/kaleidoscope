#ifndef KALEIDOSCOPE_AST_EXPRESSION_H
#define KALEIDOSCOPE_AST_EXPRESSION_H

#include <string>

namespace kaleidoscope::ast
{
class BaseExpression
{
   public:
    virtual ~BaseExpression() = default;

    virtual void PrintToString(std::string& out_str, size_t indent_level = 0,
                               char space_char = ' ',
                               size_t indent_size = 2) const = 0;
};

}  // namespace kaleidoscope::ast

#endif // KALEIDOSCOPE_AST_EXPRESSION_H