#ifndef IDLISTACTIONS_HPP
#define IDLISTACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/Module.h"

namespace PascalCompiler {

// {IdList,{ID,IdList1}, TreeConstructor},
class FinalAppendList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::vector<std::string> id_list;
        std::vector<std::string> id_rest = cast_item<std::vector<std::string>>(m_stack[2], "idlist");
        std::string new_id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
        id_list.push_back(new_id);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        (*m_value)["idlist"] = id_list;
    }
};
// {IdList1,{SRCA,ID,IdList1}, TreeConstructor},
class AppendIdList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::vector<std::string> id_list;
        std::vector<std::string> id_rest = cast_item<std::vector<std::string>>(m_stack[3], "idlist");
        std::string new_id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[2]));
        id_list.push_back(new_id);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        (*m_value)["idlist"] = id_list;
    }
};
// {IdList1,{EPSILON}, TreeConstructor},
class InitIdList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["idlist"] = std::vector<std::string>();
    }
};

}
#endif /* IDLISTACTIONS_HPP  */
