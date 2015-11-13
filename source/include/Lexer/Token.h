#ifndef TOKEN_H
#define TOKEN_H

#include <boost/any.hpp>

namespace Lexer
{

template <class _TokenType>
class Token
{
public:
    using TokenType = _TokenType;

    Token(TokenType type, std::string value):
        m_type(type),
        m_value(value)
    {

    }

    Token(const Token& token):
        m_type(token.m_type),
        m_value(token.m_value)
    {

    }

    void operator=(const Token& token)
    {
        m_type = token.m_type;
        m_value = token.m_value;
    }

    Token() = default;

    TokenType type() const { return m_type; }
    boost::any& value() { return m_value; }

    void set_type(TokenType type) { m_type = type; }

protected:
    TokenType  m_type;
    boost::any m_value;
};

}

#endif // TOKEN_H

