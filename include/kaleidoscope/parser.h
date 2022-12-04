#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include "ast/expression.h"
// #include "ast/fn.h"
// #include "ast/fn_prototype.h"

#include <memory>
// #include <string>

namespace kaleidoscope
{

class Lexer;
// enum class TokenType;

namespace parser
{
std::unique_ptr<ast::Expression> ParseNextExpression(Lexer* lexer);
}
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_PARSER_H