#include "LabelInfo.hpp"
#include "GlobalContext.h"

namespace PascalCompiler {

LabelInfo::LabelInfo(std::string name, llvm::BasicBlock* block):
    m_name(name),
    m_block(block)
{
}

const std::string& LabelInfo::getName() const
{
    return m_name;
}

llvm::BasicBlock* LabelInfo::getBasicBlock()
{
    return m_block;
}

bool LabelInfo::isDefined() const
{
    return m_defined;
}

bool LabelInfo::setDefined()
{
    m_defined = true;
}

}
