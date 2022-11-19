#ifndef LEXER_LEXER_ERROR_H
#define LEXER_LEXER_ERROR_H

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

    LexerError(const LexerError& t) = delete;
    LexerError& operator=(const LexerError&) = delete;

    const char* what() const noexcept override;

   private:
    std::string message_;
};
}  // namespace kaleidoscope

#endif  // LEXER_LEXER_ERROR_H