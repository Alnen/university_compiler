#include "Context.hpp"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "GlobalContext.h"
#include "LabelInfo.hpp"
#include "Module.h"
#include "StdFix.h"

#include <string>
#include <iostream>

namespace PascalCompiler {

Context::Context(const std::string& name, llvm::FunctionType* type, Context* parentContext):
    m_name(name),
    m_parentContext(parentContext)
{
    m_function = llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, getGlobalModule()->getModule());
    m_allocaBlock = registerBasicBlock(getGlobalModule()->getAnonimousName());
    m_blockStack.emplace_back(registerBasicBlock(getGlobalModule()->getAnonimousName()));
}

llvm::BasicBlock* Context::registerBasicBlock(const std::string& name)
{
    auto& context = getGlobalModule()->getLLVMModule().getContext();
    if (m_labelMap.find(name) == m_labelMap.end())
    {
        llvm::BasicBlock* temp = llvm::BasicBlock::Create(context, name, m_function);
        m_labelMap[name] = std::make_unique<LabelInfo>(name, temp);
        return temp;
    }
    else
    {
        throw std::runtime_error("Label with name " + name + " already exists");
    }
}

llvm::BasicBlock* Context::getBasicBlock(const std::string& name) const
{
    decltype(m_labelMap)::const_iterator pos;
    if ((pos = m_labelMap.find(name)) != m_labelMap.end())
    {
        return pos->second.get()->getBasicBlock();
    }
    else
    {
        throw std::runtime_error("No such basic block");
    }
}

const std::string& Context::getName() const
{
    return m_name;
}

Context* Context::getParentContext() const
{
    return m_parentContext;
}

VarInfo* Context::registerVariable(const std::string& name, BasicTypeInfo* type)
{
    if (m_varMap.find(name) == m_varMap.end())
    {
        auto  varInfo = std::make_unique<VarInfo>(name, type);
        VarInfo* varInfoPtr = varInfo.get();
        m_varMap[name] = std::move(varInfo);
        return varInfoPtr;
    }
    else
    {
        throw std::runtime_error("Identificator with name " + name + " already exists");
    }
}

VarInfo* Context::getVariable(const std::string& name)
{
    decltype(m_varMap)::const_iterator pos;
    if ((pos = m_varMap.find(name)) != m_varMap.end())
    {
        return pos->second.get();
    }
    else
    {
        throw std::runtime_error("No such label");
    }
}

BlockPack* Context::getCurrentBlock()
{
    return &m_blockStack.back();
}

BlockPack* Context::pushBlockPack()
{
    m_blockStack.emplace_back(registerBasicBlock(getGlobalModule()->getAnonimousName()));
    return &m_blockStack.back();
}

void Context::popBlockPack()
{
    m_blockStack.pop_back();
}

llvm::BasicBlock* Context::getAllocaBlock()
{
    return m_allocaBlock;
}

}
