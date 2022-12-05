#include <kaleidoscope/jit_interpreter.h>
#include <kaleidoscope/lexer_error.h>
#include <kaleidoscope/lexer_impl.h>
#include <kaleidoscope/parser.h>

#include <iostream>

using kaleidoscope::JitInterpreter;
using kaleidoscope::LexerImpl;
using kaleidoscope::LexerError;
using kaleidoscope::ast::BaseExpression;
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

        LexerImpl lex(std::move(input));
        if (std::unique_ptr<BaseExpression> expr = ParseNextExpression(&lex)) {
            interpreter.EvaluateExpression(expr.get());
        }
    }

    return 0;
}