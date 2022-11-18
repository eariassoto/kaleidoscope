#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include "lexer/token.h"

#include <memory>
#include <string>
#include <string_view>

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

    std::unique_ptr<Token> GetNextToken();

   private:
    // void AdvanceInputPos();
    // std::string_view GetNextAlphaNum();
    // std::string_view GetNextDigit();

    std::string input_;
    std::string_view input_to_process_;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H