#include "lexer/token2.h"

#include <gtest/gtest.h>

#include <stdexcept>

using kaleidoscope::Token2;
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
    std::unique_ptr<Token2> token = Token2::CreateEof();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token2::Type::kEof, token->GetType());
}

TEST_F(TokenTest, CreateDef)
{
    std::unique_ptr<Token2> token = Token2::CreateDef();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token2::Type::kDef, token->GetType());
}

TEST_F(TokenTest, CreateExtern)
{
    std::unique_ptr<Token2> token = Token2::CreateExtern();
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token2::Type::kExtern, token->GetType());
}

TEST_F(TokenTest, CreateIdentifier)
{
    const std::string_view identifier = "foo"sv;
    std::unique_ptr<Token2> token = Token2::CreateIdentifier(identifier);
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token2::Type::kIdentifier, token->GetType());
    EXPECT_EQ(identifier, token->GetValue<std::string_view>());
}

TEST_F(TokenTest, CreateNumber)
{
    const std::string_view number = "42.0"sv;
    std::unique_ptr<Token2> token = Token2::CreateNumber(number);
    ASSERT_TRUE(token.get());
    EXPECT_EQ(Token2::Type::kNumber, token->GetType());
    EXPECT_EQ(42.0, token->GetValue<double>());
}

TEST_F(TokenTest, GetValue_string_view)
{
    {
        std::unique_ptr<Token2> token = Token2::CreateEof();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        std::unique_ptr<Token2> token = Token2::CreateDef();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        std::unique_ptr<Token2> token = Token2::CreateExtern();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const std::string_view number = "42.0"sv;
        std::unique_ptr<Token2> token = Token2::CreateNumber(number);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<std::string_view>()),
                     std::invalid_argument);
    }
    {
        const std::string_view identifier = "foo"sv;
        std::unique_ptr<Token2> token = Token2::CreateIdentifier(identifier);
        ASSERT_TRUE(token.get());
        EXPECT_NO_THROW(ignore(token->GetValue<std::string_view>()));
        EXPECT_EQ(identifier, token->GetValue<std::string_view>());
    }
}

TEST_F(TokenTest, GetValue_double)
{
    {
        std::unique_ptr<Token2> token = Token2::CreateEof();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        std::unique_ptr<Token2> token = Token2::CreateDef();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        std::unique_ptr<Token2> token = Token2::CreateExtern();
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        const std::string_view identifier = "foo"sv;
        std::unique_ptr<Token2> token = Token2::CreateIdentifier(identifier);
        ASSERT_TRUE(token.get());
        EXPECT_THROW(ignore(token->GetValue<double>()), std::invalid_argument);
    }
    {
        const std::string_view number = "42.0"sv;
        std::unique_ptr<Token2> token = Token2::CreateNumber(number);
        ASSERT_TRUE(token.get());
        EXPECT_NO_THROW(ignore(token->GetValue<double>()));
        EXPECT_EQ(42.0, token->GetValue<double>());
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
