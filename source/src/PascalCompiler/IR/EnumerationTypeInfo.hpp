#ifndef ENUMERATIONINFO_HPP
#define ENUMERATIONINFO_HPP

#include "BasicTypeInfo.hpp"
#include <string>

namespace PascalCompiler {

class EnumerationTypeInfo: public BasicTypeInfo
{
public:
    EnumerationTypeInfo(const std::string& name = "");

    void addItem(const std::string& item);

    int getID(const std::string& item) const;

    size_t size() const;

private:
    std::vector<std::string> m_enumerationItems;
};

}

#endif /* ENUMERATIONINFO_HPP */
