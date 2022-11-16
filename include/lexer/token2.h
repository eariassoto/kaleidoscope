#ifndef LEXER_TOKEN2_H
#define LEXER_TOKEN2_H

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <variant>

namespace kaleidoscope
{
class Token2
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

    Token2() = delete;
    ~Token2();

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

    // Consider to refactoring these into CreateReservedWord
    [[nodiscard]] static std::unique_ptr<Token2> CreateEof();
    [[nodiscard]] static std::unique_ptr<Token2> CreateDef();
    [[nodiscard]] static std::unique_ptr<Token2> CreateExtern();

    [[nodiscard]] static std::unique_ptr<Token2> CreateIdentifier(
        std::string_view value);

    [[nodiscard]] static std::unique_ptr<Token2> CreateNumber(
        std::string_view value);

    [[nodiscard]] static std::unique_ptr<Token2> CreateCharacter(char value);

   private:
    explicit Token2(Type type);
    explicit Token2(std::string_view identifier_value);
    explicit Token2(double number_value);
    explicit Token2(char character_value);

   private:
    Type type_;
    std::variant<std::string_view, double, char> value_;
};

}  // namespace kaleidoscope

#endif  // LEXER_TOKEN2_H
