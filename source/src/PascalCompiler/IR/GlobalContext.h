#ifndef GLOBALCONTEXT_H
#define GLOBALCONTEXT_H

#include <string>
#include <llvm/IR/LLVMContext.h>


namespace PascalCompiler {

class Module;

void initModule(const std::string& name, llvm::LLVMContext& context);
Module* getGlobalModule();

}


#endif // GLOBALCONTEXT_H
