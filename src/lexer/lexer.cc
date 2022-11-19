#include "lexer/lexer.h"

#include "lexer/lexer_error.h"

#include <algorithm>
#include <array>

using namespace std::literals::string_view_literals;

namespace kaleidoscope
{

namespace
{
const std::string_view kDefKeyword = "def"sv;
const std::string_view kExternKeyword = "extern"sv;

static constexpr std::array<char, 7> kAllowedCharacters{'(', ')', '+', '-',
                                                        '*', ',', '.'};

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
        next_token_ = std::make_unique<Token>(TokenType::kEof);
        return *next_token_;
    }
    // Trim the input
    auto trim_it =
        std::find_if(input_to_process_.begin(), input_to_process_.end(),
                     [](unsigned char ch) { return !std::isspace(ch); });

    // Advance the string view to ignore whitespaces
    if (trim_it == input_to_process_.end()) {
        next_token_ = std::make_unique<Token>(TokenType::kEof);
        return *next_token_;
    }
    input_to_process_ =
        input_to_process_.substr(trim_it - input_to_process_.begin());

    //  Check for reserved words
    if (input_to_process_.starts_with(kDefKeyword)) {
        input_to_process_ = input_to_process_.substr(kDefKeyword.size());
        next_token_ = std::make_unique<Token>(TokenType::kDef);
        return *next_token_;
    }
    if (input_to_process_.starts_with(kExternKeyword)) {
        input_to_process_ = input_to_process_.substr(kExternKeyword.size());
        next_token_ = std::make_unique<Token>(TokenType::kExtern);
        return *next_token_;
    }

    const unsigned char next_char =
        static_cast<unsigned char>(input_to_process_.front());
    if (std::isalpha(next_char)) {
        // Find the last consecutive alphanumeric character
        auto identifier_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isalnum(ch); });
        next_token_ = std::make_unique<Token>(
            TokenType::kIdentifier,
            std::string_view(input_to_process_.data(),
                             identifier_it - input_to_process_.begin()));

        // Consume the identifier
        input_to_process_ =
            input_to_process_.substr(identifier_it - input_to_process_.begin());
        return *next_token_;
    }

    if (std::isdigit(next_char)) {
        // Find the last consecutive digit character
        auto num_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isdigit(ch); });
        next_token_ = std::make_unique<Token>(
            TokenType::kNumber,
            std::string_view(input_to_process_.data(),
                             num_it - input_to_process_.begin()));

        // Consume the identifier
        input_to_process_ =
            input_to_process_.substr(num_it - input_to_process_.begin());
        return *next_token_;
    }

    // Check if the next token is a valid character
    if (std::find(kAllowedCharacters.begin(), kAllowedCharacters.end(),
                  next_char) != kAllowedCharacters.end()) {
        next_token_ = std::make_unique<Token>(
            TokenType::kCharacter,
            std::string_view(input_to_process_.data(), 1));
        input_to_process_ = input_to_process_.substr(1);
        return *next_token_;
    }

    // Invalid character, throw error
    error_found_ = true;
    throw LexerError(next_char);
}

void Lexer::ConsumeToken()
{
    if (error_found_) return;

    // If token has not been peeked, do it and continue
    if (!next_token_) PeekToken();

    // Do not advance if EOF has been reached
    if (next_token_->GetType() == TokenType::kEof) return;

    next_token_.reset();
}

}  // namespace kaleidoscope
