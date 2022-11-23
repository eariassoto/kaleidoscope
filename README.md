# Kaleidoscope

This is my own implementation of the [LLVM language](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html) frontend tutorial. This tutorial covers a simple programming language, Kaleidoscope, to introduce basic concepts of LLVM intermediate representation (IR) code generation.

## Building

This project requires:

* A C++17 compatible compiler.
* CMake 3.12+.
* The LLVM libraries (I am using version 14.0.0).
* GTest (fetched by CMake).
* fmtlib (fetched by CMake).

Build as any other CMake project. I recommend https://cliutils.gitlab.io/modern-cmake/ as a good reference to work with CMake projects. Generally, this should be enough to build the project:

```bash
$> # Clone the repository
$> # cd /path/to/repo
$> cmake -S . -B build
```

After build, you will have the `kaleidoscope` library, and a simple executable to reads input from the standard input. In addition, there is a GTest executable for unit testing the library.

## Getting started

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