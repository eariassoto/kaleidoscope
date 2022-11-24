#include "kaleidoscope/lexer.h"

#include "kaleidoscope/token.h"

#include <gtest/gtest.h>

#include <utility>

using namespace kaleidoscope;

using namespace std::literals::string_view_literals;

namespace
{
void TokensInLexerMatch(
    Lexer *lexer,
    const std::vector<std::pair<TokenType, std::string_view>> &expected_tokens)
{
    for (size_t i = 0; i < expected_tokens.size(); ++i) {
        const auto &[type, value] = expected_tokens.at(i);
        EXPECT_EQ(type, lexer->PeekToken().Type) << "i= " << i;
        EXPECT_EQ(value, lexer->PeekToken().Value);
        lexer->ConsumeToken();
    }
    EXPECT_EQ(TokenType::kEof, lexer->PeekToken().Type);
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
        TokensInLexerMatch(&lexer, {{TokenType::kDef, "def"sv},
                                    {TokenType::kDef, "def"sv},
                                    {TokenType::kDef, "def"sv},
                                    {TokenType::kDef, "def"sv}});
    }
    {
        const char *input =
            "extern extern   "
            "extern";
        Lexer lexer(std::move(std::string(input)));
        TokensInLexerMatch(&lexer, {{TokenType::kExtern, "extern"sv},
                                    {TokenType::kExtern, "extern"sv},
                                    {TokenType::kExtern, "extern"sv}});
    }
}

TEST_F(LexerTest, ParseIdentifierTokens)
{
    Lexer lexer(std::string("   \r\nid1 Abc78TTTT"));
    TokensInLexerMatch(&lexer, {{TokenType::kIdentifier, "id1"sv},
                                {TokenType::kIdentifier, "Abc78TTTT"sv}});
}

TEST_F(LexerTest, ParseNumberTokens)
{
    Lexer lexer(std::string("   \n\n\r\n1    \r\n888734743  42"));
    TokensInLexerMatch(&lexer, {{TokenType::kNumber, "1"sv},
                                {TokenType::kNumber, "888734743"sv},
                                {TokenType::kNumber, "42"sv}});
}

TEST_F(LexerTest, ParseCharTokens)
{
    Lexer lexer(std::string("   ( ) - + * , .   "));
    TokensInLexerMatch(&lexer, {{TokenType::kLeftParen, "("sv},
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
