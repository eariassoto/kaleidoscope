#ifndef KALEIDOSCOPE_TOKEN_H
#define KALEIDOSCOPE_TOKEN_H

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

#endif // KALEIDOSCOPE_TOKEN_H
