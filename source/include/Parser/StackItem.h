#ifndef STACKITEM_H
#define STACKITEM_H

#include <memory>

#include <boost/any.hpp>

namespace Parser {

class StackItem
{
public:
    enum class Type
    {
        NONTERMINAL = 0,
        INHERIT,
        SYNTHESIZE
    };

    StackItem(Type type): m_type(type), m_value(new boost::any) {}
    Type type() const { return m_type; }

    std::shared_ptr<boost::any>& getValue()
    {
        return m_value;
    }

    virtual ~StackItem() {}

protected:
    Type    m_type;
    std::shared_ptr<boost::any> m_value;
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
    using value = boost::any;
    using value_pointer = std::shared_ptr<value>;

    void push(std::shared_ptr<boost::any> item)
    {
        m_stack.push_back(std::move(item));
    }

    virtual void executeHandler()
    {
        if (!m_stack.empty())
        {
           *m_value = m_stack[0];
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
