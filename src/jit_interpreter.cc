#include "kaleidoscope/jit_interpreter.h"

#include "kaleidoscope/ast/base_expression.h"
#include "kaleidoscope/ast/binary_op.h"
#include "kaleidoscope/ast/fn_call.h"
#include "kaleidoscope/ast/number.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include <iostream>

namespace kaleidoscope
{

JitInterpreter::JitInterpreter()
    : context_(std::make_unique<llvm::LLVMContext>()),
      module_(std::make_unique<llvm::Module>("JIT Interpreter", *context_)),
      ir_builder_(std::make_unique<llvm::IRBuilder<>>(*context_))
{
}

llvm::Value* JitInterpreter::GenerateIR(const ast::BaseExpression* expression)
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
    if (const ast::FnCall* fn_call =
            dynamic_cast<const ast::FnCall*>(expression)) {
        llvm::Function* callee_fn = module_->getFunction(fn_call->Callee);
        if (!callee_fn) {
            std::cerr << "Unknown function referenced\n";
            return nullptr;
        }
        // If argument mismatch error.
        if (callee_fn->arg_size() != fn_call->Args.size()) {
            std::cerr << "Incorrect # arguments passed\n";
            return nullptr;
        }
        std::vector<llvm::Value*> args_ir;
        for (const auto& arg : fn_call->Args) {
            llvm::Value* arg_ir = GenerateIR(arg.get());
            if (!arg_ir) return nullptr;
            args_ir.push_back(arg_ir);
        }

        return ir_builder_->CreateCall(callee_fn, args_ir, "calltmp");
    }

    return nullptr;
}

void JitInterpreter::EvaluateExpression(const ast::BaseExpression* expression)
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
