#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "BasicTypeInfo.hpp"
#include "LabelInfo.hpp"
#include "VarInfo.hpp"
#include "llvm/IR/DerivedTypes.h"
#include "boost/container/flat_map.hpp"

namespace PascalCompiler {

const std::string gInitBlockName = "allocation_block";

class BlockPack
{
public:
    BlockPack(llvm::BasicBlock* initBlock):
        m_firstBlock(initBlock),
        m_finalBlock(initBlock)
    {

    }

    llvm::BasicBlock* getFirstBlock() const
    {
        return m_firstBlock;
    }
    llvm::BasicBlock* getFinalBlock() const
    {
        return m_finalBlock;
    }

    void setFinalBlock(llvm::BasicBlock* lastBlock)
    {
        m_finalBlock = lastBlock;
    }

private:
    llvm::BasicBlock* m_firstBlock;
    llvm::BasicBlock* m_finalBlock;
};

class Context
{
public:
    Context(const std::string& name, llvm::FunctionType* type, Context* parentContext = nullptr);

    llvm::BasicBlock* registerBasicBlock(const std::string& name);

    llvm::BasicBlock* getBasicBlock(const std::string& name) const;

    const std::string& getName() const;

    Context* getParentContext() const;

    VarInfo* registerVariable(const std::string& name, BasicTypeInfo* type);

    VarInfo* getVariable(const std::string& name);

    BlockPack* getCurrentBlock();

    BlockPack* pushBlockPack();
    void popBlockPack();

    llvm::BasicBlock* getAllocaBlock();

    llvm::Function* getFunctionLLVMType() const
    {
        return m_function;
    }

private:
    std::string   m_name;
    Context*      m_parentContext;
    llvm::BasicBlock* m_allocaBlock;
    std::vector<BlockPack> m_blockStack;
    boost::container::flat_map<std::string, std::unique_ptr<VarInfo>>        m_varMap;
    boost::container::flat_map<std::string, std::unique_ptr<BasicTypeInfo>>  m_constantType;
    boost::container::flat_map<std::string, std::unique_ptr<LabelInfo>>      m_labelMap;
    llvm::Function* m_function;
};

}

#endif /* CONTEXT_HPP */
