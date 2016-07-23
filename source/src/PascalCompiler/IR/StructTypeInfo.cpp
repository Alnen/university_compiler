#include "StructTypeInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "GlobalContext.h"
#include "Module.h"

namespace PascalCompiler {

StructTypeInfo::StructTypeInfo(const MemberVector& members, const std::string& name):
    BasicTypeInfo(BasicTypeInfo::STRUCT),
    m_memberInfo(members)
{
    m_name = name;
    // members must be unique
    if (m_memberInfo.empty())
    {
        throw std::runtime_error("Array must have at least one nonzero dimension");
    }

    std::vector<llvm::Type*> typeVector;
    for (const auto& member : m_memberInfo)
    {
        typeVector.emplace_back(member.second->getLLVMType());
    }

    auto& context = getGlobalModule()->getLLVMModule().getContext();
    llvm::StructType* llvmType = llvm::StructType::create(context, llvm::ArrayRef<llvm::Type*>(typeVector), m_name);
    m_llvmType = llvmType;
}

std::pair<size_t, BasicTypeInfo*> StructTypeInfo::getMemberInfo(const std::string& memberName) const
{
    MemberVector::const_iterator pos;
    if ((pos = std::find_if(m_memberInfo.begin(), m_memberInfo.end(),[memberName](const MemberInfo& memberInfo)
                                {
                                    return memberInfo.first == memberName;
                                }
                            )) == m_memberInfo.end())
    {
        throw std::runtime_error("Couldn't find member with name " + memberName);
    }
    return { std::distance(m_memberInfo.begin(), pos), pos->second};
}

}
