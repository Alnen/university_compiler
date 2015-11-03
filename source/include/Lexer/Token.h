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

    TokenType type() const { return m_type; }
    boost::any& value() { return m_value; }

    void set_type(TokenType type) { m_type = type; }

protected:
    TokenType  m_type;
    boost::any m_value;
};

}

#endif // TOKEN_H

