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
// Forward declarations
std::unique_ptr<ast::Fn> ParseNextTopLevelExpression(Lexer* lexer);
std::unique_ptr<ast::Fn> ParseDefinition(Lexer* lexer);
std::unique_ptr<ast::FnPrototype> ParseExtern(Lexer* lexer);
std::unique_ptr<ast::FnPrototype> ParsePrototype(Lexer* lexer);
std::unique_ptr<ast::BaseExpression> ParseExpression(Lexer* lexer);
std::unique_ptr<ast::BaseExpression> ParsePrimaryExpression(Lexer* lexer);
std::unique_ptr<ast::BaseExpression> ParseBinaryOpRhs(
    Lexer* lexer, int expression_precedence,
    std::unique_ptr<ast::BaseExpression> lhs_expression);
std::unique_ptr<ast::BaseExpression> ParseParenthesesExpression(Lexer* lexer);
std::unique_ptr<ast::BaseExpression> ParseIdentifierExpression(Lexer* lexer);
std::unique_ptr<ast::BaseExpression> ParseNumberExpression(Lexer* lexer);
}  // namespace

namespace parser
{
/// top ::= definition | external | expression
std::unique_ptr<ast::BaseExpression> ParseNextExpression(Lexer* lexer)
{
    const tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const Token& token = peek_token.value();
    if (token.Type == TokenType::kEof) {
        return nullptr;
    } else if (token.Type == TokenType::kDef) {
        if (auto def = ParseDefinition(lexer)) {
            return def;
        }
        // Skip token for error recovery.
        lexer->ConsumeToken();
        return nullptr;
    } else if (token.Type == TokenType::kExtern) {
        if (auto ext = ParseExtern(lexer)) {
            return ext;
        }
        // Skip token for error recovery.
        lexer->ConsumeToken();
        return nullptr;
    } else if (auto expression = ParseExpression(lexer)) {
        return expression;
    }
    // // Skip token for error recovery.
    lexer->ConsumeToken();
    return nullptr;
}
}  // namespace parser

namespace
{
std::unique_ptr<ast::BaseExpression> LogError(const std::string& err_str)
{
    std::cerr << "LogError: " << err_str << '\n';
    return nullptr;
}

std::unique_ptr<ast::FnPrototype> LogErrorP(const std::string& err_str)
{
    std::cerr << "LogError: " << err_str << '\n';
    return nullptr;
}

int GetBinOpPrecedence(const Token& token)
{
    // if (value == '<') return 10;
    if (token.Type == TokenType::kPlusSign) return 20;
    if (token.Type == TokenType::kMinusSign) return 20;
    if (token.Type == TokenType::kAsterisk) return 40;
    throw std::invalid_argument("Not found");
}

bool IsNextTokenBinOp(const Token& token)
{
    return token.Type == TokenType::kPlusSign ||
           token.Type == TokenType::kMinusSign ||
           token.Type == TokenType::kAsterisk;
}

/// toplevelexpr ::= expression
std::unique_ptr<ast::Fn> ParseNextTopLevelExpression(Lexer* lexer)
{
    if (auto expression = ParseExpression(lexer)) {
        // Make an anonymous proto.
        auto proto = std::make_unique<ast::FnPrototype>(
            "", std::vector<std::string_view>());
        return std::make_unique<ast::Fn>(std::move(proto),
                                         std::move(expression));
    }
    // Skip token for error recovery.
    lexer->ConsumeToken();
    return nullptr;
}

/// definition ::= 'def' prototype expression
std::unique_ptr<ast::Fn> ParseDefinition(Lexer* lexer)
{
    lexer->ConsumeToken();  // eat def.
    auto proto = ParsePrototype(lexer);
    if (!proto) return nullptr;

    if (auto expression = ParseExpression(lexer))
        return std::make_unique<ast::Fn>(std::move(proto),
                                         std::move(expression));
    return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<ast::FnPrototype> ParseExtern(Lexer* lexer)
{
    lexer->ConsumeToken();  // eat extern.
    return ParsePrototype(lexer);
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<ast::FnPrototype> ParsePrototype(Lexer* lexer)
{
    tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kIdentifier)
        return LogErrorP("Expected function name in prototype");

    // Consider wrapping this to catch early bugs
    const std::string_view fn_name = peek_token->Value;
    lexer->ConsumeToken();  // Consume function name

    peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kLeftParen)
        return LogErrorP("Expected '(' in prototype");

    lexer->ConsumeToken();  // Consume '('

    // Read the list of argument names.
    std::vector<std::string_view> args_names;
    // TODO: Parse this as comma separated identifiers
    while (true) {
        if (const tl::expected<Token, LexerError> arg_token =
                lexer->PeekToken()) {
            if (arg_token->Type == TokenType::kIdentifier) {
                args_names.push_back(arg_token->Value);
                lexer->ConsumeToken();  // Consume identifier
            } else {
                break;
            }

        } else {
            // TODO: handle error
            return nullptr;
        }
    }

    peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kRightParen)
        return LogErrorP("Expected ')' in prototype");

