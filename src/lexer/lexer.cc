#include "lexer/lexer.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <utility>

using namespace std::literals::string_view_literals;

namespace kaleidoscope
{

namespace
{
const std::string_view kDefKeyword = "def"sv;
const std::string_view kExternKeyword = "extern"sv;

static constexpr std::array<char, 6> kAllowedCharacters{'(', ')', '+',
                                                        '-', '*', ','};
}  // namespace

// [[nodiscard]] const char* TokenAsString(Token t)
// {
//     if (t == Token::kTokenEof)
//         return "kTokenEof";
//     else if (t == Token::kTokenDef)
//         return "kTokenDef";
//     else if (t == Token::kTokenExtern)
//         return "kTokenExtern";
//     else if (t == Token::kTokenIdentifier)
//         return "kTokenIdentifier";
//     else if (t == Token::kTokenNumber)
//         return "kTokenNumber";
//     else if (t == Token::kTokenAscii)
//         return "kTokenAscii";
//     else if (t == Token::kTokenUnknown)
//         return "kTokenUnknown";
//     throw std::range_error("Not found");
// }

std::string_view Lexer::GetNextAlphaNum()
{
    const char* start_index = &input_[curr_input_pos_];
    while (true) {
        ++curr_input_pos_;
        if (curr_input_pos_ >= input_.size() ||
            !std::isalnum(
                static_cast<unsigned char>(input_[curr_input_pos_]))) {
            break;
        }
    }
    const size_t count = &input_[curr_input_pos_] - start_index;
    return std::string_view(start_index, count);
}

std::string_view Lexer::GetNextDigit()
{
    const char* start_index = &input_[curr_input_pos_];
    unsigned char curr_char =
        static_cast<unsigned char>(input_[curr_input_pos_]);
    bool point_found = curr_char == '.';
    while (true) {
        ++curr_input_pos_;
        if (curr_input_pos_ >= input_.size()) break;  // nothing else to read

        curr_char = static_cast<unsigned char>(input_[curr_input_pos_]);

        if (curr_char == '.') {
            // Only read one decimal point
            if (point_found) break;
            point_found = true;
        } else if (!std::isalnum(curr_char))
            break;
    }

    const size_t count = &input_[curr_input_pos_] - start_index;
    return std::string_view(start_index, count);
}

void Lexer::AdvanceInputPos()
{
    while (curr_input_pos_ < input_.size() &&
           std::isspace(static_cast<unsigned char>(input_[curr_input_pos_]))) {
        ++curr_input_pos_;
    }
}

Lexer::Lexer(std::string input) : input_(std::move(input)) {}

std::unique_ptr<Token> Lexer::GetNextToken()
{
    AdvanceInputPos();
    // std::cout << "curr_input_pos_: " << curr_input_pos_ << '\n';
    if (curr_input_pos_ >= input_.size()) return Token::CreateEof();

    const unsigned char curr_char =
        static_cast<unsigned char>(input_[curr_input_pos_]);
    if (std::isalpha(curr_char)) {
        std::string_view next_alphanum = GetNextAlphaNum();
        // std::cout << "got: " << next_alphanum << '\n';
        if (next_alphanum == kDefKeyword)
            return Token::CreateDef();
        else if (next_alphanum == kExternKeyword)
            return Token::CreateExtern();
        else
            return Token::CreateIdentifier(next_alphanum);
    } else if (std::isdigit(curr_char)) {
        std::string_view next_num = GetNextDigit();
        double result;
        auto [ptr, ec]{std::from_chars(
            next_num.data(), next_num.data() + next_num.size(), result)};
        assert(ec == std::errc());
        return Token::CreateNumber(result);
    }
    if (std::find_if(begin(kAllowedCharacters), end(kAllowedCharacters),
                     [&curr_char](const auto& ch) {
                         return curr_char == ch;
                     }) != end(kAllowedCharacters)) {
        return Token::CreateCharacter(curr_char);
    }

    // TODO: Handle invalid case
    return nullptr;
}

Lexer::~Lexer() = default;

}  // namespace kaleidoscope
