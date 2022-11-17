#include "lexer/token.h"

namespace kaleidoscope
{

Token::~Token() = default;

Token::Token(Type type) : type_(type) {}

Token::Token(std::string_view identifier_value)
    : type_(Type::kIdentifier), value_(identifier_value)
{
}

Token::Token(double number_value) : type_(Type::kNumber), value_(number_value)
{
}

Token::Token(char character_value)
    : type_(Type::kCharacter), value_(character_value)
{
}

bool Token::IsCharToken(char value) const
{
    if (type_ != Type::kCharacter) return false;
    return value == std::get<char>(value_);
}

/*static*/ std::unique_ptr<Token> Token::CreateEof()
{
    return std::unique_ptr<Token>(new Token(Type::kEof));
}

/*static*/ std::unique_ptr<Token> Token::CreateDef()
{
    return std::unique_ptr<Token>(new Token(Type::kDef));
}

/*static*/ std::unique_ptr<Token> Token::CreateExtern()
{
    return std::unique_ptr<Token>(new Token(Type::kExtern));
}

/*static*/ std::unique_ptr<Token> Token::CreateIdentifier(std::string_view name)
{
    return std::unique_ptr<Token>(new Token(name));
}

/*static*/ std::unique_ptr<Token> Token::CreateNumber(double value)
{
    return std::unique_ptr<Token>(new Token(value));
}

/*static*/ std::unique_ptr<Token> Token::CreateCharacter(char value)
{
    return std::unique_ptr<Token>(new Token(value));
}

}  // namespace kaleidoscope
