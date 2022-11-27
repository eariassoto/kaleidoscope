#ifndef KALEIDOSCOPE_LEXER_ERROR_H
#define KALEIDOSCOPE_LEXER_ERROR_H

#include <exception>
#include <string>

namespace kaleidoscope
{
class LexerError : public std::exception
{
   public:
    LexerError() = delete;
    ~LexerError();

    explicit LexerError(char input);

    const char* what() const noexcept override;

   private:
    std::string message_;
};
}  // namespace kaleidoscope

#endif // #define KALEIDOSCOPE_LEXER_ERROR_H
