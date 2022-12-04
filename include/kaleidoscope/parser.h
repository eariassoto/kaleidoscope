#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include "ast/base_expression.h"

#include <memory>

namespace kaleidoscope
{

class Lexer;

namespace parser
{
std::unique_ptr<ast::BaseExpression> ParseNextExpression(Lexer* lexer);
}
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_PARSER_H