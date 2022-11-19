#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include "ast/expression.h"
#include "ast/fn.h"
#include "ast/fn_prototype.h"

#include <memory>
#include <string>

namespace kaleidoscope
{

class Lexer;

class Parser
{
   public:
    Parser() = delete;

    Parser(std::unique_ptr<Lexer> lexer);
    ~Parser();

    std::unique_ptr<ast::Expression> ParseNextExpression();
    // std::unique_ptr<ast::Fn> ParseNextTopLevelExpression();

   private:
    std::unique_ptr<Lexer> lexer_;

   private:
   bool IsNextToken(TokenType type);
    // void AdvanceToNextToken();
    // int GetCurrBinOpTokenPrecedence() const;

    // std::unique_ptr<ast::Expression> ParseNumberExpression();
    // std::unique_ptr<ast::Expression> ParseParenthesesExpression();
    // std::unique_ptr<ast::Expression> ParseIdentifierExpression();
    // std::unique_ptr<ast::Expression> ParsePrimaryExpression();
    // std::unique_ptr<ast::Expression> ParseExpression();
    // std::unique_ptr<ast::Expression> ParseBinaryOpRhs(
    //     int expression_precedence,
    //     std::unique_ptr<ast::Expression> lhs_expression);
    // std::unique_ptr<ast::FnPrototype> ParsePrototype();
    // std::unique_ptr<ast::Fn> ParseDefinition();
    // std::unique_ptr<ast::FnPrototype> ParseExtern();
};
}  // namespace kaleidoscope

#endif  // LEXER_PARSER_H