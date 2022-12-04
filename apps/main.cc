#include <kaleidoscope/ast/number.h>
#include <kaleidoscope/jit_interpreter.h>
#include <kaleidoscope/lexer_error.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>

#include <iostream>

using kaleidoscope::JitInterpreter;
using kaleidoscope::Lexer;
using kaleidoscope::LexerError;
using kaleidoscope::ast::Expression;
using kaleidoscope::parser::ParseNextExpression;

int main()
{
    JitInterpreter interpreter;

    while (true) {
        try {
            std::cout << "Eval > ";
            std::string input;
            getline(std::cin, input);
            if (input == "quit") break;
            if (input.empty()) continue;

            Lexer lex(std::move(input));
            if (std::unique_ptr<Expression> expr = ParseNextExpression(&lex)) {
                interpreter.EvaluateExpression(expr.get());
            }
        } catch (const LexerError& lexer_err) {
            // TODO: Improve error messages
            std::cerr << "Lexer error: " << lexer_err.what();
        }
        // TODO: catch other errors
    }

    return 0;
}