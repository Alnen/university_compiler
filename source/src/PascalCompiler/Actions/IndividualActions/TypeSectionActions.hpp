#ifndef TYPESECTIONACTIONS_HPP
#define TYPESECTIONACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/Module.h"

#include <iostream>

namespace PascalCompiler
{

// TYPE System
   // {TypeSection,{RWTP,TypeDefinition,SRSM,TypeSection1}}, DEF
   // {TypeSection1,{EPSILON}}, DEF


   // {TypeSection1,{TypeDefinition,SRSM,TypeSection1}}, DEF
   // {TypeDefinition,{ID,OPEQ,Type}}, OK
class TypeRegistrationAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();

        if (isTypeInfoPtr((*(m_stack[3]))["type"]))
        {
            BasicTypeInfo* actual_type = cast_item<BasicTypeInfo*>(m_stack[3], "type");
            std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
            std::unique_ptr<CustomTypeInfo> type = std::make_unique<CustomTypeInfo>(actual_type, id);
            getGlobalModule()->registerCustomType(id, std::move(type));
        }
        else
        {
           throw std::runtime_error("TypeRegistrationAction(): Type is nullptr");
        }

    }
protected:
};
   // {Type,{ConcreteType}}, OK
   // {Type,{Massive}}, OK
   // {Type,{EnumType}}, OK
class TypePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["type"] = cast_item<BasicTypeInfo*>(m_stack[1], "type");
    }
protected:
};
   // {Type,{ID}}, OK
class GetTypeAccordingToID : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
        BasicTypeInfo* type = getGlobalModule()->getTypeInfo(id);
        (*m_value)["type"] = type;
    }
};
   // {ConcreteType,{RWINT}}, OK
class IntegerPass : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["type"] = getGlobalModule()->getTypeInfo("integer");
    }
protected:
};
   // {ConcreteType,{RWRAT}}, OK
class RationalPass : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["type"] = getGlobalModule()->getTypeInfo("rational");
    }
protected:
};
   // {ConcreteType,{RWCX}}, OK
class ComplexPass : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["type"] = getGlobalModule()->getTypeInfo("complex");
    }
protected:
};
   // {Massive,{RWAR,SRLB,IndexType,Massive1,SRRB,RWOF,FinalType}}, OK
class ArrayCreationAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();

        // Merge lists
        // Cast rest of indexes
        std::vector<std::pair<size_t, BasicTypeInfo*>>& rest = boost::any_cast<
                std::vector<std::pair<size_t, BasicTypeInfo*>>&
                >(
                    (*(m_stack[4]))["massive"]
                );
        std::vector<std::pair<size_t, BasicTypeInfo*>> value;
        if (isInt((*(m_stack[3]))["size"]))
        {
            value.emplace_back(cast_item<int>(m_stack[3], "size"), getGlobalModule()->getTypeInfo("integer"));
        }
        else
        {
            throw std::runtime_error("NOT SUPPORTED");
        }
        std::copy(rest.begin(), rest.end(), std::back_inserter(value));
        //
        if (isTypeInfoPtr((*(m_stack[7]))["type"]))
        {
            BasicTypeInfo* element_typeinfo = cast_item<BasicTypeInfo*>(m_stack[7], "type");
            std::unique_ptr<ArrayTypeInfo> type = std::make_unique<ArrayTypeInfo>(value, element_typeinfo);
            BasicTypeInfo* savedType = type.get();
            getGlobalModule()->registerCustomType(getGlobalModule()->getAnonimousName(), std::move(type));
            (*m_value)["type"] = savedType;
        }
        else
        {
            throw std::runtime_error("Unexpected error in type subsystem!");
        }
    }
protected:
};
   // {Massive1,{EPSILON}}, OK
class Massive1InitVec : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::cout << "BOOM_INIT" << std::endl;
        (*m_value)["massive"] = std::vector<std::pair<size_t, BasicTypeInfo*>>();
    }
protected:
};
   // {Massive1,{SRCA,IndexType,Massive1}}, OK
class Massive1AppendAtStart : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        std::cout << "BOOM_CONCAT" << std::endl;
        TreeConstructor::executeHandler();
        Module* module = getGlobalModule();

        // Cast rest of indexes
        std::vector<std::pair<size_t, BasicTypeInfo*>>& rest = boost::any_cast<
                std::vector<std::pair<size_t, BasicTypeInfo*>>&
                >(
                    (*(m_stack[3]))["massive"]
                );
        std::vector<std::pair<size_t, BasicTypeInfo*>> value;
        if (isInt((*(m_stack[2]))["size"]))
        {
            value.emplace_back(cast_item<int>(m_stack[2], "size"), module->getTypeInfo("integer"));
        }
        else
        {
            throw std::runtime_error("NOT SUPPORTED");
        }
        std::copy(rest.begin(), rest.end(), std::back_inserter(value));
        (*m_value)["massive"] = value;
    }
};
   // {IndexType,{CX}}, OK
class IndexTypeCXSize : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::cout << "BOOM" << std::endl;
        (*m_value)["size"] = std::atoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1])).c_str());
    }
protected:
};
   // {IndexType,{ID}}, TODO
class IndexTypeIDCase : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        // CONST
        // ENUMERATION NOUT SUPPORTED
        (*m_value)["id"] = TOKEN_VALUE(m_stack[1]);
    }
protected:
};
   // {EnumType,{SRLP,IdList,SRRP}}, OK
class EnumTypeConstruction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        std::vector<std::string> id_list = cast_item<std::vector<std::string>>(m_stack[2], "idlist");
        std::unique_ptr<EnumerationTypeInfo> type = std::make_unique<EnumerationTypeInfo>();

        for (const auto& id : id_list)
        {
            type->addItem(id);
        }
        BasicTypeInfo* savedType = type.get();
        getGlobalModule()->registerCustomType(getGlobalModule()->getAnonimousName(), std::move(type));
        (*m_value)["type"] = savedType;
    }
protected:
};

}

#endif /* TYPESECTIONACTIONS_HPP */
