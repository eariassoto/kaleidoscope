#include "lexer/lexer_error.h"

#include <fmt/core.h>

namespace kaleidoscope
{
LexerError::~LexerError() = default;

LexerError::LexerError(char ch)
    : message_(fmt::format(
          "Could not continue parsing, unknown character found: '{}'", ch))

{
}

const char* LexerError::what() const noexcept { return message_.c_str(); }
}  // namespace kaleidoscope
