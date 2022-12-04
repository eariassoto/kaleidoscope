#include <kaleidoscope/jit_interpreter.h>
#include <kaleidoscope/lexer_error.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>

#include <iostream>

using kaleidoscope::ast::BaseExpression;
using kaleidoscope::JitInterpreter;
using kaleidoscope::Lexer;
using kaleidoscope::LexerError;
using kaleidoscope::parser::ParseNextExpression;

int main()
{
    JitInterpreter interpreter;

    while (true) {
        std::cout << "Eval > ";
        std::string input;
        getline(std::cin, input);
        if (input == "quit") break;
        if (input.empty()) continue;

        Lexer lex(std::move(input));
        if (std::unique_ptr<BaseExpression> expr = ParseNextExpression(&lex)) {
            interpreter.EvaluateExpression(expr.get());
        }
    }

    return 0;
}