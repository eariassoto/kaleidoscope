#include "parser/parser.h"

#include "ast/binary_op.h"
#include "ast/fn_call.h"
#include "ast/fn_prototype.h"
#include "ast/fn.h"
#include "ast/number.h"
#include "ast/variable.h"
#include "lexer/lexer_error.h"
#include "lexer/lexer.h"

#include <charconv>
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

int GetBinOpPrecedence(char value)
{
    // if (value == '<') return 10;
    if (value == '+') return 20;
    if (value == '-') return 20;
    if (value == '*') return 40;
    throw std::invalid_argument("Not found");
}
}  // namespace

Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)) {}

Parser::~Parser() = default;

void Parser::ConsumeToken() { lexer_->ConsumeToken(); }

Token Parser::PeekToken() { return lexer_->PeekToken(); }

bool Parser::IsNextToken(TokenType type)
{
    try {
        Token t = lexer_->PeekToken();
        return t.GetType() == type;
    } catch (const LexerError& err) {
        return false;
    }
}

bool Parser::IsNextTokenCharacter(char ch)
{
    try {
        Token t = lexer_->PeekToken();
        if (t.GetType() != TokenType::kCharacter) return false;
        return t.GetValue().starts_with(ch);
    } catch (const LexerError& err) {
        return false;
    }
}

bool Parser::IsNextTokenBinOp()
{
    try {
        Token t = lexer_->PeekToken();
        if (t.GetType() != TokenType::kCharacter) return false;
        return t.GetValue().starts_with('+') || t.GetValue().starts_with('-') ||
               t.GetValue().starts_with('*');
    } catch (const LexerError& err) {
        return false;
    }
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
    ConsumeToken();
    return nullptr;
}

/// top ::= definition | external | expression
std::unique_ptr<ast::Expression> Parser::ParseNextExpression()
{
    if (IsNextToken(TokenType::kEof)) {
        return nullptr;
    } else if (IsNextToken(TokenType::kEof)) {
        if (auto def = ParseDefinition()) {
            return def;
        }
        // Skip token for error recovery.
        ConsumeToken();
        return nullptr;
    } else if (IsNextToken(TokenType::kExtern)) {
        if (auto ext = ParseExtern()) {
            return ext;
        }
        // Skip token for error recovery.
        ConsumeToken();
        return nullptr;
    }
    else if (auto expression = ParseExpression()) {
        return expression;
    }
    // // Skip token for error recovery.
    ConsumeToken();
    return nullptr;
}

/// definition ::= 'def' prototype expression
std::unique_ptr<ast::Fn> Parser::ParseDefinition()
{
    ConsumeToken();  // eat def.
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
    ConsumeToken();  // eat extern.
    return ParsePrototype();
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<ast::FnPrototype> Parser::ParsePrototype()
{
    if (!IsNextToken(TokenType::kIdentifier))
        return LogErrorP("Expected function name in prototype");

    const std::string_view fn_name = PeekToken().GetValue();
    ConsumeToken();  // Consume function name

    if (!IsNextTokenCharacter('('))
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string_view> args_names;
    while (IsNextToken(TokenType::kIdentifier))
        args_names.push_back(PeekToken().GetValue());

    if (!IsNextTokenCharacter(')'))
        return LogErrorP("Expected ')' in prototype");

    // success.
    ConsumeToken();  // eat ')'.

    return std::make_unique<ast::FnPrototype>(fn_name, std::move(args_names));
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

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ast::Expression> Parser::ParsePrimaryExpression()
{
    if (IsNextToken(TokenType::kIdentifier))
        return ParseIdentifierExpression();
    else if (IsNextToken(TokenType::kNumber))
        return ParseNumberExpression();
    else if (IsNextTokenCharacter('('))
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
        if (!IsNextTokenBinOp()) return lhs_expression;

        char bin_op = PeekToken().GetValue().front();
        int curr_token_prec = GetBinOpPrecedence(bin_op);
        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (curr_token_prec < expression_precedence) return lhs_expression;

        ConsumeToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto rhs_expression = ParsePrimaryExpression();
        if (!rhs_expression) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        if (IsNextTokenBinOp()) {
            char next_bin_op = PeekToken().GetValue().front();
            if (curr_token_prec < GetBinOpPrecedence(next_bin_op)) {
                rhs_expression = ParseBinaryOpRhs(expression_precedence + 1,
                                                  std::move(rhs_expression));
                if (!rhs_expression) return nullptr;
            }
        }

        // Merge LHS/RHS.
        lhs_expression = std::make_unique<ast::BinaryOp>(
            bin_op, std::move(lhs_expression), std::move(rhs_expression));
    }
}

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ast::Expression> Parser::ParseParenthesesExpression()
{
    ConsumeToken();  // eat '('
    auto expression = ParseExpression();
    if (!expression) return nullptr;

    if (!IsNextTokenCharacter(')')) return LogError("expected ')'");
    ConsumeToken();  // eat ')'
    return expression;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ast::Expression> Parser::ParseIdentifierExpression()
{
    std::string_view identifier = PeekToken().GetValue();
    ConsumeToken();  // eat identifier
    if (!IsNextTokenCharacter('(')) {
        return std::make_unique<ast::Variable>(identifier);
    }

    // Process fn call
    ConsumeToken();  // eat '('
    std::vector<std::unique_ptr<ast::Expression>> fn_args;
    if (!IsNextTokenCharacter(')')) {
        while (true) {
            if (auto arg = ParseExpression())
                fn_args.push_back(std::move(arg));
            else
                return nullptr;

            if (IsNextTokenCharacter(')')) break;

            if (!IsNextTokenCharacter(','))
                return LogError("Expected ')' or ',' in argument list");
            ConsumeToken();
        }
    }

    // Eat ')'.
    ConsumeToken();

    return std::make_unique<ast::FnCall>(identifier, std::move(fn_args));
}

/// numberexpr ::= number
std::unique_ptr<ast::Expression> Parser::ParseNumberExpression()
{
    std::string_view value = PeekToken().GetValue();
    ConsumeToken();  // eat number
    
    double num_value;
    auto result =
        std::from_chars(value.data(), value.data() + value.size(), num_value);
    if (result.ec == std::errc::invalid_argument) {
        return LogError("Could not convert number.");
    }
    return std::make_unique<ast::Number>(num_value);
}

}  // namespace kaleidoscope
