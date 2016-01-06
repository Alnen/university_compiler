#ifndef ARRAYTYPEINFO_HPP
#define ARRAYTYPEINFO_HPP

#include "BasicTypeInfo.hpp"
#include <vector>
#include <utility>

namespace PascalCompiler {

class ArrayTypeInfo : public BasicTypeInfo
{
public:
    using DimensionInfo = std::pair<size_t, BasicTypeInfo*>;
    using DimensionsVector = std::vector<DimensionInfo>;

    ArrayTypeInfo(const DimensionsVector& dimensions, BasicTypeInfo* elementType);

    BasicTypeInfo* getElementType() const;
    const DimensionsVector& getDimensions() const;

protected:
    DimensionsVector m_dimensions;
    BasicTypeInfo* m_elementType = nullptr;
};

}

#endif /* ARRAYTYPEINFO_HPP */
