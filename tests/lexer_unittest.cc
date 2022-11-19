#include "lexer/lexer.h"

#include "lexer/token.h"

#include <gtest/gtest.h>

#include <optional>
#include <utility>

using namespace kaleidoscope;

using namespace std::literals::string_view_literals;

namespace
{
void TokensInLexerMatch(
    Lexer *lexer,
    const std::vector<std::pair<TokenType, std::optional<std::string_view>>>
        &expected_tokens)
{
    for (size_t i = 0; i < expected_tokens.size(); ++i) {
        const auto &[type, value] = expected_tokens.at(i);
        Token next_token = lexer->PeekToken();
        EXPECT_EQ(type, next_token.GetType()) << "i= " << i;
        if (value.has_value()) EXPECT_EQ(*value, next_token.GetValue());
        lexer->ConsumeToken();
    }
    Token eof_token = lexer->PeekToken();
    EXPECT_EQ(TokenType::kEof, eof_token.GetType());
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
        TokensInLexerMatch(&lexer, {{TokenType::kDef, std::nullopt},
                                    {TokenType::kDef, std::nullopt},
                                    {TokenType::kDef, std::nullopt},
                                    {TokenType::kDef, std::nullopt}});
    }
    {
        const char *input =
            "extern extern   "
            "extern";
        Lexer lexer(std::move(std::string(input)));
        TokensInLexerMatch(&lexer, {{TokenType::kExtern, std::nullopt},
                                    {TokenType::kExtern, std::nullopt},
                                    {TokenType::kExtern, std::nullopt}});
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
    TokensInLexerMatch(&lexer, {{TokenType::kCharacter, "("sv},
                                {TokenType::kCharacter, ")"sv},
                                {TokenType::kCharacter, "-"sv},
                                {TokenType::kCharacter, "+"sv},
                                {TokenType::kCharacter, "*"sv},
                                {TokenType::kCharacter, ","sv},
                                {TokenType::kCharacter, "."sv}});
}
