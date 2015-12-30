#include "GlobalContext.h"
#include "Module.h"
#include "StdFix.h"
#include <memory>

namespace PascalCompiler {

std::unique_ptr<Module> globalModule;

void initModule(const std::string& name)
{
    globalModule = std::make_unique<Module>(name);
}

PascalCompiler::Module* getGlobalModule()
{
    return globalModule.get();
}

}
