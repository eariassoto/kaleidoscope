#ifndef KALEIDOSCOPE_LEXER_IMPL_H
#define KALEIDOSCOPE_LEXER_IMPL_H

#include "kaleidoscope/lexer.h"
#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/token.h"

#include <tl/expected.hpp>

#include <optional>
#include <string_view>
#include <string>

namespace kaleidoscope
{

class LexerImpl : public Lexer
{
   public:
    LexerImpl() = delete;
    ~LexerImpl() override;
    explicit LexerImpl(std::string input);
    LexerImpl(const LexerImpl& t) = delete;
    LexerImpl& operator=(const LexerImpl&) = delete;

    tl::expected<Token, LexerError> PeekToken() override;

    void ConsumeToken() override;

   private:
    std::string input_;
    std::string_view input_to_process_;

    std::optional<tl::expected<Token, LexerError>> next_token_ = std::nullopt;
};
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_LEXER_IMPL_H