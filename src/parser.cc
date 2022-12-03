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
}  // namespace

Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)) {}

Parser::~Parser() = default;

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
    const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const Token& token = peek_token.value();
    if (token.Type == TokenType::kEof) {
        return nullptr;
    } else if (token.Type == TokenType::kDef) {
        if (auto def = ParseDefinition()) {
            return def;
        }
        // Skip token for error recovery.
        lexer_->ConsumeToken();
        return nullptr;
    } else if (token.Type == TokenType::kExtern) {
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
    tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kIdentifier)
        return LogErrorP("Expected function name in prototype");

    // Consider wrapping this to catch early bugs
    const std::string_view fn_name = peek_token->Value;
    lexer_->ConsumeToken();  // Consume function name

    peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kLeftParen)
        return LogErrorP("Expected '(' in prototype");

    lexer_->ConsumeToken();  // Consume '('

    // Read the list of argument names.
    std::vector<std::string_view> args_names;
    // TODO: Parse this as comma separated identifiers
    while (true) {
        if (const tl::expected<Token, LexerError> arg_token =
                lexer_->PeekToken()) {
            if (arg_token->Type == TokenType::kIdentifier) {
                args_names.push_back(arg_token->Value);
                lexer_->ConsumeToken();  // Consume identifier
            } else {
                break;
            }

        } else {
            // TODO: handle error
            return nullptr;
        }
    }

    peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kRightParen)
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
    const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type == TokenType::kIdentifier)
        return ParseIdentifierExpression();
    else if (peek_token->Type == TokenType::kNumber)
        return ParseNumberExpression();
    else if (peek_token->Type == TokenType::kLeftParen)
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
        const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
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

        lexer_->ConsumeToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto rhs_expression = ParsePrimaryExpression();
        if (!rhs_expression) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        const tl::expected<Token, LexerError> next_peek_token =
            lexer_->PeekToken();
        if (!next_peek_token) {
            // TODO: Handle error
            return nullptr;
        }
        if (IsNextTokenBinOp(*next_peek_token)) {
            if (curr_token_prec < GetBinOpPrecedence(*next_peek_token)) {
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

    const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kRightParen)
        return LogError("expected ')'");
    lexer_->ConsumeToken();  // eat ')'
    return expression;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ast::Expression> Parser::ParseIdentifierExpression()
{
    const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const std::string_view identifier = peek_token->Value;
    lexer_->ConsumeToken();  // eat identifier
    if (peek_token->Type != TokenType::kLeftParen) {
        return std::make_unique<ast::Variable>(identifier);
    }

    // Process fn call
    lexer_->ConsumeToken();  // eat '('
    std::vector<std::unique_ptr<ast::Expression>> fn_args;

    tl::expected<Token, LexerError> arg_token = lexer_->PeekToken();
    if (!arg_token) {
        // TODO: Handle error
        return nullptr;
    }
    if (peek_token->Type != TokenType::kRightParen) {
        while (true) {
            if (auto arg = ParseExpression())
                fn_args.push_back(std::move(arg));
            else
                return nullptr;

            arg_token = lexer_->PeekToken();
            if (!arg_token) {
                // TODO: Handle error
                return nullptr;
            }

            if (peek_token->Type == TokenType::kRightParen) break;

            if (peek_token->Type != TokenType::kComma)
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
    const tl::expected<Token, LexerError> peek_token = lexer_->PeekToken();
    if (!peek_token) {
        // TODO: Handle error
        return nullptr;
    }
    const std::string_view value = peek_token->Value;
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
