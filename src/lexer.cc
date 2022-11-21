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

static constexpr std::array<std::pair<unsigned char, Token>, 7>
    kAllowedCharacters{{{'(', Token::kLeftParen},
                        {')', Token::kRightParen},
                        {'+', Token::kPlusSign},
                        {'-', Token::kMinusSign},
                        {'*', Token::kAsterisk},
                        {',', Token::kComma},
                        {'.', Token::kDot}}};

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

    if (next_token_) return *next_token_;

    if (input_to_process_.empty()) {
        next_token_ = Token::kEof;
        return *next_token_;
    }
    // Trim the input
    auto trim_it =
        std::find_if(input_to_process_.begin(), input_to_process_.end(),
                     [](unsigned char ch) { return !std::isspace(ch); });

    if (trim_it == input_to_process_.end()) {
        next_token_ = Token::kEof;
        next_token_value_ = std::string_view(nullptr, 0);
        return *next_token_;
    }
    // Advance the string view to ignore whitespaces
    input_to_process_ =
        input_to_process_.substr(trim_it - input_to_process_.begin());

    //  Check for reserved words
    if (input_to_process_.starts_with(kDefKeyword)) {
        next_token_ = Token::kDef;
        next_token_value_ =
            std::string_view(input_to_process_.data(), kDefKeyword.size());
        input_to_process_ = input_to_process_.substr(kDefKeyword.size());
        return *next_token_;
    }
    if (input_to_process_.starts_with(kExternKeyword)) {
        next_token_ = Token::kExtern;
        next_token_value_ =
            std::string_view(input_to_process_.data(), kExternKeyword.size());
        input_to_process_ = input_to_process_.substr(kExternKeyword.size());
        return *next_token_;
    }

    const unsigned char next_char =
        static_cast<unsigned char>(input_to_process_.front());
    if (std::isalpha(next_char)) {
        // Find the last consecutive alphanumeric character
        auto identifier_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isalnum(ch); });

        const size_t value_size = identifier_it - input_to_process_.begin();
        next_token_ = Token::kIdentifier;
        next_token_value_ =
            std::string_view(input_to_process_.data(), value_size);

        // Consume the identifier
        input_to_process_ = input_to_process_.substr(value_size);

        return *next_token_;
    }

    if (std::isdigit(next_char)) {
        // Find the last consecutive digit character
        auto num_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isdigit(ch); });

        const size_t value_size = num_it - input_to_process_.begin();
        next_token_ = Token::kNumber;
        next_token_value_ =
            std::string_view(input_to_process_.data(), value_size);

        // Consume the identifier
        input_to_process_ = input_to_process_.substr(value_size);

        return *next_token_;
    }

    // Check if the next token is a valid character
    if (auto it = std::find_if(
            kAllowedCharacters.begin(), kAllowedCharacters.end(),
            [&next_char](const auto& p) { return p.first == next_char; });
        it != kAllowedCharacters.end()) {
        next_token_ = (*it).second;
        next_token_value_ = std::string_view(input_to_process_.data(), 1);

        input_to_process_ = input_to_process_.substr(1);

        return *next_token_;
    }

    // Invalid character, throw error
    error_found_ = true;
    throw LexerError(next_char);
}

std::string_view Lexer::PeekTokenValue()
{
    // If token has not been peeked, do it and continue
    if (!next_token_) PeekToken();

    return next_token_value_;
}

void Lexer::ConsumeToken()
{
    // If token has not been peeked, do it and continue
    if (!next_token_) PeekToken();

    // Do not advance if EOF has been reached
    if (*next_token_ == Token::kEof) return;

    next_token_.reset();
}

}  // namespace kaleidoscope