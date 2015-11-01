#include <iostream>
#include <utility>

#include "Map.h"
#include "RegexASTParser.h"
#include "Lexer.h"

enum class TokenType
{
    ID,
    CI
};

class IDHandler : public Lexer::TokenHandler
{
public:
    virtual TokenPtr operator()(TokenPtr a) override{ return a;}
};

int main (int argc, char** argv)
{
    std::vector<Lexer::Lexer<TokenType>::Rule> rules;
    rules.emplace_back(TokenType::ID, std::string("(a|b)*abb#"), std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::ID, std::string("(a|b)*cdd#"), std::move(std::make_unique<IDHandler>()));

    Lexer::Lexer<TokenType> lexer( std::move(rules), "");

    return 0;
}
