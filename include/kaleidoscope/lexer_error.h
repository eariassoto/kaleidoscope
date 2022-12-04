#ifndef KALEIDOSCOPE_LEXER_ERROR_H
#define KALEIDOSCOPE_LEXER_ERROR_H

#include <string>

namespace kaleidoscope
{
class LexerError
{
   public:
    LexerError() = delete;
    ~LexerError();

    explicit LexerError(char input);

    const char* what() const noexcept;

   private:
    std::string message_;
};
}  // namespace kaleidoscope

#endif  // #define KALEIDOSCOPE_LEXER_ERROR_H
