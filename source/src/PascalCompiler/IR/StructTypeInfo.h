#ifndef STRUCTTYPEINFO_H
#define STRUCTTYPEINFO_H

#include "BasicTypeInfo.hpp"
#include <vector>
#include <utility>
#include <boost/container/flat_map.hpp>

namespace PascalCompiler {

class StructTypeInfo : public BasicTypeInfo
{
public:
    using MemberInfo = std::pair<std::string, BasicTypeInfo*>;
    using MemberVector = std::vector<MemberInfo>;

    StructTypeInfo(const MemberVector& dimensions, const std::string& name);

    std::pair<size_t, BasicTypeInfo*> getMemberInfo(const std::string& memberName) const;

protected:
    MemberVector m_memberInfo;
};

}

#endif /* STRUCTTYPEINFO_H */
