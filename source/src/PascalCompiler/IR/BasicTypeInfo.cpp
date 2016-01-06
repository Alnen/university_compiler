#include "BasicTypeInfo.hpp"
#include "llvm/IR/Type.h"
#include "llvm/IR/TypeBuilder.h"

namespace PascalCompiler {

BasicTypeInfo::BasicTypeInfo(Type type):
    m_currentType(type),
    m_llvmType(nullptr)
{
}

const std::string& BasicTypeInfo::getName() const
{
    return m_name;
}

void BasicTypeInfo::setName(const std::string& name)
{
    m_name = name;
}

BasicTypeInfo::Type BasicTypeInfo::getType() const
{
    return m_currentType;
}

llvm::Type* BasicTypeInfo::getLLVMType() const
{
    return m_llvmType;
}

}
