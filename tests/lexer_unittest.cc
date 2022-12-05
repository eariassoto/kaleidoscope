#include "kaleidoscope/lexer_impl.h"

#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/token.h"

#include <gtest/gtest.h>

#include <utility>

using kaleidoscope::LexerImpl;
using kaleidoscope::Token;
using kaleidoscope::TokenType;
using kaleidoscope::LexerError;

using namespace std::literals::string_view_literals;

namespace
{
void TokensInLexerMatch(const char *actual_input,
                        const std::vector<Token> &expected_tokens)
{
    std::string input_str(actual_input);
    LexerImpl lexer(std::move(input_str));
    for (const Token &expected_token : expected_tokens) {
        const tl::expected<Token, LexerError> actual_token = lexer.PeekToken();
        ASSERT_TRUE(actual_token);
        EXPECT_EQ(expected_token.Type, actual_token->Type);
        EXPECT_EQ(expected_token.Value, actual_token->Value);
        lexer.ConsumeToken();
    }
    const tl::expected<Token, LexerError> eof_token = lexer.PeekToken();
    ASSERT_TRUE(eof_token);
    EXPECT_EQ(TokenType::kEof, eof_token->Type);
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
