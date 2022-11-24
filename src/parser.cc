#include "kaleidoscope/parser.h"

#include "kaleidoscope/ast/binary_op.h"
#include "kaleidoscope/ast/fn_call.h"
#include "kaleidoscope/ast/fn_prototype.h"
#include "kaleidoscope/ast/fn.h"
#include "kaleidoscope/ast/number.h"
#include "kaleidoscope/ast/variable.h"
#include "kaleidoscope/lexer_error.h"
#include "kaleidoscope/lexer.h"
#include "kaleidoscope/token.h"

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

bool Parser::IsNextToken(TokenType type)
{
    try {
        const Token t = lexer_->PeekToken();
        std::cout << "IsNextToken " << TokenTypeToString(t.Type) << '\n';
        return t.Type == type;
    } catch (const LexerError& err) {
        return false;
    }
}

bool Parser::IsNextTokenBinOp()
{
    return IsNextToken(TokenType::kPlusSign) ||
           IsNextToken(TokenType::kMinusSign) ||
           IsNextToken(TokenType::kAsterisk);
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
    lexer_->ConsumeToken();
    return nullptr;
}

/// top ::= definition | external | expression
std::unique_ptr<ast::Expression> Parser::ParseNextExpression()
{
    if (IsNextToken(TokenType::kEof)) {
        return nullptr;
    } else if (IsNextToken(TokenType::kDef)) {
        if (auto def = ParseDefinition()) {
            return def;
        }
        // Skip token for error recovery.
        lexer_->ConsumeToken();
        return nullptr;
    } else if (IsNextToken(TokenType::kExtern)) {
        if (auto ext = ParseExtern()) {
            return ext;
        }
        // Skip token for error recovery.
        lexer_->ConsumeToken();
        return nullptr;
    } else if (auto expression = ParseExpression()) {
        return expression;
    }
    // // Skip token for error recovery.
    lexer_->ConsumeToken();
    return nullptr;
}

/// definition ::= 'def' prototype expression
std::unique_ptr<ast::Fn> Parser::ParseDefinition()
{
    lexer_->ConsumeToken();  // eat def.
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
    lexer_->ConsumeToken();  // eat extern.
    return ParsePrototype();
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<ast::FnPrototype> Parser::ParsePrototype()
{
    if (!IsNextToken(TokenType::kIdentifier))
        return LogErrorP("Expected function name in prototype");

    // Consider wrapping this to catch early bugs
    const std::string_view fn_name = lexer_->PeekToken().Value;
    lexer_->ConsumeToken();  // Consume function name

    if (!IsNextToken(TokenType::kLeftParen))
        return LogErrorP("Expected '(' in prototype");
    lexer_->ConsumeToken();  // Consume '('

    // Read the list of argument names.
    std::vector<std::string_view> args_names;
    while (IsNextToken(TokenType::kIdentifier)) {
        args_names.push_back(lexer_->PeekToken().Value);
        lexer_->ConsumeToken();  // Consume identifier
        // TODO, handle error here
    }

    if (!IsNextToken(TokenType::kRightParen))
        return LogErrorP("Expected ')' in prototype");

    // success.
    lexer_->ConsumeToken();  // eat ')'.

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
    else if (IsNextToken(TokenType::kLeftParen))
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

        char bin_op = lexer_->PeekToken().Value.front();
        int curr_token_prec = GetBinOpPrecedence(bin_op);
        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (curr_token_prec < expression_precedence) return lhs_expression;

        lexer_->ConsumeToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto rhs_expression = ParsePrimaryExpression();
        if (!rhs_expression) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        if (IsNextTokenBinOp()) {
            char next_bin_op = lexer_->PeekToken().Value.front();
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
    lexer_->ConsumeToken();  // eat '('
    auto expression = ParseExpression();
    if (!expression) return nullptr;

    if (!IsNextToken(TokenType::kRightParen)) return LogError("expected ')'");
    lexer_->ConsumeToken();  // eat ')'
    return expression;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ast::Expression> Parser::ParseIdentifierExpression()
{
    const std::string_view identifier = lexer_->PeekToken().Value;
    lexer_->ConsumeToken();  // eat identifier
    if (!IsNextToken(TokenType::kLeftParen)) {
        return std::make_unique<ast::Variable>(identifier);
    }

    // Process fn call
    lexer_->ConsumeToken();  // eat '('
    std::vector<std::unique_ptr<ast::Expression>> fn_args;
    if (!IsNextToken(TokenType::kRightParen)) {
        while (true) {
            if (auto arg = ParseExpression())
                fn_args.push_back(std::move(arg));
            else
                return nullptr;

            if (IsNextToken(TokenType::kRightParen)) break;

            if (!IsNextToken(TokenType::kComma))
                return LogError("Expected ')' or ',' in argument list");
            lexer_->ConsumeToken();
        }
    }

    // Eat ')'.
    lexer_->ConsumeToken();

    return std::make_unique<ast::FnCall>(identifier, std::move(fn_args));
}

/// numberexpr ::= number
std::unique_ptr<ast::Expression> Parser::ParseNumberExpression()
{
    const std::string_view value = lexer_->PeekToken().Value;
    lexer_->ConsumeToken();  // eat number

    double num_value;
    auto result =
        std::from_chars(value.data(), value.data() + value.size(), num_value);
    if (result.ec == std::errc::invalid_argument) {
        return LogError("Could not convert number.");
    }
    return std::make_unique<ast::Number>(num_value);
}

}  // namespace kaleidoscope
