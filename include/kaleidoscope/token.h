#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string_view>

namespace kaleidoscope
{
enum class Token {
    kEof,

    // reserved words
    kDef,
    kExtern,

    // variable values
    kIdentifier,
    kNumber,

    // characters
    kLeftParen,
    kRightParen,
    kPlusSign,
    kMinusSign,
    kAsterisk,
    kComma,
    kDot
};

}  // namespace kaleidoscope

#endif  // LEXER_TOKEN_H
