#include "EnumerationTypeInfo.hpp"

namespace PascalCompiler {

EnumerationTypeInfo::EnumerationTypeInfo(const std::string& name):
    BasicTypeInfo(BasicTypeInfo::Type::ENUMERATION, name)
{
}

void EnumerationTypeInfo::addItem(const std::string& item)
{
    auto pos = std::find(m_enumerationItems.begin(), m_enumerationItems.end(), item);
    if (pos != m_enumerationItems.end())
    {
        throw std::runtime_error(item + " already in enum type " + getName());
    }
    else
    {
        m_enumerationItems.emplace_back(item);
    }
}

int EnumerationTypeInfo::getID(const std::string& item) const
{
    auto pos = std::find(m_enumerationItems.begin(), m_enumerationItems.end(), item);
    if (pos == m_enumerationItems.end())
    {
        throw std::runtime_error(item + " is not in enum type " + getName());
    }
    else
    {
        return std::distance(m_enumerationItems.begin(), pos);
    }
}

size_t EnumerationTypeInfo::size() const
{
    return m_enumerationItems.size();
}

}
