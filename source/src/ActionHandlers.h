#include "Parser/SyntaxAnalyzer.h"

namespace PascalParser
{

class Action1 : public Parser::ActionItem
{
public:
    virtual void operator()(/*Parser::stack<stack_item_pointer>& stack*/)
    {
        std::cout << "ACTION1" << std::endl;
    }
};

class Action2 : public Parser::ActionItem
{
public:
    virtual void operator()(/*Parser::stack<stack_item_pointer>& stack*/)
    {
        std::cout << "ACTION2" << std::endl;
    }
};


}
