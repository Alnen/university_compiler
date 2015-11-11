#include "Parser/SyntaxAnalyzer.h"

namespace PascalParser
{

class Action1 : public Parser::BaseSynthesizeItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "SYNTHESIZE program name is " << boost::any_cast<std::string>(*m_stack[2]) << std::endl;
    }
};

class Action2 : public Parser::BaseSynthesizeItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "ACTION2" << std::endl;
    }
};

class Action3 : public Parser::BaseInheritItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "INHERITED program name is " << boost::any_cast<std::string>(*m_stack[2]) << std::endl;
    }
};


}
