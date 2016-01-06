#include "CustomTypeInfo.hpp"
#include "GlobalContext.h"

namespace PascalCompiler {

CustomTypeInfo::CustomTypeInfo(BasicTypeInfo* trueType, const std::string& name):
    BasicTypeInfo(BasicTypeInfo::CUSTOM),
    m_trueType(trueType)
{
    m_name = name;
    m_llvmType = trueType->getLLVMType();
}

BasicTypeInfo* CustomTypeInfo::getTrueType() const
{
    return m_trueType;
}

}
