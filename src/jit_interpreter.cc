#include "kaleidoscope/jit_interpreter.h"

#include "kaleidoscope/ast/binary_op.h"
#include "kaleidoscope/ast/expression.h"
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

llvm::Value* JitInterpreter::GenerateIR(const ast::Expression* expression)
{
    llvm::Value* val = nullptr;
    if (const ast::Number* number =
            dynamic_cast<const ast::Number*>(expression)) {
        return llvm::ConstantFP::get(*context_, llvm::APFloat(number->Value));
    }
    if (const ast::BinaryOp* bin_op =
            dynamic_cast<const ast::BinaryOp*>(expression)) {
        auto lhs = GenerateIR(bin_op->LhsOp.get());
        auto rhs = GenerateIR(bin_op->RhsOp.get());
        if (!lhs || !rhs) {
            return nullptr;
        }
        switch (bin_op->Op) {
            case '+':
                return ir_builder_->CreateFAdd(lhs, rhs, "addtmp");
            case '-':
                return ir_builder_->CreateFSub(lhs, rhs, "subtmp");
            case '*':
                return ir_builder_->CreateFMul(lhs, rhs, "multmp");
            default:
                return nullptr;
        }
    }
    return nullptr;
}

void JitInterpreter::EvaluateExpression(const ast::Expression* expression)
{
    if (llvm::Value* val = GenerateIR(expression)) {
        val->print(llvm::outs());
        std::cout << '\n';
    } else {
        std::string res;
        expression->PrintToString(res, 0, '=');
        std::cout << res;
    }
}

JitInterpreter::~JitInterpreter() = default;

}  // namespace kaleidoscope
