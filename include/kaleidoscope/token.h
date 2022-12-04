#ifndef KALEIDOSCOPE_TOKEN_H
#define KALEIDOSCOPE_TOKEN_H

#include <string_view>

namespace kaleidoscope
{
enum class TokenType {
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

constexpr const char* TokenTypeToString(TokenType t) noexcept
{
    switch (t) {
        case TokenType::kEof:
            return "EOF";

        // reserved words
        case TokenType::kDef:
            return "DEF";
        case TokenType::kExtern:
            return "EXTERN";

        // variable values
        case TokenType::kIdentifier:
            return "ID";
        case TokenType::kNumber:
            return "NUMBER";

        // characters
        case TokenType::kLeftParen:
            return "LEFT_PAREN";
        case TokenType::kRightParen:
            return "RIGHT_PAREN";
        case TokenType::kPlusSign:
            return "PLUS";
        case TokenType::kMinusSign:
            return "MINUS";
        case TokenType::kAsterisk:
            return "ASTERISK";
        case TokenType::kComma:
            return "COMMA";
        case TokenType::kDot:
            return "DOT";
    }
}

struct Token {
    Token() = delete;
    ~Token();
    // Consider deleting
    // Token(const Token& t) = delete;
    // Token& operator=(const Token&) = delete;
    Token(TokenType type, std::string_view value);

    TokenType Type;
    std::string_view Value;
};

}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_TOKEN_H
