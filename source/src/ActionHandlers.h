#include "Parser/SyntaxAnalyzer.h"

namespace PascalParser
{

class Action1 : public Parser::BaseSynthesizeItem
{
public:
    virtual void operator()()
    {
        std::cout << "ACTION1" << std::endl;
    }
};

class Action2 : public Parser::BaseSynthesizeItem
{
public:
    virtual void operator()()
    {
        std::cout << "ACTION2" << std::endl;
    }
};


}
