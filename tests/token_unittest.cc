#include "lexer/token.h"

#include "lexer/token.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace kaleidoscope;
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

TEST_F(TokenTest, Ctor_NoValueTokens)
{
    try {
        Token t(TokenType::kCharacter);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type must have a value.");
    }
    try {
        Token t(TokenType::kIdentifier);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type must have a value.");
    }
    try {
        Token t(TokenType::kNumber);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type must have a value.");
    }

    try {
        Token t(TokenType::kDef);
        EXPECT_EQ(t.GetType(), TokenType::kDef);
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }
    try {
        Token t(TokenType::kEof);
        EXPECT_EQ(t.GetType(), TokenType::kEof);
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }
    try {
        Token t(TokenType::kExtern);
        EXPECT_EQ(t.GetType(), TokenType::kExtern);
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }
}

TEST_F(TokenTest, Ctor_WithValueTokens)
{
    try {
        Token t(TokenType::kIdentifier, "foo"sv);
        EXPECT_EQ(t.GetType(), TokenType::kIdentifier);
        EXPECT_EQ("foo"sv, t.GetValue());
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }
    try {
        Token t(TokenType::kNumber, "1234"sv);
        EXPECT_EQ(t.GetType(), TokenType::kNumber);
        EXPECT_EQ("1234"sv, t.GetValue());
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }
    try {
        Token t(TokenType::kCharacter, "+"sv);
        EXPECT_EQ(t.GetType(), TokenType::kCharacter);
        EXPECT_EQ("+"sv, t.GetValue());
    } catch (const std::invalid_argument& ex) {
        FAIL() << "This ctor should not throw";
    }

    try {
        Token t(TokenType::kDef, "value"sv);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type cannot have a value.");
    }
    try {
        Token t(TokenType::kEof, "value"sv);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type cannot have a value.");
    }
    try {
        Token t(TokenType::kExtern, "value"sv);
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type cannot have a value.");
    }
}

TEST_F(TokenTest, GetValue_Exceptions)
{
    try {
        Token t(TokenType::kDef);
        auto value = t.GetValue();
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type does not have a value.");
    }
    try {
        Token t(TokenType::kEof);
        auto value = t.GetValue();
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type does not have a value.");
    }
    try {
        Token t(TokenType::kExtern);
        auto value = t.GetValue();
        FAIL() << "Expected an exception to be thrown";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "Token type does not have a value.");
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
