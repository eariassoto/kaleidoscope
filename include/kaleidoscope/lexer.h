#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/token.h"

#include <tl/expected.hpp>

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

    tl::expected<Token, LexerError> PeekToken();

    void ConsumeToken();

   private:
    std::string input_;
    std::string_view input_to_process_;

    std::optional<tl::expected<Token, LexerError>> next_token_ = std::nullopt;
};
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_LEXER_H