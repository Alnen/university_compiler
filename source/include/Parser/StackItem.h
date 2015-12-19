#ifndef STACKITEM_H
#define STACKITEM_H

#include <memory>
#include <vector>
#include <boost/any.hpp>
#include <boost/container/flat_map.hpp>

namespace Parser {

class StackItem
{
public:
    using value = boost::container::flat_map<std::string, boost::any>;
    using value_pointer = std::shared_ptr<value>;
    enum class Type
    {
        NONTERMINAL = 0,
        INHERIT,
        SYNTHESIZE
    };

    StackItem(Type type): m_type(type), m_value(new value) {}
    Type type() const { return m_type; }

    value_pointer& getValue()
    {
        return m_value;
    }

    virtual ~StackItem() {}

protected:


    Type            m_type;
    value_pointer   m_value;
};

class NonterminalItem : public StackItem
{
public:
    NonterminalItem(): StackItem(Type::NONTERMINAL)
    {
    }
};

class BaseStackedItem : public StackItem
{
public:
    using value         = StackItem::value;
    using value_pointer = StackItem::value_pointer;

    void push(value_pointer item)
    {
        m_stack.push_back(std::move(item));
    }

    virtual void executeHandler()
    {
        if (!m_stack.empty())
        {
           m_value = m_stack[0];
        }
    }

protected:
    BaseStackedItem(Type type): StackItem(type) {}

    std::vector<value_pointer> m_stack;
};

class BaseInheritItem : public BaseStackedItem
{
public:
     BaseInheritItem(): BaseStackedItem(Type::INHERIT) {}
};

class BaseSynthesizeItem : public BaseStackedItem
{
public:
     BaseSynthesizeItem(): BaseStackedItem(Type::SYNTHESIZE) {}
};

}
#endif // STACKITEM_H
