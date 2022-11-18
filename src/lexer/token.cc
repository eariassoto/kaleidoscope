#include "lexer/token.h"

#include <stdexcept>

namespace kaleidoscope
{

Token::~Token() = default;

Token::Token(TokenType type) : type_(type)
{
    if (type_ == TokenType::kIdentifier || type_ == TokenType::kNumber ||
        type_ == TokenType::kCharacter) {
        throw std::invalid_argument("Token type must have a value.");
    }
}

Token::Token(TokenType type, std::string_view value)
    : type_(type), value_(value)
{
    if (type_ != TokenType::kIdentifier && type_ != TokenType::kNumber &&
        type_ != TokenType::kCharacter) {
        throw std::invalid_argument("Token type cannot have a value.");
    }
}

TokenType Token::GetType() const { return type_; }

std::string_view Token::GetValue() const
{
    if (type_ != TokenType::kIdentifier && type_ != TokenType::kNumber &&
        type_ != TokenType::kCharacter) {
        throw std::invalid_argument("Token type does not have a value.");
    }
    return value_;
}

}  // namespace kaleidoscope
