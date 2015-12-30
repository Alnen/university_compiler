#ifndef LABELINFO_HPP
#define LABELINFO_HPP

#include "BasicTypeInfo.hpp"
#include <string>
#include "llvm/IR/BasicBlock.h"

namespace PascalCompiler {

class LabelInfo
{
public:
    LabelInfo(std::string name, llvm::BasicBlock* block);

    const std::string& getName() const;

    llvm::BasicBlock* getBasicBlock();

    bool isDefined() const;

    bool setDefined();

protected:
    std::string m_name;
    llvm::BasicBlock* m_block;
    bool m_defined = false;
};

}

#endif /* LABELINFO_HPP */
