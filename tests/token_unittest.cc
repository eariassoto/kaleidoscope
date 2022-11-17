#include "lexer/token.h"

#include <gtest/gtest.h>

#include <stdexcept>

using kaleidoscope::Token;
using namespace std::literals::string_view_literals;

namespace
{
template <class T>
void ignore(const T&)
{
}
}  // namespace

class TokenTest : public ::testing::Test
{
};

TEST_F(TokenTest, CreateEof)
{
    std::unique_ptr<Token> token = Token::CreateEof();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kEof, token->GetType());
}

TEST_F(TokenTest, CreateDef)
{
    std::unique_ptr<Token> token = Token::CreateDef();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kDef, token->GetType());
}

TEST_F(TokenTest, CreateExtern)
{
    std::unique_ptr<Token> token = Token::CreateExtern();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kExtern, token->GetType());
}

TEST_F(TokenTest, CreateIdentifier)
{
    const std::string_view identifier = "foo"sv;
    std::unique_ptr<Token> token = Token::CreateIdentifier(identifier);
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kIdentifier, token->GetType());
    EXPECT_EQ(identifier, token->GetValue<std::string_view>());
}

TEST_F(TokenTest, CreateNumber)
{
    const double number = 42.0;
    std::unique_ptr<Token> token = Token::CreateNumber(number);
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kNumber, token->GetType());
    EXPECT_EQ(number, token->GetValue<double>());
}

TEST_F(TokenTest, CreateCharacter)
{
    const char c = '(';
    std::unique_ptr<Token> token = Token::CreateCharacter(c);
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token::Type::kCharacter, token->GetType());
    EXPECT_EQ(c, token->GetValue<char>());
}

TEST_F(TokenTest, GetValue_string_view)
{
    {
        std::unique_ptr<Token> token = Token::CreateEof();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        std::unique_ptr<Token> token = Token::CreateDef();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        std::unique_ptr<Token> token = Token::CreateExtern();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const double number = 42.0;
        std::unique_ptr<Token> token = Token::CreateNumber(number);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const char c = '(';
        std::unique_ptr<Token> token = Token::CreateCharacter(c);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const std::string_view identifier = "foo"sv;
        std::unique_ptr<Token> token = Token::CreateIdentifier(identifier);
        ASSERT_TRUE(token.get());
        EXPECT_NO_THROW(ignore(token->GetValue<std::string_view>()));
        EXPECT_EQ(identifier, token->GetValue<std::string_view>());
    }
}

TEST_F(TokenTest, GetValue_double)
{
    {
        std::unique_ptr<Token> token = Token::CreateEof();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        std::unique_ptr<Token> token = Token::CreateDef();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        std::unique_ptr<Token> token = Token::CreateExtern();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        const std::string_view identifier = "foo"sv;
        std::unique_ptr<Token> token = Token::CreateIdentifier(identifier);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        const char c = '(';
        std::unique_ptr<Token> token = Token::CreateCharacter(c);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const double number = 42.0;
        std::unique_ptr<Token> token = Token::CreateNumber(number);
        ASSERT_TRUE(token.get());
        EXPECT_NO_THROW(ignore(token->GetValue<double>()));
        EXPECT_EQ(number, token->GetValue<double>());
    }
}

TEST_F(TokenTest, GetValue_invalid_type)
{
    const double number = 42.0;
    std::unique_ptr<Token> token = Token::CreateNumber(number);
    ASSERT_TRUE(token.get());
    EXPECT_THROW(ignore(token->GetValue<float>()), std::invalid_argument);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
