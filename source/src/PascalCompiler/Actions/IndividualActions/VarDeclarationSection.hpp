#ifndef VARDELCARATIONSECTION_HPP
#define VARDELCARATIONSECTION_HPP

#include "BaseHandlers.h"
#include "../../IR/GlobalContext.h"
#include "../../IR/Context.hpp"
#include "../../IR/Module.h"

namespace PascalCompiler
{

// {VarSection,{RWV,VarSection1,VarSection2}, TreeConstructor},
// {VarSection2,{VarSection1,VarSection2}, TreeConstructor},
// {VarSection2,{EPSILON}, TreeConstructor},

// {VarSection1,{IdList,SRCN,Type,SRSM}, TreeConstructor},

#include <iostream>

class VariableDeclarationAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::cout << "DEBUG" << std::endl;
        std::vector<std::string> id_list = cast_item<std::vector<std::string>>(m_stack[1], "idlist");
        BasicTypeInfo* varType = cast_item<BasicTypeInfo*>(m_stack[3], "type");
        Context* currentContext = getGlobalModule()->getCurrentContext();
        for (const auto& variableName : id_list)
        {
            currentContext->registerVariable(variableName, varType);
        }
    }
};

}

#endif /* VARDELCARATIONSECTION_HPP */
