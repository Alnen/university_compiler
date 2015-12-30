#include "Module.h"
#include "StdFix.h"
#include "BasicTypeInfo.hpp"
#include <iostream>

namespace PascalCompiler {

Module::Module(std::string name):
    m_name(name),
    m_llvmModule(m_name, llvm::getGlobalContext())
{
    // Integer tpye
    llvm::IntegerType*  integer_type  = llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext());

    // Populate default type system
    m_typeMap["integer"] = std::make_unique<BasicTypeInfo>(BasicTypeInfo::INTEGER);
    m_typeMap["rational"] = std::make_unique<BasicTypeInfo>(BasicTypeInfo::RATIONAL);
    m_typeMap["complex"] = std::make_unique<BasicTypeInfo>(BasicTypeInfo::COMPLEX);

    llvm::Type* rational_type = m_typeMap["rational"]->getLLVMType();
    llvm::Type* complex_type = m_typeMap["complex"]->getLLVMType();

    // Base operations
    m_binaryBaseOperators["integer"] = [this](llvm::BasicBlock* block, llvm::Value* L, llvm::Value* R,int OP)
    {
        llvm::IRBuilder<> irBuilder(block);
        llvm::Value* result_codegen = nullptr;

        switch(OP)
        {
        case PascalCompiler::Grammar::RWLO: // ||
            result_codegen = irBuilder.CreateAnd(L, R, "tempand");
            break;
        case PascalCompiler::Grammar::RWLA: // &&
            result_codegen = irBuilder.CreateOr(L, R, "tempor");
            break;

        case PascalCompiler::Grammar::OPGT: // >
            result_codegen = irBuilder.CreateICmpSGT(L, R, "tempgt");
            break;
        case PascalCompiler::Grammar::OPLT: // <
            result_codegen = irBuilder.CreateICmpSLT(L, R, "templt");
            break;
        case PascalCompiler::Grammar::OPGE: // >=
            result_codegen = irBuilder.CreateICmpSGE(L, R, "tempge");
            break;
        case PascalCompiler::Grammar::OPLE: // <=
            result_codegen = irBuilder.CreateICmpSLE(L, R, "temple");
            break;
        case PascalCompiler::Grammar::OPEQ: // ==
            result_codegen = irBuilder.CreateICmpEQ(L, R, "tempeq");
            break;
        case PascalCompiler::Grammar::OPNE: // !=
            result_codegen = irBuilder.CreateICmpNE(L, R, "tempne");
            break;

        case PascalCompiler::Grammar::OPPLUS: // +
            result_codegen = irBuilder.CreateAdd(L, R, "tempadd");
            break;
        case PascalCompiler::Grammar::OPMINUS: // -
            result_codegen = irBuilder.CreateSub(L, R, "tempsub");
            break;

        case PascalCompiler::Grammar::OPMUL: // *
            result_codegen = irBuilder.CreateAdd(L, R, "tempmul");
            break;
        case PascalCompiler::Grammar::OPDIV: // /
            result_codegen = irBuilder.CreateSub(L, R, "tempdiv");
            break;

        case PascalCompiler::Grammar::OPAS: // :=
            // L is Alloca, R is Value
        {
            llvm::Value* lPtr = irBuilder.CreateGEP(L, {llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0)});
            result_codegen = irBuilder.CreateStore(R, lPtr, "tempas");
            break;
        }

        case PascalCompiler::Grammar::RWCM: // common
            throw std::runtime_error("common for integer unsupported ");
            break;

        default:
            throw std::runtime_error("unsupported operation ");
        }

