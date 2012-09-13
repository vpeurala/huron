#ifndef HURON_JIT_H
#define HURON_JIT_H

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>

struct huron_object;

struct huron_function {
    LLVMValueRef            func;
    LLVMModuleRef            module;
    LLVMExecutionEngineRef        engine;
};

struct huron_function *huron_function_compile(struct huron_object *expr);

struct huron_object *huron_function_call(struct huron_function *f);

void huron_function_delete(struct huron_function *f);

void huron_jit_init(void);

#endif
