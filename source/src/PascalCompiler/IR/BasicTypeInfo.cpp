#include "BasicTypeInfo.hpp"
#include "llvm/IR/Type.h"
#include "llvm/IR/TypeBuilder.h"

namespace PascalCompiler {

BasicTypeInfo::BasicTypeInfo(Type type, std::string name):
    m_currentType(type),
    m_llvmType(nullptr)
{
    switch (type)
    {
    case Type::INTEGER:
        m_name = "integer";
        m_llvmType  = llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext());
        break;

    case Type::ENUMERATION:
        m_name = "enumeration";
        m_llvmType  = llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext());
        break;

    case Type::RATIONAL:
    {
        // RealType
        llvm::StructType*   rational_type = llvm::StructType::create(llvm::getGlobalContext(), "rational");
        rational_type->setBody({ llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext()),
                                 llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext()) });
        m_name = "rational";
        m_llvmType = llvm::StructType::create(llvm::getGlobalContext(), "rational");
        break;
    }

    case Type::COMPLEX:
    {
        // ComplexType
        llvm::StructType*   rational_type = llvm::StructType::create(llvm::getGlobalContext(), "rational");
        rational_type->setBody({ llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext()),
                                 llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext()) });
        llvm::StructType*   complex_type = llvm::StructType::create(llvm::getGlobalContext(), "complex");
        complex_type->setBody({ rational_type, rational_type });
        m_name = "complex";
        m_llvmType = llvm::StructType::create(llvm::getGlobalContext(), "complex");
        break;
    }

    case Type::CUSTOM:
        m_name = name;

        break;
    }
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
