#ifndef VARINFO_HPP
#define VARINFO_HPP

#include "GlobalContext.h"
#include "BasicTypeInfo.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"

namespace PascalCompiler {

class VarInfo
{
public:
    VarInfo(const std::string& name, BasicTypeInfo* type);

    const std::string& getName() const;
    BasicTypeInfo* getType() const;
    llvm::AllocaInst* getAllocaInst() const;

protected:
    std::string m_name;
    BasicTypeInfo*   m_type;
    llvm::AllocaInst* m_alloca;
};

}

#endif /* VARINFO_HPP */