        return result_codegen;
    };

    m_binaryBaseOperators["rational"] = [this, integer_type, rational_type](llvm::BasicBlock* block, llvm::Value* L, llvm::Value* R,int OP)
    {
        llvm::IRBuilder<> irBuilder(block);
        llvm::Value* result_codegen = nullptr;

        llvm::ArrayRef<llvm::Value*> first_index = {llvm::ConstantInt::get(integer_type, 0), llvm::ConstantInt::get(integer_type, 0)};
        llvm::ArrayRef<llvm::Value*> second_index = {llvm::ConstantInt::get(integer_type, 0), llvm::ConstantInt::get(integer_type, 1)};

        llvm::Value* pointer_to_L_0 = irBuilder.CreateGEP(L, first_index);
        llvm::Value* pointer_to_L_1 = irBuilder.CreateGEP(L, second_index);
        llvm::Value* pointer_to_L_0_value = irBuilder.CreateLoad(pointer_to_L_0);
        llvm::Value* pointer_to_L_1_value = irBuilder.CreateLoad(pointer_to_L_1);

        llvm::Value* pointer_to_R_0 = irBuilder.CreateGEP(R, first_index);
        llvm::Value* pointer_to_R_1 = irBuilder.CreateGEP(R, second_index);
        llvm::Value* pointer_to_R_0_value = irBuilder.CreateLoad(pointer_to_R_0);
        llvm::Value* pointer_to_R_1_value = irBuilder.CreateLoad(pointer_to_R_1);


        llvm::Value* allocaToTemp = irBuilder.CreateAlloca(rational_type);
        llvm::Value* pointer_to_TEMP_0 = irBuilder.CreateGEP(allocaToTemp, first_index);
        llvm::Value* pointer_to_TEMP_1 = irBuilder.CreateGEP(allocaToTemp, second_index);

        llvm::Value* temp_op_ret_0 = nullptr;
        llvm::Value* temp_op_ret_1 = nullptr;
        switch(OP)
        {
        case PascalCompiler::Grammar::RWLO: // ||
            temp_op_ret_0 = irBuilder.CreateAnd(pointer_to_L_0_value, pointer_to_R_0_value, "tempand");
            temp_op_ret_1 = irBuilder.CreateAnd(pointer_to_L_1_value, pointer_to_R_1_value, "tempand");
            break;

        case PascalCompiler::Grammar::RWLA: // &&
            temp_op_ret_0 = irBuilder.CreateOr(pointer_to_L_0_value, pointer_to_R_0_value, "tempor");
            temp_op_ret_1 = irBuilder.CreateOr(pointer_to_L_0_value, pointer_to_R_1_value, "tempor");
            break;

        case PascalCompiler::Grammar::OPGT: // >
            temp_op_ret_0 = irBuilder.CreateICmpSGT(pointer_to_L_0_value, pointer_to_R_0_value, "tempgt");
            temp_op_ret_1 = irBuilder.CreateICmpSGT(pointer_to_L_1_value, pointer_to_R_1_value, "tempgt");
            break;

        case PascalCompiler::Grammar::OPLT: // <
            temp_op_ret_0 = irBuilder.CreateICmpSLT(pointer_to_L_0_value, pointer_to_R_0_value, "templt");
            temp_op_ret_1 = irBuilder.CreateICmpSLT(pointer_to_L_1_value, pointer_to_R_1_value, "templt");
            break;

        case PascalCompiler::Grammar::OPGE: // >=
            temp_op_ret_0 = irBuilder.CreateICmpSGE(pointer_to_L_0_value, pointer_to_R_0_value, "tempge");
            temp_op_ret_1 = irBuilder.CreateICmpSGE(pointer_to_L_1_value, pointer_to_R_1_value, "tempge");
            break;

        case PascalCompiler::Grammar::OPLE: // <=
            temp_op_ret_0 = irBuilder.CreateICmpSLE(pointer_to_L_0_value, pointer_to_R_0_value, "temple");
            temp_op_ret_1 = irBuilder.CreateICmpSLE(pointer_to_L_1_value, pointer_to_R_1_value, "temple");
            break;

        case PascalCompiler::Grammar::OPEQ: // ==
            temp_op_ret_0 = irBuilder.CreateICmpEQ(pointer_to_L_0_value, pointer_to_R_0_value, "tempeq");
            temp_op_ret_1 = irBuilder.CreateICmpEQ(pointer_to_L_1_value, pointer_to_R_1_value, "tempeq");
            break;
        case PascalCompiler::Grammar::OPNE: // !=
            temp_op_ret_0 = irBuilder.CreateICmpNE(pointer_to_L_0_value, pointer_to_R_0_value, "tempne");
            temp_op_ret_1 = irBuilder.CreateICmpNE(pointer_to_L_1_value, pointer_to_R_1_value, "tempne");
            break;

        case PascalCompiler::Grammar::OPPLUS: // +
            temp_op_ret_0 = irBuilder.CreateAdd(pointer_to_L_0_value, pointer_to_R_0_value, "tempadd");
            temp_op_ret_1 = irBuilder.CreateAdd(pointer_to_L_1_value, pointer_to_R_1_value, "tempadd");
            break;
        case PascalCompiler::Grammar::OPMINUS: // -
            temp_op_ret_0 = irBuilder.CreateSub(pointer_to_L_0_value, pointer_to_R_0_value, "tempsub");
            temp_op_ret_1 = irBuilder.CreateSub(pointer_to_L_1_value, pointer_to_R_1_value, "tempsub");
            break;

        case PascalCompiler::Grammar::RWCM: // common
            throw std::runtime_error("common for integer unsupported " + std::to_string(__LINE__) + " " + std::string(__FILE__));
            break;

        default:
            throw std::runtime_error("unsupported operation " + std::to_string(__LINE__) + " " + std::string(__FILE__));
        }

        irBuilder.CreateStore(temp_op_ret_0, pointer_to_TEMP_0);
        irBuilder.CreateStore(temp_op_ret_1, pointer_to_TEMP_1);

        result_codegen = allocaToTemp;
        return result_codegen;
    };

    m_binaryBaseOperators["complex"] = [this, integer_type, complex_type](llvm::BasicBlock* block, llvm::Value* L, llvm::Value* R,int OP)
    {
        llvm::IRBuilder<> irBuilder(block);
        llvm::Value* result_codegen = nullptr;

        llvm::ArrayRef<llvm::Value*> first_index = {llvm::ConstantInt::get(integer_type, 0), llvm::ConstantInt::get(integer_type, 0)};
        llvm::ArrayRef<llvm::Value*> second_index = {llvm::ConstantInt::get(integer_type, 0), llvm::ConstantInt::get(integer_type, 1)};

        llvm::Value* pointer_to_L_0 = irBuilder.CreateGEP(L, first_index);
        llvm::Value* pointer_to_L_1 = irBuilder.CreateGEP(L, second_index);

        llvm::Value* pointer_to_R_0 = irBuilder.CreateGEP(R, first_index);
        llvm::Value* pointer_to_R_1 = irBuilder.CreateGEP(R, second_index);


        llvm::Value* allocaToTemp = irBuilder.CreateAlloca(complex_type);
        llvm::Value* pointer_to_TEMP_0 = irBuilder.CreateGEP(L, first_index);
        llvm::Value* pointer_to_TEMP_1 = irBuilder.CreateGEP(L, second_index);

        llvm::Value* temp_op_ret_0 = nullptr;
        llvm::Value* temp_op_ret_1 = nullptr;

        temp_op_ret_0 = m_binaryBaseOperators["rational"](block, pointer_to_L_0, pointer_to_R_0, OP);
        temp_op_ret_1 = m_binaryBaseOperators["rational"](block, pointer_to_L_1, pointer_to_R_1, OP);

        irBuilder.CreateStore(temp_op_ret_0, pointer_to_TEMP_0);
        irBuilder.CreateStore(temp_op_ret_1, pointer_to_TEMP_1);

        result_codegen = allocaToTemp;
        return result_codegen;
    };

    // Unary operations
    m_unaryBaseOperators["integer"] = [this, integer_type](llvm::BasicBlock* block, llvm::Value* L,int OP)
    {
        llvm::IRBuilder<> irBuilder(block);
        llvm::Value* result_codegen = nullptr;

        switch(OP)
        {
        case PascalCompiler::Grammar::RWFR: // frac
            throw std::runtime_error("frac operation in son supported by integer type");
            break;
        case PascalCompiler::Grammar::RWINTOP: // int
            result_codegen = L;
            break;
        case PascalCompiler::Grammar::RWSM: // simplify
            throw std::runtime_error("simplify operation in son supported by integer type");
            break;
        case PascalCompiler::Grammar::RWIR: // irregular
            throw std::runtime_error("irregular operation in son supported by integer type");
            break;
        case PascalCompiler::Grammar::RWLN: // not
            result_codegen = irBuilder.CreateNot(L, "tempnot");
            break;
        case PascalCompiler::Grammar::RWRE: // re
            throw std::runtime_error("re operation in son supported by integer type");
            break;
        case PascalCompiler::Grammar::RWIM: // im
            throw std::runtime_error("im operation in son supported by integer type");
            break;

        case PascalCompiler::Grammar::OPPLUS: // +
            result_codegen = L;
            break;
        case PascalCompiler::Grammar::OPMINUS: // -
        {
            llvm::Value* zero_const = llvm::ConstantInt::get(integer_type, 0);
            result_codegen = irBuilder.CreateSub(zero_const, L, "tempsub");
            break;
        }

        default:
            throw std::runtime_error("unsupported operation ");
        }

        return result_codegen;
    };

}

