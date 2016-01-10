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

    void initializePrintf()
    {
        llvm::IRBuilder<> builder(m_currentContext->getAllocaBlock());
        intFormatString = builder.CreateGlobalStringPtr("%d");
        nFormatString = builder.CreateGlobalStringPtr("\n");
        strFormatString = builder.CreateGlobalStringPtr("%s");

        std::vector<llvm::Type *> putsArgs;
        putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
        //putsArgs.push_back(builder.getInt32Ty());
        llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

        llvm::FunctionType *putsType = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), argsRef, true);
        printf = m_llvmModule.getOrInsertFunction("printf", putsType);

        scanf = m_llvmModule.getOrInsertFunction("scanf", putsType);
    }

    void addPrintfIntCall(llvm::BasicBlock* block, llvm::Value* value)
    {
        llvm::IRBuilder<> builder(block);
        std::vector<llvm::Value*> args;
        args.push_back(intFormatString);
        args.push_back(value);
        builder.CreateCall(printf, llvm::ArrayRef<llvm::Value*>(args));
    }

    void addScanfIntCall(llvm::BasicBlock* block, llvm::Value* value)
    {
        llvm::IRBuilder<> builder(block);
        std::vector<llvm::Value*> args;
        args.push_back(intFormatString);
        args.push_back(value);
        builder.CreateCall(scanf, llvm::ArrayRef<llvm::Value*>(args));
    }

    void addPrintfENDL(llvm::BasicBlock* block)
    {
        llvm::IRBuilder<> builder(block);
        std::vector<llvm::Value*> args;
        args.push_back(nFormatString);
        builder.CreateCall(printf, llvm::ArrayRef<llvm::Value*>(args));
    }

    void addPrintfString(llvm::BasicBlock* block, llvm::Value* str)
    {
        llvm::IRBuilder<> builder(block);
        std::vector<llvm::Value*> args;
        args.push_back(strFormatString);
        args.push_back(str);
        builder.CreateCall(printf, llvm::ArrayRef<llvm::Value*>(args));
    }

    llvm::Value* getCharStringPtr(const std::string& str)
    {
        llvm::IRBuilder<> builder(m_currentContext->getAllocaBlock());
        return builder.CreateGlobalStringPtr(str.c_str());
    }

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

    //
    llvm::Constant* scanf = nullptr;
    llvm::Constant* printf = nullptr;
    llvm::Value* intFormatString = nullptr;
    llvm::Value* nFormatString = nullptr;
    llvm::Value* strFormatString = nullptr;
};

}

#endif // MODULE_H
