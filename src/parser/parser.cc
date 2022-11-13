#include "parser/parser.h"

#include "ast/binary_op.h"
#include "ast/number.h"
#include "ast/fn_call.h"
#include "ast/variable.h"
#include "lexer/lexer.h"
#include "ast/fn_prototype.h"
#include "ast/fn.h"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace kaleidoscope
{
namespace
{
std::unique_ptr<ast::Expression> LogError(const std::string& err_str)
{
    std::cerr << "LogError: " << err_str << '\n';
    return nullptr;
}

std::unique_ptr<ast::FnPrototype> LogErrorP(const std::string& err_str)
{
    std::cerr << "LogError: " << err_str << '\n';
    return nullptr;
}
}  // namespace

Parser::Parser(std::unique_ptr<Lexer> lexer) : m_Lexer(std::move(lexer)) {}

Parser::~Parser() = default;

Token Parser::GetNextToken()
{
    return m_CurrentToken = m_Lexer->GetNextToken();
}

bool Parser::IsCurrentTokenAscii(char ch) const
{
    if (m_CurrentToken != Token::kTokenAscii) return false;
    return m_Lexer->GetAscii() == ch;
}

bool Parser::IsCurrentTokenBinOp() const
{
    if (m_CurrentToken != Token::kTokenAscii) return false;
    return m_Lexer->GetAscii() == '+' || m_Lexer->GetAscii() == '-' ||
           m_Lexer->GetAscii() == '*' || m_Lexer->GetAscii() == '<';
}

int Parser::GetCurrBinOpTokenPrecedence() const
{
    if (m_Lexer->GetAscii() == '<') return 10;
    if (m_Lexer->GetAscii() == '+') return 20;
    if (m_Lexer->GetAscii() == '-') return 20;
    if (m_Lexer->GetAscii() == '*') return 40;
    throw std::range_error("Not found");
}

/// numberexpr ::= number
std::unique_ptr<ast::Expression> Parser::ParseNumberExpression()
{
    GetNextToken();  // eat number
    return std::make_unique<ast::Number>(m_Lexer->GetNumber());
}

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ast::Expression> Parser::ParseParenthesesExpression()
{
    GetNextToken();  // eat '('
    auto expression = ParseExpression();
    if (!expression) return nullptr;

    if (!IsCurrentTokenAscii(')')) return LogError("expected ')'");
    GetNextToken();  // eat ')'
    return expression;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ast::Expression> Parser::ParseIdentifierExpression()
{
    std::string_view identifier = m_Lexer->GetIdentifier();
    GetNextToken();  // eat identifier
    if (!IsCurrentTokenAscii('(')) {
        return std::make_unique<ast::Variable>(identifier);
    }

    // Process fn call
    GetNextToken();  // eat '('
    std::vector<std::unique_ptr<ast::Expression>> fn_args;
    if (!IsCurrentTokenAscii(')')) {
        while (true) {
            if (auto arg = ParseExpression())
                fn_args.push_back(std::move(arg));
            else
                return nullptr;

            if (IsCurrentTokenAscii(')')) break;

            if (!IsCurrentTokenAscii(','))
                return LogError("Expected ')' or ',' in argument list");
            GetNextToken();
        }
    }

    // Eat ')'.
    GetNextToken();

    return std::make_unique<ast::FnCall>(identifier, std::move(fn_args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ast::Expression> Parser::ParsePrimaryExpression()
{
    if (m_CurrentToken == Token::kTokenIdentifier)
        return ParseIdentifierExpression();
    else if (m_CurrentToken == Token::kTokenNumber)
        return ParseNumberExpression();
    else if (IsCurrentTokenAscii('('))
        return ParseParenthesesExpression();
    else
        return LogError("Unknown token when expecting an expression");
}

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ast::Expression> Parser::ParseBinaryOpRhs(
    int expression_precedence, std::unique_ptr<ast::Expression> lhs_expression)
{
    // If this is a binop, find its precedence.
    while (true) {
        if (!IsCurrentTokenBinOp()) return lhs_expression;

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (GetCurrBinOpTokenPrecedence() < expression_precedence)
            return lhs_expression;

        char bin_op = m_Lexer->GetAscii();
        GetNextToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto rhs_expression = ParsePrimaryExpression();
        if (!rhs_expression) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        if (IsCurrentTokenBinOp()
                 && expression_precedence < GetCurrBinOpTokenPrecedence()) {
            rhs_expression = ParseBinaryOpRhs(expression_precedence + 1,
                                              std::move(rhs_expression));
            if (!rhs_expression) return nullptr;
        }

        // Merge LHS/RHS.
        lhs_expression = std::make_unique<ast::BinaryOp>(
            bin_op, std::move(lhs_expression), std::move(rhs_expression));
    }
}

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ast::Expression> Parser::ParseExpression()
{
    auto lhs_op = ParsePrimaryExpression();
    if (!lhs_op) return nullptr;

    return ParseBinaryOpRhs(0, std::move(lhs_op));
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<ast::FnPrototype> Parser::ParsePrototype()
{
    if (m_CurrentToken != Token::kTokenIdentifier)
        return LogErrorP("Expected function name in prototype");

    const std::string_view fn_name = m_Lexer->GetIdentifier();
    GetNextToken();

    if (!IsCurrentTokenAscii('('))
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string_view> args_names;
    while (GetNextToken() == Token::kTokenIdentifier)
        args_names.push_back(m_Lexer->GetIdentifier());

    if (!IsCurrentTokenAscii(')'))
        return LogErrorP("Expected ')' in prototype");

    // success.
    GetNextToken();  // eat ')'.

    return std::make_unique<ast::FnPrototype>(fn_name, std::move(args_names));
}

/// definition ::= 'def' prototype expression
std::unique_ptr<ast::Fn> Parser::ParseDefinition()
{
    GetNextToken();  // eat def.
    auto proto = ParsePrototype();
    if (!proto) return nullptr;

    if (auto expression = ParseExpression())
        return std::make_unique<ast::Fn>(std::move(proto),
                                         std::move(expression));
    return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<ast::FnPrototype> Parser::ParseExtern()
{
    GetNextToken();  // eat extern.
    return ParsePrototype();
}

/// toplevelexpr ::= expression
std::unique_ptr<ast::Fn> Parser::ParseNextTopLevelExpression()
{
    if (auto expression = ParseExpression()) {
        // Make an anonymous proto.
        auto proto = std::make_unique<ast::FnPrototype>(
            "", std::vector<std::string_view>());
        return std::make_unique<ast::Fn>(std::move(proto),
                                         std::move(expression));
    }
    // Skip token for error recovery.
    GetNextToken();
    return nullptr;
}

/// top ::= definition | external | expression
std::unique_ptr<ast::Expression> Parser::ParseNextExpression()
{
    GetNextToken();
    if (m_CurrentToken == Token::kTokenEof)
        return nullptr;
    else if (m_CurrentToken == Token::kTokenDef) {
        if (auto def = ParseDefinition()) {
            std::cout << "Parsed a function definition.\n";
            return def;
        }
        // Skip token for error recovery.
        GetNextToken();
        return nullptr;
    } else if (m_CurrentToken == Token::kTokenExtern) {
        if (auto ext = ParseExtern()) {
            std::cout << "Parsed an extern\n";
            return ext;
        } else {
            // Skip token for error recovery.
            GetNextToken();
        }
    }
    else if (auto expression = ParseExpression()) {
        std::cout << "Parsed an expression\n";
        return expression;
    }
    // Skip token for error recovery.
    GetNextToken();
    return nullptr;
}

}  // namespace kaleidoscope
