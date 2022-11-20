#include "lexer/lexer.h"

#include "lexer/token.h"

#include <gtest/gtest.h>

#include <utility>

using namespace kaleidoscope;

using namespace std::literals::string_view_literals;

namespace
{
void TokensInLexerMatch(
    Lexer *lexer,
    const std::vector<std::pair<Token, std::string_view>> &expected_tokens)
{
    for (size_t i = 0; i < expected_tokens.size(); ++i) {
        const auto &[type, value] = expected_tokens.at(i);
        EXPECT_EQ(type, lexer->PeekToken()) << "i= " << i;
        EXPECT_EQ(value, lexer->PeekTokenValue());
        lexer->ConsumeToken();
    }
    Token eof_token = lexer->PeekToken();
    EXPECT_EQ(Token::kEof, lexer->PeekToken());
}
}  // namespace

class LexerTest : public ::testing::Test
{
};

TEST_F(LexerTest, EmptyInput)
{
    {
        Lexer lexer(std::string(""));
        TokensInLexerMatch(&lexer, {});
    }
    {
        const char *input =
            "    "
            "    ";
        Lexer lexer(std::move(std::string(input)));
        TokensInLexerMatch(&lexer, {});
    }
}

TEST_F(LexerTest, ParseReservedWordTokens)
{
    {
        const char *input =
            "def def   def"
            "def";
        Lexer lexer(std::move(std::string(input)));
        TokensInLexerMatch(&lexer, {{Token::kDef, "def"sv},
                                    {Token::kDef, "def"sv},
                                    {Token::kDef, "def"sv},
                                    {Token::kDef, "def"sv}});
    }
    {
        const char *input =
            "extern extern   "
            "extern";
        Lexer lexer(std::move(std::string(input)));
        TokensInLexerMatch(&lexer, {{Token::kExtern, "extern"sv},
                                    {Token::kExtern, "extern"sv},
                                    {Token::kExtern, "extern"sv}});
    }
}

TEST_F(LexerTest, ParseIdentifierTokens)
{
    Lexer lexer(std::string("   \r\nid1 Abc78TTTT"));
    TokensInLexerMatch(&lexer, {{Token::kIdentifier, "id1"sv},
                                {Token::kIdentifier, "Abc78TTTT"sv}});
}

TEST_F(LexerTest, ParseNumberTokens)
{
    Lexer lexer(std::string("   \n\n\r\n1    \r\n888734743  42"));
    TokensInLexerMatch(&lexer, {{Token::kNumber, "1"sv},
                                {Token::kNumber, "888734743"sv},
                                {Token::kNumber, "42"sv}});
}

TEST_F(LexerTest, ParseCharTokens)
{
    Lexer lexer(std::string("   ( ) - + * , .   "));
    TokensInLexerMatch(&lexer, {{Token::kLeftParen, "("sv},
                                {Token::kRightParen, ")"sv},
                                {Token::kMinusSign, "-"sv},
                                {Token::kPlusSign, "+"sv},
                                {Token::kAsterisk, "*"sv},
                                {Token::kComma, ","sv},
                                {Token::kDot, "."sv}});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
