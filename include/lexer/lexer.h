#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include "lexer/token.h"

#include <memory>
#include <optional>
#include <string_view>
#include <string>

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

    std::unique_ptr<Token> next_token_ = nullptr;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H