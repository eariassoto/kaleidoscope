#include "kaleidoscope/lexer.h"

#include "kaleidoscope/token.h"

#include <gtest/gtest.h>

#include <utility>

using namespace kaleidoscope;

using namespace std::literals::string_view_literals;

namespace
{
void TokensInLexerMatch(const char *actual_input,
                        const std::vector<Token> &expected_tokens)
{
    std::string input_str(actual_input);
    Lexer lexer(std::move(input_str));
    for (size_t i = 0; i < expected_tokens.size(); ++i) {
        const auto &[type, value] = expected_tokens.at(i);
        EXPECT_EQ(type, lexer.PeekToken().Type) << "i= " << i;
        EXPECT_EQ(value, lexer.PeekToken().Value);
        lexer.ConsumeToken();
    }
    EXPECT_EQ(TokenType::kEof, lexer.PeekToken().Type);
}
}  // namespace

class LexerTest : public ::testing::Test
{
};

TEST_F(LexerTest, EmptyInput)
{
    {
        TokensInLexerMatch("", {});
    }
    {
        const char *input =
            "    "
            "    ";
        TokensInLexerMatch(input, {});
    }
}

TEST_F(LexerTest, ParseReservedWordTokens)
{
    {
        const char *input = "def defdef    \r\n  \ndef";
        TokensInLexerMatch(input, {{TokenType::kDef, "def"sv},
                                   {TokenType::kIdentifier, "defdef"sv},
                                   {TokenType::kDef, "def"sv}});
    }
    {
        const char *input = "exte rn extern   extern";
        TokensInLexerMatch(input, {{TokenType::kIdentifier, "exte"sv},
                                   {TokenType::kIdentifier, "rn"sv},
                                   {TokenType::kExtern, "extern"sv},
                                   {TokenType::kExtern, "extern"sv}});
    }
}

TEST_F(LexerTest, ParseIdentifierTokens)
{
    const char *input = "   \r\nid1 Abc78TTTT";
    TokensInLexerMatch(input, {{TokenType::kIdentifier, "id1"sv},
                               {TokenType::kIdentifier, "Abc78TTTT"sv}});
}

TEST_F(LexerTest, ParseNumberTokens)
{
    const char *input = "   \n\n\r\n1    \r\n888734743  42";
    TokensInLexerMatch(input, {{TokenType::kNumber, "1"sv},
                               {TokenType::kNumber, "888734743"sv},
                               {TokenType::kNumber, "42"sv}});
}

TEST_F(LexerTest, ParseCharTokens)
{
    const char *input = "   ( ) - + * , .   ";
    TokensInLexerMatch(input, {{TokenType::kLeftParen, "("sv},
                               {TokenType::kRightParen, ")"sv},
                               {TokenType::kMinusSign, "-"sv},
                               {TokenType::kPlusSign, "+"sv},
                               {TokenType::kAsterisk, "*"sv},
                               {TokenType::kComma, ","sv},
                               {TokenType::kDot, "."sv}});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
