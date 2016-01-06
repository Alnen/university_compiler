#ifndef LABELSECTIONACTIONS_HPP
#define LABELSECTIONACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/LabelInfo.hpp"
#include "../../IR/Module.h"

namespace PascalCompiler {

// {LabelSection,{RWLB,Label,LabelSection1,SRSM}, TreeConstructor},
// {LabelSection1,{EPSILON}, TreeConstructor},
// {LabelSection1,{SRCA,Label,LabelSection1}, TreeConstructor},
class LabelRegistration : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::string id = cast_item<std::string>(m_stack[2], "id");
        getGlobalModule()->getCurrentContext()->registerBasicBlock(id);
    }
};
// {Label,{ID}, TreeConstructor},
class LabelID : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["id"] = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
    }
};

}

#endif /* LABELSECTIONACTIONS_HPP */
