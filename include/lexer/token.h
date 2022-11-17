#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <variant>

namespace kaleidoscope
{
class Token
{
   public:
    enum class Type {
        kEof,

        // reserved words
        kDef,
        kExtern,

        kIdentifier,
        kNumber,
        kCharacter
    };

    Token() = delete;
    ~Token();

    inline Type GetType() const { return type_; }

    template <typename V>
    [[nodiscard]] const V& GetValue() const
    {
        if constexpr (std::is_same_v<V, std::string_view>) {
            if (type_ != Type::kIdentifier)
                throw std::invalid_argument("Token is not an identifier.");
            return *std::get_if<V>(&value_);
        } else if constexpr (std::is_same_v<V, double>) {
            if (type_ != Type::kNumber)
                throw std::invalid_argument("Token is not a number.");
            return *std::get_if<V>(&value_);
        } else if constexpr (std::is_same_v<V, char>) {
            if (type_ != Type::kCharacter)
                throw std::invalid_argument("Token is not a character.");
            return *std::get_if<V>(&value_);
        }
        throw std::invalid_argument("Invalid type for token.");
    }

    bool IsCharToken(char value) const;

    [[nodiscard]] static std::unique_ptr<Token> CreateEof();
    [[nodiscard]] static std::unique_ptr<Token> CreateDef();
    [[nodiscard]] static std::unique_ptr<Token> CreateExtern();

    [[nodiscard]] static std::unique_ptr<Token> CreateIdentifier(
        std::string_view value);

    [[nodiscard]] static std::unique_ptr<Token> CreateNumber(double value);

    [[nodiscard]] static std::unique_ptr<Token> CreateCharacter(char value);

   private:
    explicit Token(Type type);
    explicit Token(std::string_view identifier_value);
    explicit Token(double number_value);
    explicit Token(char character_value);

   private:
    Type type_;
    std::variant<std::string_view, double, char> value_;
};

}  // namespace kaleidoscope

#endif  // LEXER_TOKEN_H
