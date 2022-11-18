#include "lexer/lexer.h"

#include "lexer/token.h"

#include <gtest/gtest.h>

using namespace kaleidoscope;

class LexerTest : public ::testing::Test
{
};

TEST_F(LexerTest, EmptyInput)
{
    Lexer lexer(std::string(""));
    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}

TEST_F(LexerTest, EmptyInputWithSpaces)
{
    const char *input =
        "    "
        "    ";
    Lexer lexer(std::move(std::string(input)));
    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}

TEST_F(LexerTest, ParseDefTokens)
{
    const char *input =
        "def def   def"
        "def";
    Lexer lexer(std::move(std::string(input)));

    std::unique_ptr<Token> next_token = nullptr;
    for (int i = 0; i < 4; ++i) {
        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token) << "i=" << i;
        EXPECT_EQ(TokenType::kDef, next_token->GetType());
    }
    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}

TEST_F(LexerTest, ParseExternTokens)
{
    const char *input =
        "extern extern   "
        "extern";
    Lexer lexer(std::move(std::string(input)));

    std::unique_ptr<Token> next_token = nullptr;
    for (int i = 0; i < 3; ++i) {
        next_token = lexer.GetNextToken();
        ASSERT_TRUE(next_token) << "i=" << i;
        EXPECT_EQ(TokenType::kExtern, next_token->GetType());
    }
    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}

TEST_F(LexerTest, ParseIdentifierTokens)
{
    Lexer lexer(std::string("   \r\nid1 Abc78TTTT"));

    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kIdentifier, next_token->GetType());
    EXPECT_EQ("id1", next_token->GetValue());

    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kIdentifier, next_token->GetType());
    EXPECT_EQ("Abc78TTTT", next_token->GetValue());

    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}

TEST_F(LexerTest, ParseNumberTokens)
{
    Lexer lexer(std::string("   \n\n\r\n1    \r\n888734743  42"));

    std::unique_ptr<Token> next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kNumber, next_token->GetType());
    EXPECT_EQ("1", next_token->GetValue());

    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kNumber, next_token->GetType());
    EXPECT_EQ("888734743", next_token->GetValue());

    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kNumber, next_token->GetType());
    EXPECT_EQ("42", next_token->GetValue());

    next_token = lexer.GetNextToken();
    ASSERT_TRUE(next_token);
    EXPECT_EQ(TokenType::kEof, next_token->GetType());
}
