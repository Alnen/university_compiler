#ifndef GLOBALCONTEXT_H
#define GLOBALCONTEXT_H

#include <string>

namespace PascalCompiler {

class Module;

void initModule(const std::string& name);
Module* getGlobalModule();

}


#endif // GLOBALCONTEXT_H
