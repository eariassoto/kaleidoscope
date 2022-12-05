#ifndef TESTS_MOCK_LEXER_H
#define TESTS_MOCK_LEXER_H

#include "kaleidoscope/lexer.h"

#include "gmock/gmock.h"

namespace kaleidoscope
{

class MockLexer : public Lexer
{
   public:
    MOCK_METHOD((tl::expected<Token, LexerError>), PeekToken, (), (override));
    MOCK_METHOD(void, ConsumeToken, (), (override));
};
}  // namespace kaleidoscope
#endif  // TESTS_MOCK_LEXER_H
