#ifndef MODULE_H
#define MODULE_H

#include <functional>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include "boost/container/flat_map.hpp"
#include "boost/any.hpp"

#include "../Grammar/Terminals.h"
#include <algorithm>
#include <iterator>

//
#include "BasicTypeInfo.hpp"
#include "ArrayTypeInfo.hpp"
#include "EnumerationTypeInfo.hpp"
#include "CustomTypeInfo.hpp"

#include "VarInfo.hpp"
#include "Context.hpp"
#include "LabelInfo.hpp"

namespace PascalCompiler {

class VarModificator
{
public:
    enum Type
    {
        NOT_INITIALIZED,
        MATRIX_INDEX,
        MEMBER_FETCH
    };

    Type getType() const
    {
        return m_type;
    }

    std::string getName() const
    {
        return boost::any_cast<std::string>(m_value);
    }

    void setName(const std::string& name)
    {
        m_type = MEMBER_FETCH;
        m_value = name;
    }

    std::pair<llvm::Value*, BasicTypeInfo*> getIndexValue() const
    {
        return boost::any_cast<std::pair<llvm::Value*, BasicTypeInfo*>>(m_value);
    }

    void setIndexValue(const std::pair<llvm::Value*, BasicTypeInfo*>& indexValue)
    {
        m_type = MATRIX_INDEX;
        m_value = indexValue;
    }

private:
    Type m_type = NOT_INITIALIZED;
    boost::any m_value;
};


class Module
{
public:
    Module(std::string name);

    llvm::Module* getModule();

    std::pair<llvm::Value*, BasicTypeInfo*>
    addBinaryOperation(llvm::BasicBlock* block,
                       llvm::Value* L, BasicTypeInfo* type_a,
                       llvm::Value* R, BasicTypeInfo* type_b,
                       int OP);

    std::pair<llvm::Value*, BasicTypeInfo*>
    addUnaryOperation(llvm::BasicBlock* block,
                      llvm::Value* L, BasicTypeInfo* type_a,
                      int OP);

    std::pair<llvm::Value*, BasicTypeInfo*> addLoadOperation(llvm::BasicBlock* block,
                                                             llvm::Value* ptr, BasicTypeInfo* ptrType);
    std::pair<llvm::Value*, BasicTypeInfo*> addStoreOperation(llvm::BasicBlock* block,
                                                              llvm::Value* ptr, BasicTypeInfo* ptrType,
                                                              llvm::Value* value, BasicTypeInfo* valueTyp);

    std::pair<llvm::Value*, BasicTypeInfo*>
    addSubscription(llvm::BasicBlock* block, llvm::Value* L, BasicTypeInfo* typeInfo, const std::vector<VarModificator>& modificators);

    BasicTypeInfo* registerCustomType(const std::string& name, std::unique_ptr<BasicTypeInfo> type_info);

    BasicTypeInfo* getTypeInfo(const std::string& type_name);

    std::string getAnonimousName() const;

    Context* registerContext(const std::string& name, llvm::FunctionType* functionType, Context* parentContext);

    void setContext(Context* context);
    Context* getCurrentContext() const;

    Context* getContextByName(const std::string& name) const;

    ~Module()
    {
        m_llvmModule.dump();
    }

protected:
    std::string         m_name;
    llvm::Module        m_llvmModule;

    // Tables
    Context* m_currentContext = nullptr;
    boost::container::flat_map<std::string, std::unique_ptr<Context>>   m_functionMap;
    boost::container::flat_map<std::string, std::unique_ptr<BasicTypeInfo>>  m_typeMap;
    boost::container::flat_map<std::string, std::function<llvm::Value*(llvm::BasicBlock*, llvm::Value*, llvm::Value*, int)>>   m_binaryBaseOperators;
    boost::container::flat_map<std::string, std::function<llvm::Value*(llvm::BasicBlock*, llvm::Value*, int)>>                 m_unaryBaseOperators;
};

}

#endif // MODULE_H
