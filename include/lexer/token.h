#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string_view>

namespace kaleidoscope
{
enum class TokenType {
    kEof,

    // reserved words
    kDef,
    kExtern,

    kIdentifier,
    kNumber,
    kCharacter
};

class Token
{
   public:
    Token() = delete;
    explicit Token(TokenType type);
    Token(TokenType type, std::string_view value);

    ~Token();

    TokenType GetType() const;
    [[nodiscard]] std::string_view GetValue() const;

   private:
    TokenType type_;
    std::string_view value_ = std::string_view(nullptr, 0);
};

}  // namespace kaleidoscope

#endif  // LEXER_TOKEN_H
