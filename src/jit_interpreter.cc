#include "kaleidoscope/jit_interpreter.h"

#include "kaleidoscope/ast/number.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>

namespace kaleidoscope
{

JitInterpreter::JitInterpreter()
    : context_(std::make_unique<llvm::LLVMContext>()),
      module_(std::make_unique<llvm::Module>("JIT Interpreter", *context_)),
      ir_builder_(std::make_unique<llvm::IRBuilder<>>(*context_))
{
}

void JitInterpreter::GenerateIR(const ast::Number& expression)
{
    llvm::Value* val =
        llvm::ConstantFP::get(*context_, llvm::APFloat(expression.Value));
    if (val) val->print(llvm::outs());
}

JitInterpreter::~JitInterpreter() = default;

}  // namespace kaleidoscope
