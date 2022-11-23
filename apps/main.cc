#include <kaleidoscope/ast/number.h>
#include <kaleidoscope/jit_interpreter.h>
#include <kaleidoscope/lexer_error.h>
#include <kaleidoscope/lexer.h>
#include <kaleidoscope/parser.h>

#include <iostream>

int main()
{
    using namespace kaleidoscope;
    JitInterpreter interpreter;

    while (true) {
        try {
            std::cout << "Eval > ";
            std::string input;
            getline(std::cin, input);
            if (input == "quit") break;

            Parser parser(std::make_unique<Lexer>(std::move(input)));
            if (std::unique_ptr<ast::Expression> expr =
                    parser.ParseNextExpression()) {
                if (ast::Number* number =
                        dynamic_cast<ast::Number*>(expr.get())) {
                    interpreter.GenerateIR(*number);
                    // ugly, fix
                    std::cout << '\n';
                    continue;
                }
                std::string res;
                expr->PrintToString(res, 0, '=');
                std::cout << res;
            }
        } catch (const LexerError& lexer_err) {
            // TODO: Improve error messages
            std::cerr << "Lexer error: " << lexer_err.what();
        }
        // TODO: catch other errors
    }

    return 0;
}