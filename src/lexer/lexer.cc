#include "lexer/lexer.h"

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

std::unique_ptr<Token> Lexer::GetNextToken()
{
    if (input_to_process_.empty())
        return std::make_unique<Token>(TokenType::kEof);

    // Trim the input
    auto trim_it =
        std::find_if(input_to_process_.begin(), input_to_process_.end(),
                     [](unsigned char ch) { return !std::isspace(ch); });

    // Advance the string view to ignore whitespaces
    if (trim_it == input_to_process_.end())
        return std::make_unique<Token>(TokenType::kEof);

    input_to_process_ =
        input_to_process_.substr(trim_it - input_to_process_.begin());

    //  Check for reserved words
    if (input_to_process_.starts_with(kDefKeyword)) {
        input_to_process_ = input_to_process_.substr(kDefKeyword.size());
        return std::make_unique<Token>(TokenType::kDef);
    }
    if (input_to_process_.starts_with(kExternKeyword)) {
        input_to_process_ = input_to_process_.substr(kExternKeyword.size());
        return std::make_unique<Token>(TokenType::kExtern);
    }

    const unsigned char next_char =
        static_cast<unsigned char>(input_to_process_.front());
    if (std::isalpha(next_char)) {
        // Find the last consecutive alphanumeric character
        auto identifier_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isalnum(ch); });
        auto res = std::make_unique<Token>(
            TokenType::kIdentifier,
            std::string_view(input_to_process_.data(),
                             identifier_it - input_to_process_.begin()));

        // Consume the identifier
        input_to_process_ =
            input_to_process_.substr(identifier_it - input_to_process_.begin());
        return res;
    }
    if (std::isdigit(next_char)) {
        // Find the last consecutive digit character
        auto num_it =
            std::find_if(input_to_process_.begin(), input_to_process_.end(),
                         [](unsigned char ch) { return !std::isdigit(ch); });
        auto res = std::make_unique<Token>(
            TokenType::kNumber,
            std::string_view(input_to_process_.data(),
                             num_it - input_to_process_.begin()));

        // Consume the identifier
        input_to_process_ =
            input_to_process_.substr(num_it - input_to_process_.begin());
        return res;
    }

    // Check if the next token is a valid character
    if (std::find(kAllowedCharacters.begin(), kAllowedCharacters.end(),
                  next_char) != kAllowedCharacters.end()) {
        auto res = std::make_unique<Token>(
            TokenType::kCharacter,
            std::string_view(input_to_process_.data(), 1));
        input_to_process_ = input_to_process_.substr(1);
        return res;
    }

    // TODO: Handle invalid case
    return nullptr;
}

Lexer::~Lexer() = default;

}  // namespace kaleidoscope
