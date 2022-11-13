#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <string>
#include <string_view>

namespace kaleidoscope
{

enum Token : signed char {
    kTokenEof = -1,

    // commands
    kTokenDef = -2,
    kTokenExtern = -3,

    // primary
    kTokenIdentifier = -4,
    kTokenNumber = -5,
    kTokenAscii = -6,

    kTokenUnknown = -7,
};

[[nodiscard]] const char* TokenAsString(Token t);

class Lexer
{
   public:
    Lexer() = delete;
    ~Lexer();

    Lexer(std::string input);

    Token GetNextToken();

    std::string_view GetIdentifier() const;
    double GetNumber() const;
    char GetAscii() const;

   private:
    void AdvanceIndex();
    std::string_view GetNextAlphaNum();
    std::string_view GetNextDigit();

    std::string m_Input;
    size_t m_CharIndex = 0;

    std::string_view m_Identifier;
    double m_Num;
    char m_Ascii;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_H