source/src/PascalCompiler/IR/BasicTypeInfo.cpp#ifndef BASICINFOTYPE_HPP
#define BASICINFOTYPE_HPP

#include "BasicTypeInfo.hpp"
#include "llvm/IR/Type.h"

namespace PascalCompiler {

class BasicTypeInfo
{
public:
    enum Type
    {
        ARRAY,
        INTEGER,
        RATIONAL,
        COMPLEX,
        ENUMERATION,
        STRUCT,
        CUSTOM
    };

    BasicTypeInfo(Type type);

    const std::string& getName() const;
    void setName(const std::string& name);
    Type getType() const;
    llvm::Type* getLLVMType() const;

protected:
    Type            m_currentType;
    std::string     m_name;
    llvm::Type*     m_llvmType;
};

}

#endif /* BASICINFOTYPE_HPP */
