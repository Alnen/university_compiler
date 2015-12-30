#ifndef CUSTOMTYPEINFO_HPP
#define CUSTOMTYPEINFO_HPP

#include "BasicTypeInfo.hpp"
#include <string>

namespace PascalCompiler {

class CustomTypeInfo : public BasicTypeInfo
{
public:
    CustomTypeInfo(BasicTypeInfo* trueType, const std::string& name = "");
    BasicTypeInfo* getTrueType() const;

protected:
    BasicTypeInfo* m_trueType;
};

}

#endif /* CUSTOMTYPEINFO_HPP */
