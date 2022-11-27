#include "kaleidoscope/lexer.h"

#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/token.h"

#include <algorithm>
#include <array>
#include <utility>

using namespace std::literals::string_view_literals;

namespace kaleidoscope
{

namespace
{
const std::string_view kDefKeyword = "def"sv;
const std::string_view kExternKeyword = "extern"sv;

static constexpr std::array<std::pair<unsigned char, TokenType>, 7>
    kAllowedCharacters{{{'(', TokenType::kLeftParen},
                        {')', TokenType::kRightParen},
                        {'+', TokenType::kPlusSign},
                        {'-', TokenType::kMinusSign},
                        {'*', TokenType::kAsterisk},
                        {',', TokenType::kComma},
                        {'.', TokenType::kDot}}};

std::string::const_iterator AdvanceInputPos(
    const std::string::const_iterator& curr_pos,
    const std::string::const_iterator& end_it)
{
    std::string::const_iterator new_pos = curr_pos;
    while (new_pos != end_it &&
           std::isspace(static_cast<unsigned char>(*new_pos))) {
        new_pos++;
    }
    return new_pos;
}
}  // namespace

Lexer::Lexer(std::string input)
    : input_(std::move(input)), input_to_process_(input_.data(), input_.size())
{
}

Lexer::~Lexer() = default;

Token Lexer::PeekToken()
{
    if (error_found_) throw LexerError(input_to_process_.front());

    if (next_token_) return next_token_.value();

    if (input_to_process_.empty()) {
        next_token_.emplace(TokenType::kEof, std::string_view());
        return next_token_.value();
    }
    // Trim the input
    auto trim_it =
        std::find_if(input_to_process_.begin(), input_to_process_.end(),
                     [](unsigned char ch) { return !std::isspace(ch); });

    if (trim_it == input_to_process_.end()) {
        next_token_.emplace(TokenType::kEof, std::string_view());
        return next_token_.value();
    }
    // Advance the string view to ignore whitespaces
    input_to_process_ =
        input_to_process_.substr(trim_it - input_to_process_.begin());

    const unsigned char next_char =
        static_cast<unsigned char>(input_to_process_.front());
    if (std::isalpha(next_char)) {
        // Find the last consecutive alphanumeric character
        auto identifier_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isalnum(ch); });
        const std::string_view next_alpha_num = input_to_process_.substr(
            0, identifier_it - input_to_process_.begin());
        TokenType token_found = TokenType::kIdentifier;
        //  Check for reserved words
        if (next_alpha_num == kDefKeyword) {
            token_found = TokenType::kDef;
        } else if (next_alpha_num == kExternKeyword) {
            token_found = TokenType::kExtern;
        }

        // Consume the identifier
        input_to_process_ = input_to_process_.substr(next_alpha_num.size());

        return next_token_.emplace(token_found, next_alpha_num);
    }

    if (std::isdigit(next_char)) {
        // Find the last consecutive digit character
        auto num_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isdigit(ch); });
        const std::string_view next_digit =
            input_to_process_.substr(0, num_it - input_to_process_.begin());

        // Consume the identifier
        input_to_process_ = input_to_process_.substr(next_digit.size());
        return next_token_.emplace(TokenType::kNumber, next_digit);
    }

    // Check if the next token is a valid character
    if (auto it = std::find_if(
            kAllowedCharacters.begin(), kAllowedCharacters.end(),
            [&next_char](const auto& p) { return p.first == next_char; });
        it != kAllowedCharacters.end()) {
        next_token_.emplace((*it).second,
                            std::string_view(input_to_process_.data(), 1));

        input_to_process_ = input_to_process_.substr(1);

        return next_token_.value();
    }

    // Invalid character, throw error
    error_found_ = true;
    throw LexerError(next_char);
}

void Lexer::ConsumeToken()
{
    // If token has not been peeked, do it and continue
    if (!next_token_) PeekToken();

    // Do not advance if EOF has been reached
    if (next_token_->Type == TokenType::kEof) return;

    next_token_.reset();
}

}  // namespace kaleidoscope
