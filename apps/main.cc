#include <kaleidoscope/ast/number.h>
#include <kaleidoscope/jit_interpreter.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>

#include <iostream>

int main()
{
    kaleidoscope::JitInterpreter interpreter;

    while (true) {
        std::cout << "Eval > ";
        std::string input;
        getline(std::cin, input);
        if (input == "quit") break;

        auto lexer = std::make_unique<kaleidoscope::Lexer>(std::move(input));
        kaleidoscope::Parser parser(std::move(lexer));
        if (auto expr = parser.ParseNextExpression()) {
            if (auto number =
                    dynamic_cast<kaleidoscope::ast::Number*>(expr.get())) {
                interpreter.GenerateIR(*number);
                // ugly, fix
                std::cout << '\n';
                continue;
            }
            std::string res;
            expr->PrintToString(res, 0, '=');
            std::cout << res;
        }
    }

    return 0;
}