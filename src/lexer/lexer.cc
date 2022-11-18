#include "lexer/lexer.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>

using namespace std::literals::string_view_literals;

namespace kaleidoscope
{

namespace
{
const std::string_view kDefKeyword = "def"sv;
const std::string_view kExternKeyword = "extern"sv;

static constexpr std::array<char, 6> kAllowedCharacters{'(', ')', '+',
                                                        '-', '*', ','};

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

// std::string_view Lexer::GetNextAlphaNum()
// {
//     const std::string::const_iterator start_index = input_pos_;
//     while (true) {
//         ++input_pos_;
//         if (input_pos_ >= input_.cend() ||
//             !std::isalnum(static_cast<unsigned char>(*input_pos_))) {
//             break;
//         }
//     }
//     const size_t count = input_pos_ - start_index;
//     return std::string_view(&(*start_index), count);
// }

// std::string_view Lexer::GetNextDigit()
// {
//     const std::string::const_iterator start_index = input_pos_;
//     unsigned char curr_char = static_cast<unsigned char>(*input_pos_);
//     bool point_found = curr_char == '.';
//     while (true) {
//         ++input_pos_;
//         if (input_pos_ >= input_.cend()) break;  // nothing else to read

//         curr_char = static_cast<unsigned char>(*input_pos_);
//         if (curr_char == '.') {
//             // Only read one decimal point
//             if (point_found) break;
//             point_found = true;
//         } else if (!std::isalnum(curr_char))
//             break;
//     }

//     const size_t count = input_pos_ - start_index;
//     return std::string_view(&(*start_index), count);
// }

Lexer::Lexer(std::string input)
    : input_(std::move(input)), input_to_process_(input_.data(), input_.size())
{
}

std::unique_ptr<Token> Lexer::GetNextToken()
{
    // std::cout << "enter GetNextToken\n";
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

    // std::cout << "GetNextToken: " << input_to_process_ << '\n';
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
        // Find the last valid alphanumeric character
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
    }

    // // std::cout << "curr_input_pos_: " << curr_input_pos_ << '\n';
    // if (input_pos_ >= input_.cend()) return Token::CreateEof();

    // const unsigned char curr_char = static_cast<unsigned char>(*input_pos_);
    // if (std::isalpha(curr_char)) {
    //     std::string_view next_alphanum = GetNextAlphaNum();
    //     // std::cout << "got: " << next_alphanum << '\n';
    //     if (next_alphanum == kDefKeyword)
    //         return Token::CreateDef();
    //     else if (next_alphanum == kExternKeyword)
    //         return Token::CreateExtern();
    //     else
    //         return Token::CreateIdentifier(next_alphanum);
    // } else if (std::isdigit(curr_char)) {
    //     std::string_view next_num = GetNextDigit();
    //     double result;
    //     auto [ptr, ec]{std::from_chars(
    //         next_num.data(), next_num.data() + next_num.size(), result)};
    //     assert(ec == std::errc());
    //     return Token::CreateNumber(result);
    // }
    // if (std::find_if(begin(kAllowedCharacters), end(kAllowedCharacters),
    //                  [&curr_char](const auto& ch) {
    //                      return curr_char == ch;
    //                  }) != end(kAllowedCharacters)) {
    //     return Token::CreateCharacter(curr_char);
    // }

    // TODO: Handle invalid case
    return nullptr;
}

Lexer::~Lexer() = default;

}  // namespace kaleidoscope
