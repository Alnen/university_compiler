#include "ArrayTypeInfo.hpp"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"

namespace PascalCompiler {

ArrayTypeInfo::ArrayTypeInfo(const DimensionsVector& dimensions, BasicTypeInfo* elementType):
    BasicTypeInfo(BasicTypeInfo::ARRAY),
    m_dimensions(dimensions),
    m_elementType(elementType)
{
    if (m_dimensions.empty())
    {
        throw std::runtime_error("Array must have at least one nonzero dimension");
    }
    if (std::find_if(m_dimensions.begin(), m_dimensions.end(), [](const DimensionInfo& dimension) { return dimension.first == 0; }) != m_dimensions.end())
    {
        throw std::runtime_error("Dimensions of array must have nonzero size");
    }
    m_name = "array@" + elementType->getName();

    llvm::Type* currentLLVMType = elementType->getLLVMType();
    for (int i = m_dimensions.size()-1; i >= 0; --i)
    {
        // only integer indexes supported
        currentLLVMType = llvm::ArrayType::get(currentLLVMType, m_dimensions[i].first);
    }
    m_llvmType = currentLLVMType;
}

BasicTypeInfo* ArrayTypeInfo::getElementType() const
{
    return m_elementType;
}

const ArrayTypeInfo::DimensionsVector& ArrayTypeInfo::getDimensions() const
{
    return m_dimensions;
}

}
