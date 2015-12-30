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

    llvm::Value* addDNoperator(llvm::BasicBlock* block, llvm::Value* L, bool isNumenator, bool isStore);

    std::pair<llvm::Value*, BasicTypeInfo*>
    addSubscription(llvm::Value* L, BasicTypeInfo* typeInfo, const std::vector<size_t>& offsets);

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
