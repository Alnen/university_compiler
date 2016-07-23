#include "GlobalContext.h"
#include "Module.h"
#include "StdFix.h"
#include <memory>
#include "llvm/IR/LLVMContext.h"

namespace PascalCompiler {

std::unique_ptr<Module> globalModule;

void initModule(const std::string& name, llvm::LLVMContext& context)
{
    globalModule = std::make_unique<Module>(name, context);
}

PascalCompiler::Module* getGlobalModule()
{
    return globalModule.get();
}

}
