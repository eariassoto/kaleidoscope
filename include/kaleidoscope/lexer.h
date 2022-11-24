#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <optional>
#include <string_view>
#include <string>

#include "kaleidoscope/token.h"

namespace kaleidoscope
{

class Lexer
{
   public:
    Lexer() = delete;
    ~Lexer();
    explicit Lexer(std::string input);
    Lexer(const Lexer& t) = delete;
    Lexer& operator=(const Lexer&) = delete;

    Token PeekToken();

    void ConsumeToken();

   private:
    std::string input_;
    std::string_view input_to_process_;

    bool error_found_ = false;
    std::optional<Token> next_token_ = std::nullopt;
};
}  // namespace kaleidoscope

#endif // KALEIDOSCOPE_LEXER_H