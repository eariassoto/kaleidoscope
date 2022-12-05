# Kaleidoscope

This is my own implementation of the [LLVM language](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html) frontend tutorial. This tutorial covers a simple programming language, Kaleidoscope, to introduce basic concepts of LLVM intermediate representation (IR) code generation.

## Building

This project requires:

* A C++17 compatible compiler.
* CMake 3.12+.
* The LLVM libraries (I am using version 14.0.0).

Dependencies fetched by CMake:

* GTest
* [fmtlib](https://github.com/fmtlib/fmt)
* [tl::expected](https://github.com/TartanLlama/expected) (Sy Brand's `std::expected` implementation for C++11/14/17)

Build as any other CMake project. I recommend https://cliutils.gitlab.io/modern-cmake/ as a good reference to work with CMake projects. Generally, this should be enough to build the project:

```bash
$> # Clone the repository
$> # cd /path/to/repo
$> cmake -S . -B build
```

After build, you will have the `kaleidoscope` library, and a simple executable to reads input from the standard input. In addition, there is a GTest executable for unit testing the library.

**Note for building on Windows:**

LLVM pre-built binaries for Windows do not include the CMake files needed to include the project via `find_package`. For this to work, you need to compile and install LLVM from sources. To generate the LLVM solution I use:

```bash
 # Shallow clone LLVM repo release 14.0.0
 git clone git@github.com:llvm/llvm-project.git \
     --branch llvmorg-14.0.0 \
     --depth 1
 cd llvm-project
 mkdir build
 cd build
 cmake -S ../llvm -G "Visual Studio 17 2022" \
     -DLLVM_ENABLE_PROJECTS=llvm \
     -DCMAKE_BUILD_TYPE=Release \
     -DLLVM_ENABLE_THREADS=off \
     -DLLVM_BUILD_TESTS=off \
     -DLLVM_INCLUDE_TESTS=off \
     -DCMAKE_INSTALL_PREFIX=<path-to-install>
```

This will create the `build/LLVM.sln` solution file, build this in Visual Studio. Make sure to build the `INSTALL` target. Once is built, add a new enviroment variable to set `LLVM_ROOT` as `<path-to-install>\lib\cmake\llvm`.



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