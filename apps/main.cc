#include <lexer/lexer.h>
#include <parser/parser.h>

#include <iostream>

using kaleidoscope::Token;

void processAndPrint(std::string input)
{
    kaleidoscope::Lexer l(std::move(input));

    kaleidoscope::Token next_t;
    do {
        next_t = l.GetNextToken();
        if (next_t == Token::kTokenIdentifier) {
            std::cout << kaleidoscope::TokenAsString(next_t) << ": "
                      << l.GetIdentifier() << '\n';
        } else if (next_t == Token::kTokenNumber) {
            std::cout << kaleidoscope::TokenAsString(next_t) << ": "
                      << l.GetNumber() << '\n';
        } else if (next_t == Token::kTokenAscii) {
            std::cout << kaleidoscope::TokenAsString(next_t) << ": "
                      << l.GetAscii() << '\n';
        } else
            std::cout << kaleidoscope::TokenAsString(next_t) << '\n';
    } while (next_t != Token::kTokenUnknown && next_t != Token::kTokenEof);
}

int main()
{
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