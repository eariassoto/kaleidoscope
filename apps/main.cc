#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>
#include <kaleidoscope/jit_interpreter.h>

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
            std::string res;
            expr->PrintToString(res, 0, '=');
            std::cout << res;
        }
    }

    return 0;
}