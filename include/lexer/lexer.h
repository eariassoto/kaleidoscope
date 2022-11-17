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
    void AdvanceIndex();
    std::string_view GetNextAlphaNum();
    std::string_view GetNextDigit();

    std::string m_Input;
    size_t m_CharIndex = 0;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H