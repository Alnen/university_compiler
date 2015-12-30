#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include "StackItem.h"

#include <memory>

#include <boost/mpl/map.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

template <class ActionMapping, class TokenType>
class ActionFactory
{
public:
    using ActionPtr = std::unique_ptr<Parser::BaseStackedItem>;
    ActionFactory()
    {
        mapping_constructor functor(*this);
        boost::mpl::for_each<ActionMapping>(functor);
    }

    ActionPtr operator()(TokenType action)
    {
        std::cout << "Action " << action << std::endl;
        return m_map[action]();
    }

protected:
    boost::container::flat_map<int, std::function<ActionPtr()>> m_map;

private:
    class mapping_constructor
    {
    public:
        mapping_constructor(ActionFactory& factory): m_factory(factory) {}

        template <class P>
        void operator()(P pair)
        {
            m_factory.m_map[decltype(pair)::first::value] = [](){ return std::make_unique<typename decltype(pair)::second>(); };
        }

    protected:
        ActionFactory& m_factory;
    };
};

#endif // ACTIONFACTORY_H