llvm::Value* Module::addLoadOperation(llvm::BasicBlock* block, const std::string& name)
{
    VarInfo* nameID = m_currentContext->getVariable(name);
    llvm::IRBuilder<> irBuilder(block);
    llvm::Value* ptr = irBuilder.CreateGEP(nameID->getAllocaInst(), {llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0)});
    llvm::Value* value = irBuilder.CreateLoad(ptr);
    return value;
}

llvm::Value* Module::addStoreOperation(llvm::BasicBlock* block, const std::string& name, llvm::Value* value)
{
    VarInfo* nameID = m_currentContext->getVariable(name);
    llvm::IRBuilder<> irBuilder(block);
    llvm::Value* ptr = irBuilder.CreateGEP(nameID->getAllocaInst(),{llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0)});
    llvm::Value* return_value = irBuilder.CreateStore(value, ptr);
    return return_value;
}

llvm::Module* Module::getModule()
{
    return &m_llvmModule;
}

std::pair<llvm::Value*, BasicTypeInfo*>
Module::addBinaryOperation(llvm::BasicBlock* block,
                   llvm::Value* L, BasicTypeInfo* type_a,
                   llvm::Value* R, BasicTypeInfo* type_b,
                   int OP)
{
    llvm::Value* result_value = nullptr;
    BasicTypeInfo* result_type = nullptr;

    do
    {
        if(type_a->getType() == BasicTypeInfo::ARRAY || type_b->getType() == BasicTypeInfo::ARRAY)
        {
            // one of them is array its kinda bad
            throw std::runtime_error("Cannot perform binary operation on array");
        }

        if (type_a->getType() == BasicTypeInfo::CUSTOM || type_b->getType() == BasicTypeInfo::CUSTOM)
        {
            // atleast one of them is custom
            // need overload or we screwed
            // if exist do it
            // if not exist then throw
            break;
        }

        if (type_a->getType() == BasicTypeInfo::ENUMERATION && type_b->getType() == BasicTypeInfo::ENUMERATION)
        {
            // enumeartion support only equality comparison and second
            if(OP == Grammar::OPAS)
            {

                //TODO call int thing
            }
            else if(OP == Grammar::OPEQ)
            {
                //TODO call int thing
            }
            else if(OP == Grammar::OPNE)
            {
                //TODO call int thing
            }
            else
            {
                throw std::runtime_error("Enumeration support only assignment and equality compariosons");
            }
            break;
        }

        if((type_a->getType() == BasicTypeInfo::INTEGER || type_a->getType() == BasicTypeInfo::RATIONAL || type_a->getType() == BasicTypeInfo::COMPLEX) &&
           (type_b->getType() == BasicTypeInfo::INTEGER || type_b->getType() == BasicTypeInfo::RATIONAL || type_b->getType() == BasicTypeInfo::COMPLEX))
        {
            // TODO
            // if both types simple
            // cast to highest
            // perform operations

            if (type_a->getType() == BasicTypeInfo::INTEGER && type_b->getType() == BasicTypeInfo::INTEGER)
            {
                result_type = m_typeMap["integer"].get();
                result_value = m_binaryBaseOperators["integer"](block, L, R, OP);
            }
            else
            {
                throw std::runtime_error("Only int supported");
            }
            break;
        }

        // if came to here then some wierd stuff is going on around
        throw std::runtime_error("addBinaryOperation error unknown type_a is "
                                 + std::to_string(type_a->getType())
                                 + " and type_b is "
                                 + std::to_string(type_b->getType()));
    }
    while(false);

    return {result_value, result_type};
}

