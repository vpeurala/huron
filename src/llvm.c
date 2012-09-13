#include "huron/jit.h"

#include "huron/object.h"
#include "huron/eval.h"

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern int huron_debug;

struct huron_function *huron_function_compile(struct huron_object *expr)
{
    struct huron_function *f;
    char *error = NULL;

    f = calloc(1, sizeof *f);
    if (!f)
        return NULL;

    f->module = LLVMModuleCreateWithName("module");

    LLVMModuleProviderRef provider = LLVMCreateModuleProviderForExistingModule(f->module);
    if (LLVMCreateJITCompiler(&f->engine, provider, 2, &error) != 0) {
        fprintf(stderr, "%s\n", error);
        LLVMDisposeMessage(error);
        return NULL;
    }

    LLVMTypeRef return_type = LLVMPointerType(LLVMInt8Type(), 0);

    f->func = LLVMAddFunction(f->module, "function", LLVMFunctionType(return_type, NULL, 0, 0));

    LLVMSetFunctionCallConv(f->func, LLVMCCallConv);

    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMBasicBlockRef end = LLVMAppendBasicBlock(f->func, "end");
    LLVMPositionBuilderAtEnd(builder, end);
    LLVMValueRef number = LLVMConstInt(LLVMInt64Type(), (unsigned long) expr, 1);
    LLVMValueRef res = LLVMBuildIntToPtr(builder, number, LLVMPointerType(LLVMInt8Type(), 0), "");

    LLVMBuildRet(builder, res);

    LLVMPassManagerRef pass = LLVMCreatePassManager();
    LLVMAddTargetData(LLVMGetExecutionEngineTargetData(f->engine), pass);
    LLVMRunPassManager(pass, f->module);

    if (huron_debug)
        LLVMDumpModule(f->module);

    LLVMDisposePassManager(pass);
    LLVMDisposeBuilder(builder);

    return f;
}

void huron_function_delete(struct huron_function *f)
{
    LLVMDisposeExecutionEngine(f->engine);

    free(f);
}

struct huron_object *huron_function_call(struct huron_function *f)
{
    LLVMGenericValueRef result;
    void *ret;

    result = LLVMRunFunction(f->engine, f->func, 0, NULL);

    ret = LLVMGenericValueToPointer(result);

    LLVMDisposeGenericValue(result);

    return ret;
}

void huron_jit_init(void)
{
    LLVMLinkInJIT();
    LLVMInitializeNativeTarget();
}
