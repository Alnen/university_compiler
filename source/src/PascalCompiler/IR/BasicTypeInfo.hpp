#ifndef BASICINFOTYPE_HPP
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
        CUSTOM
    };

    BasicTypeInfo(Type type, std::string name = "");

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
