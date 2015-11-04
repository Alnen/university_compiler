#ifndef RULEIMPL_H
#define RULEIMPL_H

#include "TokenHandler.h"
#include "DefaultTokenHandler.h"

#include <memory>
#include <string>
#include <utility>

namespace Lexer
{

template <class _TokenType>
struct RuleImpl
{
    using TokenType = _TokenType;
    using HandlerPtr = std::shared_ptr<TokenHandler<_TokenType>>;

    RuleImpl() = default;
    RuleImpl(TokenType type, std::string regex, HandlerPtr handlerPtr = std::make_shared<DefaultTokenHandler<TokenType>>()):
        m_type(type),
        m_regex(regex),
        m_handlerPtr(std::move(handlerPtr))
    {

    }


    RuleImpl(RuleImpl&& rhs):
        m_type(rhs.m_type),
        m_regex(rhs.m_regex),
        m_handlerPtr(std::move(rhs.m_handlerPtr))
    {

    }

    RuleImpl& operator=(RuleImpl&& rhs)
    {
        m_type = rhs.m_type;
        m_regex = rhs.m_regex;
        m_handlerPtr = std::move(rhs.m_handlerPtr);
    }

    RuleImpl(const RuleImpl&) = delete;
    RuleImpl& operator=(const RuleImpl&) = delete;


    TokenType m_type;
    std::string m_regex;
    HandlerPtr m_handlerPtr;
};

}

#endif // RULEIMPL_H