std::pair<llvm::Value*, BasicTypeInfo*>
Module::addUnaryOperation(llvm::BasicBlock* block,
                  llvm::Value* L, BasicTypeInfo* type_a,
                  int OP)
{
    llvm::Value* result_value = nullptr;
    BasicTypeInfo* result_type = nullptr;

    if (type_a->getType() == BasicTypeInfo::INTEGER)
    {
        result_type = type_a;
        result_value = m_unaryBaseOperators["integer"](block, L, OP);
    }
    else
    {
        throw std::runtime_error("Unary operations supported only for integers");
    }

    return {result_value, result_type};
}

llvm::Value* Module::addDNoperator(llvm::BasicBlock* block, llvm::Value* L, bool isNumenator, bool isStore)
{
    if (isNumenator)
    {
        if (isStore)
        {

        }
        else
        {

        }
    }
    else
    {
        if (isStore)
        {

        }
        else
        {

        }
    }
}

std::pair<llvm::Value*, BasicTypeInfo*>
Module::addSubscription(llvm::Value* L, BasicTypeInfo* type, const std::vector<size_t>& offsets)
{
    llvm::Value* return_value = nullptr;
    BasicTypeInfo* return_type =  nullptr;

    size_t currentItem = 0;

    throw std::runtime_error("NOT IMPLEMENTED SUBSCRIPTION");

    for (size_t currentIndex = 0; currentIndex < offsets.size(); ++currentIndex)
    {

    }

    return {return_value, return_type};
}

