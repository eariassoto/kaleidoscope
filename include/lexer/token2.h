#ifndef LEXER_TOKEN2_H
#define LEXER_TOKEN2_H

#include <memory>

namespace kaleidoscope
{
class Token2
{
   public:
    enum class Type {
        kEof,

        // reserved words
        kDef,
        kExtern,

        kIdentifier,
        kNumber,
        kCharacter
    };

    Token2() = delete;
    ~Token2();

    static std::unique_ptr<Token2> CreateEofToken();

   private:
    explicit Token2(Type type);

   private:
    Type type_;

    friend class TokenTest;
};

}  // namespace kaleidoscope

#endif  // LEXER_TOKEN2_H
