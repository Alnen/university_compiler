#ifndef PROGRAMACTIONS_HPP
#define PROGRAMACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/GlobalContext.h"
#include "../../IR/Module.h"
#include "../../IR/Context.hpp"
#include <iostream>

namespace PascalCompiler {

// {Program ,{RWPR,ID,SRSM , Program1, TreeConstructor}, TreeConstructor},
// {Program1,{ProcedureFunctions,CompoundStatement,SRSP}, TreeConstructor},
class FinishMainFuntion : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        std::cout << "***asasadasd****" << std::endl;
        auto& context = PascalCompiler::getGlobalModule()->getLLVMModule().getContext();
        TreeConstructor::executeHandler();
        BlockPack* mainBlockPack = getGlobalModule()->getCurrentContext()->getCurrentBlock();
        llvm::Value* returnValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true);
        llvm::IRBuilder<> irBuilder(getGlobalModule()->getCurrentContext()->getAllocaBlock());
        irBuilder.CreateBr(mainBlockPack->getFirstBlock());
        irBuilder.SetInsertPoint(mainBlockPack->getFinalBlock());
        irBuilder.CreateRet(returnValue);
    }

protected:


};

// {Program1,{DescriptionSection1,DescriptionSection,ProcedureFunctions,CompoundStatement,SRSP}, TreeConstructor},
// {DescriptionSection,{EPSILON}, TreeConstructor},
// {DescriptionSection,{DescriptionSection1,DescriptionSection}, TreeConstructor},
// {DescriptionSection1,{LabelSection}, TreeConstructor},
// {DescriptionSection1,{ConstSection}, TreeConstructor},
// {DescriptionSection1,{TypeSection}, TreeConstructor},
// {DescriptionSection1,{VarSection}, TreeConstructor},
// {DescriptionSection1,{OperatorOverloadSection}, TreeConstructor},
// Default

}
#endif /* PROGRAMACTIONS_HPP */
