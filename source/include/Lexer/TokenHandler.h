#ifndef TOKENHANDLER_H
#define TOKENHANDLER_H

#include "Token.h"

#include <memory>

namespace Lexer
{

template <class _TokenType>
class TokenHandler
{
public:
    using TokenType = _TokenType;
    using TokenPtr = std::unique_ptr<Token<TokenType>>;

    virtual TokenPtr operator()(TokenPtr token) = 0;
};

}

#endif // TOKENHANDLER_H
