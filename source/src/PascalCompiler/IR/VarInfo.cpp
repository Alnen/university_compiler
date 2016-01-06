#include "GlobalContext.h"
#include "VarInfo.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "Module.h"

namespace PascalCompiler {

VarInfo::VarInfo(const std::string& name, BasicTypeInfo* type):
    m_name(name),
    m_type(type)
{
    llvm::BasicBlock* block = getGlobalModule()->getCurrentContext()->getAllocaBlock();
    llvm::IRBuilder<> irbuilder(block);
    m_alloca = irbuilder.CreateAlloca(type->getLLVMType(), nullptr, name);
}

const std::string& VarInfo::getName() const
{
    return m_name;
}

BasicTypeInfo* VarInfo::getType() const
{
    return m_type;
}

llvm::AllocaInst* VarInfo::getAllocaInst() const
{
    return m_alloca;
}

}