BasicTypeInfo* Module::registerCustomType(const std::string& name, std::unique_ptr<BasicTypeInfo> type_info)
{
    if (m_typeMap.find(name) == m_typeMap.end())
    {
        BasicTypeInfo* temp = type_info.get();
        m_typeMap[name] = std::move(type_info);
        return temp;
    }
    else
    {
        throw std::runtime_error("Type with name " + name + " has been already declared previously.");
    }
}

BasicTypeInfo* Module::getTypeInfo(const std::string& type_name)
{
    auto pos = m_typeMap.find(type_name);
    for (const auto& pair: m_typeMap)
    {
        std::cout << "F" << pair.first << std::endl;
    }
    if (pos != m_typeMap.end())
    {
        return (*pos).second.get();
    }
    else
    {
        return nullptr;
    }
}

std::string Module::getAnonimousName() const
{
    static int id = 0;
    return "$anonimous" + std::to_string(++id);
}

Context* Module::registerContext(const std::string& name, llvm::FunctionType* functionType, Context* parentContext)
{
    if (m_functionMap.find(name) != m_functionMap.end())
    {
        throw std::runtime_error("Context with that name already exsists");
    }
    std::unique_ptr<Context> newContext = std::make_unique<Context>(name, functionType, parentContext);
    Context* newCotextPtr = newContext.get();
    m_functionMap[name] = std::move(newContext);
    return newCotextPtr;
}

void Module::setContext(Context* context)
{
    m_currentContext = context;
}

Context* Module::getCurrentContext() const
{
    return m_currentContext;
}

Context* Module::getContextByName(const std::string& name) const
{
    decltype(m_functionMap)::const_iterator pos;
    if ((pos = m_functionMap.find(name)) != m_functionMap.end())
    {
        return pos->second.get();
    }
    else
    {
        throw std::runtime_error("No such function or overload");
    }
}

}