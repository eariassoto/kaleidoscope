#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <optional>
#include <string_view>
#include <string>

namespace kaleidoscope
{

enum class Token;

class Lexer
{
   public:
    Lexer() = delete;
    ~Lexer();

    explicit Lexer(std::string input);

    Lexer(const Lexer& t) = delete;
    Lexer& operator=(const Lexer&) = delete;

    Token PeekToken();
    std::string_view PeekTokenValue();

    void ConsumeToken();

   private:
    std::string input_;
    std::string_view input_to_process_;

    bool error_found_ = false;
    std::optional<Token> next_token_ = std::nullopt;
    std::string_view next_token_value_ = std::string_view(nullptr, 0);
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H