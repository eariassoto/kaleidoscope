#include "lexer/token2.h"

#include <charconv>

namespace kaleidoscope
{

Token2::~Token2() = default;

Token2::Token2(Type type) : type_(type) {}

Token2::Token2(std::string_view identifier_value)
    : type_(Type::kIdentifier), value_(identifier_value)
{
}

Token2::Token2(double number_value) : type_(Type::kNumber), value_(number_value)
{
}

Token2::Token2(char character_value)
    : type_(Type::kCharacter), value_(character_value)
{
}

/*static*/ std::unique_ptr<Token2> Token2::CreateEof()
{
    return std::unique_ptr<Token2>(new Token2(Type::kEof));
}

/*static*/ std::unique_ptr<Token2> Token2::CreateDef()
{
    return std::unique_ptr<Token2>(new Token2(Type::kDef));
}

/*static*/ std::unique_ptr<Token2> Token2::CreateExtern()
{
    return std::unique_ptr<Token2>(new Token2(Type::kExtern));
}

/*static*/ std::unique_ptr<Token2> Token2::CreateIdentifier(
    std::string_view name)
{
    return std::unique_ptr<Token2>(new Token2(name));
}

/*static*/ std::unique_ptr<Token2> Token2::CreateNumber(std::string_view value)
{
    double result;
    auto [ptr, ec]{
        std::from_chars(value.data(), value.data() + value.size(), result)};
    if (ec != std::errc())
        throw std::invalid_argument("Value cannot be converted to double.");
    return std::unique_ptr<Token2>(new Token2(result));
}

/*static*/ std::unique_ptr<Token2> Token2::CreateCharacter(char value)
{
    return std::unique_ptr<Token2>(new Token2(value));
}

}  // namespace kaleidoscope
