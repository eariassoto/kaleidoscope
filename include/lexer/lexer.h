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

    Lexer(std::string input);

    std::unique_ptr<Token> GetNextToken();

   private:
    void AdvanceInputPos();
    std::string_view GetNextAlphaNum();
    std::string_view GetNextDigit();

    std::string input_;
    size_t curr_input_pos_ = 0;

    std::unique_ptr<Token> next_token_ = nullptr;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H