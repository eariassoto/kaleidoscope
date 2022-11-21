#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include <string>

namespace kaleidoscope::ast
{
class Expression
{
   public:
    virtual ~Expression() = default;

    virtual void PrintToString(std::string& out_str, size_t indent_level = 0,
                               char space_char = ' ',
                               size_t indent_size = 2) = 0;
};

}  // namespace kaleidoscope::ast

#endif  // AST_EXPRESSION_H