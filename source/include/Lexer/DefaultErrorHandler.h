#ifndef DEFAULTERRORHANDLER_H
#define DEFAULTERRORHANDLER_H

#include "TokenHandler.h"

namespace Lexer
{

template <class _TokenType>
class DefaultErrorHandler : public TokenHandler<_TokenType>
{
public:
    using TokenType = _TokenType;
    using TokenPtr = typename TokenHandler<TokenType>::TokenPtr;

    virtual TokenPtr operator()(TokenPtr token) override
    {
        return token;
    }
};

}

#endif // DEFAULTERRORHANDLER_H
