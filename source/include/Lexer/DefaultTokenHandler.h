#ifndef DEFAULTETOKKENHANDLER_H
#define DEFAULTETOKKENHANDLER_H

#include "TokenHandler.h"

namespace Lexer
{

template <class _TokenType>
class DefaultTokenHandler : public TokenHandler<_TokenType>
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

#endif // DEFAULTETOKKENHANDLER_H
