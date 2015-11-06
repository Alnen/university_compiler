#ifndef RULEHANDLER_H
#define RULEHANDLER_H

#include "boost/any.hpp"
#include <vector>

template <class TokenType>
class RuleHandler
{
public:
    virtual void operator()(std::vector<TokenType>& firstStack, std::vector<TokenType>& secondStack)
    {

    }
};

#endif
