#ifndef KALEIDOSCOPE_JIT_INTERPRETER_H
#define KALEIDOSCOPE_JIT_INTERPRETER_H

#include <llvm/IR/LLVMContext.h>

namespace kaleidoscope
{

class JitInterpreter
{
   public:
    JitInterpreter();
    JitInterpreter(const JitInterpreter& t) = delete;
    JitInterpreter& operator=(const JitInterpreter&) = delete;
    ~JitInterpreter();

    private:
    llvm::LLVMContext context_;
};
}  // namespace kaleidoscope

#endif // KALEIDOSCOPE_JIT_INTERPRETER_H