#ifndef LABELSECTIONACTIONS_HPP
#define LABELSECTIONACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/LabelInfo.hpp"
#include "../../IR/Module.h"

namespace PascalCompiler {

// {LabelSection,{RWLB,Label,LabelSection1,SRSM}, TreeConstructor},
// {LabelSection1,{EPSILON}, TreeConstructor},
// {LabelSection1,{SRCA,Label,LabelSection1}, TreeConstructor},
// {Label,{ID}, TreeConstructor},
class LabelRegistration : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
        getGlobalModule()->getCurrentContext()->registerBasicBlock(id);
    }
};

}

#endif /* LABELSECTIONACTIONS_HPP */
