
#include "kaleidoscope/token.h"

namespace kaleidoscope
{
Token::~Token() = default;

Token::Token(TokenType type, std::string_view value) : Type(type), Value(value)
{
}

}  // namespace kaleidoscope