    // success.
    lexer->ConsumeToken();  // eat ')'.

    return std::make_unique<ast::FnPrototype>(fn_name, std::move(args_names));
}

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ast::BaseExpression> ParseExpression(Lexer* lexer)
{
    auto lhs_op = ParsePrimaryExpression(lexer);
    if (!lhs_op) return nullptr;

    return ParseBinaryOpRhs(lexer, 0, std::move(lhs_op));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ast::BaseExpression> ParsePrimaryExpression(Lexer* lexer)
{
    const tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type == TokenType::kIdentifier)
        return ParseIdentifierExpression(lexer);
    else if (peek_token->Type == TokenType::kNumber)
        return ParseNumberExpression(lexer);
    else if (peek_token->Type == TokenType::kLeftParen)
        return ParseParenthesesExpression(lexer);
    else
        return LogError("Unknown token when expecting an expression");
}

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ast::BaseExpression> ParseBinaryOpRhs(
    Lexer* lexer, int expression_precedence,
    std::unique_ptr<ast::BaseExpression> lhs_expression)
{
    // If this is a binop, find its precedence.
    while (true) {
        const tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
        if (!peek_token) {
            // TODO: Handle error
            return nullptr;
        }
        if (!IsNextTokenBinOp(*peek_token)) return lhs_expression;
        char bin_op = peek_token->Value.front();
        int curr_token_prec = GetBinOpPrecedence(*peek_token);
        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise we are done.
        if (curr_token_prec < expression_precedence) return lhs_expression;

        lexer->ConsumeToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto rhs_expression = ParsePrimaryExpression(lexer);
        if (!rhs_expression) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        const tl::expected<Token, LexerError> next_peek_token =
            lexer->PeekToken();
        if (!next_peek_token) {
            // TODO: Handle error
            return nullptr;
        }
        if (IsNextTokenBinOp(*next_peek_token)) {
            if (curr_token_prec < GetBinOpPrecedence(*next_peek_token)) {
                rhs_expression =
                    ParseBinaryOpRhs(lexer, expression_precedence + 1,
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
std::unique_ptr<ast::BaseExpression> ParseParenthesesExpression(Lexer* lexer)
{
    lexer->ConsumeToken();  // eat '('
    auto expression = ParseExpression(lexer);
    if (!expression) return nullptr;

    const tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kRightParen)
        return LogError("expected ')'");
    lexer->ConsumeToken();  // eat ')'
    return expression;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ast::BaseExpression> ParseIdentifierExpression(Lexer* lexer)
{
    tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const std::string_view identifier = peek_token->Value;
    lexer->ConsumeToken();  // eat identifier

    peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kLeftParen) {
        return std::make_unique<ast::Variable>(identifier);
    }

    // Process fn call
    lexer->ConsumeToken();  // eat '('
    std::vector<std::unique_ptr<ast::BaseExpression>> fn_args;

    tl::expected<Token, LexerError> arg_token = lexer->PeekToken();
    if (!arg_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (arg_token->Type != TokenType::kRightParen) {
        while (true) {
            if (auto arg = ParseExpression(lexer)) {
                fn_args.push_back(std::move(arg));
            } else {
                return nullptr;
            }

            arg_token = lexer->PeekToken();
            if (!arg_token) {
                // TODO: Handle error
                return nullptr;
            }
            if (arg_token->Type == TokenType::kRightParen) break;
            if (arg_token->Type != TokenType::kComma)
                return LogError("Expected ')' or ',' in argument list");
            lexer->ConsumeToken();
        }
    }

    // Eat ')'.
    lexer->ConsumeToken();

    return std::make_unique<ast::FnCall>(identifier, std::move(fn_args));
}

/// numberexpr ::= number
std::unique_ptr<ast::BaseExpression> ParseNumberExpression(Lexer* lexer)
{
    const tl::expected<Token, LexerError> peek_token = lexer->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const std::string_view value = peek_token->Value;
    lexer->ConsumeToken();  // eat number

    double num_value;
    auto result =
        std::from_chars(value.data(), value.data() + value.size(), num_value);
    if (result.ec == std::errc::invalid_argument) {
        return LogError("Could not convert number.");
    }
    return std::make_unique<ast::Number>(num_value);
}
}  // namespace

}  // namespace kaleidoscope
