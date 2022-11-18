#include "lexer/lexer.h"

#include "lexer/token.h"

#include <gtest/gtest.h>

#include <array>

using namespace kaleidoscope;

class LexerTest : public ::testing::Test
{
};

TEST_F(LexerTest, EmptyInput)
{
    Lexer lexer(std::string(""));
    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(Token::Type::kEof, next_token->GetType());
}

TEST_F(LexerTest, EmptyInputWithSpaces)
{
    const char *input =
        "    "
        "    ";
    Lexer lexer(std::move(std::string(input)));
    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(Token::Type::kEof, next_token->GetType());
}

TEST_F(LexerTest, Def)
{
    const char *input =
        "def def   def"
        "def";
    Lexer lexer(std::move(std::string(input)));

    std::unique_ptr<Token> next_token = nullptr;
    for (int i = 0; i < 4; ++i) {
        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token) << "i=" << i;
        EXPECT_EQ(Token::Type::kDef, next_token->GetType());
    }
    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(Token::Type::kEof, next_token->GetType());
}

TEST_F(LexerTest, Extern)
{
    const char *input =
        "extern extern   "
        "extern";
    Lexer lexer(std::move(std::string(input)));

    std::unique_ptr<Token> next_token = nullptr;
    for (int i = 0; i < 3; ++i) {
        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token) << "i=" << i;
        EXPECT_EQ(Token::Type::kExtern, next_token->GetType());
    }
    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(Token::Type::kEof, next_token->GetType());
}

TEST_F(LexerTest, Indentifier)
{
    {
        Lexer lexer(std::string("abc12AB"));

        std::unique_ptr<Token> next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token);
        EXPECT_EQ(Token::Type::kIdentifier, next_token->GetType());
        EXPECT_EQ("abc12AB", next_token->GetValue<std::string_view>());

        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token);
        EXPECT_EQ(Token::Type::kEof, next_token->GetType());
    }
    {
        Lexer lexer(std::string("   \r\nid1 Abc78TTTT"));

        std::unique_ptr<Token> next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token);
        EXPECT_EQ(Token::Type::kIdentifier, next_token->GetType());
        EXPECT_EQ("id1", next_token->GetValue<std::string_view>());

        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token);
        EXPECT_EQ(Token::Type::kIdentifier, next_token->GetType());
        EXPECT_EQ("Abc78TTTT", next_token->GetValue<std::string_view>());

        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token);
        EXPECT_EQ(Token::Type::kEof, next_token->GetType());
    }
}
