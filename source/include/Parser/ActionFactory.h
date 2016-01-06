#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include "StackItem.h"

#include <memory>

#include <boost/mpl/map.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

#include <boost/hana/map.hpp>
#include <boost/hana/for_each.hpp>

template <class ActionMapping, class TokenType>
class ActionFactory
{
public:
    using ActionPtr = std::unique_ptr<Parser::BaseStackedItem>;
    ActionFactory()
    {
        boost::hana::for_each(ActionMapping(), [this](auto pair){
            m_map[std::remove_reference<decltype(boost::hana::first(pair))>::type::value] = [](){
                return std::make_unique<typename std::remove_reference<decltype(boost::hana::second(pair))>::type::type>();
            };
        });
    }

    ActionPtr operator()(TokenType action)
    {
        std::cout << "Action " << action << std::endl;
        return m_map[action]();
    }

protected:
    boost::container::flat_map<int, std::function<ActionPtr()>> m_map;
};

#endif // ACTIONFACTORY_H
