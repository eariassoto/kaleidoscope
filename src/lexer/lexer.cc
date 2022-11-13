#include "lexer/lexer.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <utility>

using namespace std::literals::string_view_literals;

namespace kaleidoscope
{

namespace
{
static constexpr std::array<std::pair<std::string_view, Token>, 2>
    RESERVED_TOKENS{
        {{"def"sv, Token::kTokenDef}, {"extern"sv, Token::kTokenExtern}}};

// Could be made constexpr for C++20
[[nodiscard]] Token FindReserved(std::string_view input)
{
    const auto it =
        std::find_if(begin(RESERVED_TOKENS), end(RESERVED_TOKENS),
                     [&input](const auto& v) { return input == v.first; });
    if (it != end(RESERVED_TOKENS)) {
        return it->second;
    }
    return Token::kTokenIdentifier;
}
}  // namespace

[[nodiscard]] const char* TokenAsString(Token t)
{
    if (t == Token::kTokenEof)
        return "kTokenEof";
    else if (t == Token::kTokenDef)
        return "kTokenDef";
    else if (t == Token::kTokenExtern)
        return "kTokenExtern";
    else if (t == Token::kTokenIdentifier)
        return "kTokenIdentifier";
    else if (t == Token::kTokenNumber)
        return "kTokenNumber";
    else if (t == Token::kTokenAscii)
        return "kTokenAscii";
    else if (t == Token::kTokenUnknown)
        return "kTokenUnknown";
    throw std::range_error("Not found");
}

std::string_view Lexer::GetNextAlphaNum()
{
    const char* start_index = &m_Input[m_CharIndex];
    while (true) {
        ++m_CharIndex;
        if (m_CharIndex >= m_Input.size() ||
            !std::isalnum(static_cast<unsigned char>(m_Input[m_CharIndex]))) {
            break;
        }
    }
    const size_t count = &m_Input[m_CharIndex] - start_index;
    return std::string_view(start_index, count);
}

std::string_view Lexer::GetNextDigit()
{
    const char* start_index = &m_Input[m_CharIndex];
    unsigned char curr_char = static_cast<unsigned char>(m_Input[m_CharIndex]);
    bool point_found = curr_char == '.';
    while (true) {
        ++m_CharIndex;
        if (m_CharIndex >= m_Input.size()) break;  // nothing else to read

        curr_char = static_cast<unsigned char>(m_Input[m_CharIndex]);

        if (curr_char == '.') {
            // Only read one decimal point
            if (point_found) break;
            point_found = true;
        } else if (!std::isalnum(curr_char))
            break;
    }

    const size_t count = &m_Input[m_CharIndex] - start_index;
    return std::string_view(start_index, count);
}

void Lexer::AdvanceIndex()
{
    while (m_CharIndex < m_Input.size() &&
           std::isspace(static_cast<unsigned char>(m_Input[m_CharIndex]))) {
        ++m_CharIndex;
    }
}

Lexer::Lexer(std::string input) : m_Input(std::move(input)) {}

std::string_view Lexer::GetIdentifier() const { return m_Identifier; }

double Lexer::GetNumber() const { return m_Num; }

char Lexer::GetAscii() const { return m_Ascii; }

Token Lexer::GetNextToken()
{
    AdvanceIndex();
    // std::cout << "m_CharIndex: " << m_CharIndex << '\n';
    if (m_CharIndex >= m_Input.size()) return Token::kTokenEof;

    const unsigned char curr_char =
        static_cast<unsigned char>(m_Input[m_CharIndex]);
    if (std::isalpha(curr_char)) {
        std::string_view next_alphanum = GetNextAlphaNum();
        // std::cout << "got: " << next_alphanum << '\n';
        Token next_token = FindReserved(next_alphanum);
        if (next_token == Token::kTokenIdentifier) m_Identifier = next_alphanum;
        return next_token;
    } else if (std::isdigit(curr_char)) {
        std::string_view next_num = GetNextDigit();
        std::from_chars(next_num.data(), next_num.data() + next_num.size(),
                        m_Num);
        return Token::kTokenNumber;
    }
    // default, assume ascii
     m_Ascii = curr_char;
     ++m_CharIndex;
     return Token::kTokenAscii;
    //return Token::kTokenUnknown;
}

Lexer::~Lexer() = default;

}  // namespace kaleidoscope
