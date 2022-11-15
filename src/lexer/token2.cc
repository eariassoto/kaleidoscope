#include "lexer/token2.h"

namespace kaleidoscope
{

Token2::~Token2() = default;

Token2::Token2(Type type) : type_(type) {}

/*static*/ std::unique_ptr<Token2> Token2::CreateEofToken()
{
    return std::make_unique<Token2>(Token2::Type::kEof);
}

}  // namespace kaleidoscope
