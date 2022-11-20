# Kaleidoscope
Toy interpreter inspired by LLVM Kaleidoscope tutorial

## Lexer
```
<<EOF>>              EOF
def                  DEF
extern               EXTERN
[a-zA-Z][a-zA-Z0-9]* IDENTIFIER
[0-9]+               NUMBER
(                    LEFT_PAREN
)                    RIGHT_PAREN
+                    PLUS_SIGN
-                    MINUS_SIGN
,                    COMMA
.                    DOT
```

## Parser

```
toplevelexpr ::= expression

top ::= definition | external | expression

definition ::= DEF prototype expression

external ::= EXTERN prototype

prototype
  ::= IDENTIFIER '(' IDENTIFIER* ')'

expression
  ::= primary binoprhs

primary
  ::= identifierexpr
  ::= numberexpr
  ::= parenexpr

binoprhs
  ::= ('+' primary)*

binopsign ::= PLUS_SIGN | MINUS_SIGN | ASTERISK

parenexpr ::= LEFT_PAREN expression RIGHT_PAREN

identifierexpr
  ::= IDENTIFIER
  ::= IDENTIFIER LEFT_PAREN expression* RIGHT_PAREN

numberexpr ::= NUMBER

```