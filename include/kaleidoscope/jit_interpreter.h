#ifndef KALEIDOSCOPE_JIT_INTERPRETER_H
#define KALEIDOSCOPE_JIT_INTERPRETER_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace kaleidoscope
{

namespace ast
{
class Expression;
}

class JitInterpreter
{
   public:
    JitInterpreter();
    JitInterpreter(const JitInterpreter& t) = delete;
    JitInterpreter& operator=(const JitInterpreter&) = delete;
    ~JitInterpreter();

    void EvaluateExpression(const ast::Expression* expression);

   private:
    llvm::Value* GenerateIR(const ast::Expression* expression);

        private : std::unique_ptr<llvm::LLVMContext> context_ = nullptr;
    std::unique_ptr<llvm::Module> module_ = nullptr;
    std::unique_ptr<llvm::IRBuilder<>> ir_builder_ = nullptr;
    std::unordered_map<std::string, llvm::Value*> named_values;
};
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_JIT_INTERPRETER_H