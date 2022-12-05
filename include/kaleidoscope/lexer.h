#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/token.h"

#include <tl/expected.hpp>

#include <optional>

namespace kaleidoscope
{
class Lexer
{
   public:
    virtual ~Lexer() = default;

    virtual tl::expected<Token, LexerError> PeekToken() = 0;

    virtual void ConsumeToken() = 0;
};
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_LEXER_H