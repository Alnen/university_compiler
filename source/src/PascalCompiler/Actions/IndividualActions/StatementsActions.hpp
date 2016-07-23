#ifndef STATEMENTSACTIONS_HPP
#define STATEMENTSACTIONS_HPP

#include "BaseHandlers.h"
#include "../../IR/GlobalContext.h"
#include "../../IR/Context.hpp"
#include "../../IR/Module.h"
#include <iostream>

namespace PascalCompiler {

// {CreateBlock, {EPSILON}, CreateBlockAction},
class CreateBlockAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::cout << "new BlockPack" << std::endl;
        getGlobalModule()->getCurrentContext()->pushBlockPack();
    }
};

//  {CompoundStatement,{RWB,Statement,CompoundStatement1}, TreeConstructor},
//  {CompoundStatement1,{SRSM,CompoundStatement2}, TreeConstructor},
//  {CompoundStatement1,{RWEND}, TreeConstructor},
//  {CompoundStatement2,{RWEND}, TreeConstructor},
//  {CompoundStatement2,{SRSM,CompoundStatement2}, TreeConstructor},
//  {CompoundStatement2,{Statement2,CompoundStatement1}, TreeConstructor},
//  Default

//  {Statement,{EPSILON}, TreeConstructor},
//  {Statement,{Statement2}, TreeConstructor},
//  {Statement2,{ConditionalOperator}, TreeConstructor},
//  {Statement2,{CompoundStatement}, TreeConstructor},
//  {Statement2,{InputOperator}, TreeConstructor},
//  {Statement2,{OutputOperator}, TreeConstructor},
//  {Statement2,{UnconditionalJumpOperator}, TreeConstructor},
//  {Statement2,{LoopWithParameterOperator}, TreeConstructor}
//  Default

//  {Statement2,{ID,Statement1}, TreeConstructor},
class LabelOrAssignmentAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        TreeConstructor::executeHandler();
        std::cout << "LABLEOOM" << std::endl;
        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack* currentBlockPack = currentContext->getCurrentBlock();

        std::cout << "LABLEOOM" << std::endl;
        bool isLabel = cast_item<bool>(m_stack[2], "label");
        std::cout << "LABLEOOM" << std::endl;
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));

        if (isLabel)
        {
            /*llvm::BasicBlock* currentFinalBlock = currentBlockPack->getFinalBlock();
            llvm::BasicBlock* labelBlock = currentContext->getBasicBlock(id);
            llvm::IRBuilder<> builder(currentFinalBlock);
            builder.CreateBr(labelBlock);
            currentBlockPack->setFinalBlock(labelBlock);*/
        }
        else
        {
            std::vector<VarModificator> modificators = cast_item<std::vector<VarModificator>>(m_stack[2], "VarHandlerList");
            std::pair<llvm::Value*, BasicTypeInfo*> valueToStore = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[2], "Value");
            VarInfo* idInfo = getGlobalModule()->getCurrentContext()->getVariable(id);
            llvm::BasicBlock* block = getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock();
            auto   ptrPair = getGlobalModule()->addSubscription(block,
                                                                idInfo->getAllocaInst(), idInfo->getType(),
                                                                modificators);
            (*m_value)["Value"] = getGlobalModule()->addStoreOperation(block,
                                                                       ptrPair.first, ptrPair.second,
                                                                       valueToStore.first, valueToStore.second);
            //throw std::runtime_error("Assignment not suported");
        }
    }
};

class LabelStatement : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        TreeConstructor::executeHandler();
        std::string id = boost::any_cast<std::string>((*(m_stack[0]))["id"]);
        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack* currentBlockPack = currentContext->getCurrentBlock();
        llvm::BasicBlock* currentFinalBlock = currentBlockPack->getFinalBlock();
        llvm::BasicBlock* labelBlock = currentContext->getBasicBlock(id);
        llvm::IRBuilder<> builder(currentFinalBlock);
        builder.CreateBr(labelBlock);
        currentBlockPack->setFinalBlock(labelBlock);
    }
};

class IDPass2 : public Parser::BaseInheritItem
{
    virtual void executeHandler() override
    {
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["id"] = boost::any_cast<std::string>((*(m_stack[0]))["id"]);
    }
};

class IDPass : public Parser::BaseInheritItem
{
    virtual void executeHandler() override
    {
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["id"] = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
    }
};

//  {Statement1,{SRCN,Statement}, TreeConstructor},
class LabelAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["label"] = true;
    }
};

// {Statement1,{LeftHandVar3,OPAS,Expression}, AssignmentAction},
class AssignmentAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["label"] = false;
        (*m_value)["VarHandlerList"] = cast_item<std::vector<VarModificator>>(m_stack[1], "VarHandlerList");
        (*m_value)["OP"]        = OPAS;
        (*m_value)["Value"]     = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[3], "Value");
    }
};

//  {UnconditionalJumpOperator,{RWGT,Label}, TreeConstructor},
class GotoLabel : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::string id = cast_item<std::string>(m_stack[2], "id");
        llvm::IRBuilder<> irBuilder(getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock());
        irBuilder.CreateBr(getGlobalModule()->getCurrentContext()->getBasicBlock(id));
        getGlobalModule()->getCurrentContext()->getCurrentBlock()->setFinalBlock(getGlobalModule()->getCurrentContext()->registerBasicBlock(getGlobalModule()->getAnonimousName()));
    }
};

//  {LoopWithParameterOperator,{RWFOR,ID,OPAS,Expression,LoopDirection,Expression,RWDO, CreateBlock, Statement}, TreeConstructor},
class LoopWithParameterAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        auto& context = getGlobalModule()->getLLVMModule().getContext();
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        TokenType direction = cast_item<TokenType>(m_stack[5],"direction");

        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack bodyBlockPack = *(currentContext->getCurrentBlock());
        currentContext->popBlockPack();
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;

        llvm::BasicBlock* previousBlock  = currentContext->getCurrentBlock()->getFinalBlock();
        llvm::BasicBlock* checkBlock     = currentContext->registerBasicBlock("check_block@" + getGlobalModule()->getAnonimousName());
        llvm::BasicBlock* afterBodyBlock = currentContext->registerBasicBlock("after_body@" + getGlobalModule()->getAnonimousName());

        llvm::IRBuilder<> irBuilder(previousBlock);
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        VarInfo*         counterInfo = currentContext->getVariable(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[2])));
        std::pair<llvm::Value*, BasicTypeInfo*> initValuePair = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[4], "Value");
        llvm::Value*     initValue = initValuePair.first;
        BasicTypeInfo*   initTypeInfo = initValuePair.second;
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        if (counterInfo->getType()->getType() != BasicTypeInfo::INTEGER) // TODO check second adgument
        {
            throw std::runtime_error("Only integer supported as counter in for statement");
        }
        // Add to previous block
        std::pair<llvm::Value*, BasicTypeInfo*> counterPtr = getGlobalModule()->addSubscription(
                    previousBlock,
                    counterInfo->getAllocaInst(), counterInfo->getType(),
                    {});
        getGlobalModule()->addStoreOperation(previousBlock,
                                             counterPtr.first, counterPtr.second,
                                             initValuePair.first, initValuePair.second
                                             );
        irBuilder.SetInsertPoint(previousBlock);
        irBuilder.CreateBr(checkBlock);
        // Construct create block
        std::pair<llvm::Value*, BasicTypeInfo*> endValuePair = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[6], "Value");
        llvm::Value*     endValue = endValuePair.first;
        BasicTypeInfo*   endTypeInfo = endValuePair.second;

        irBuilder.SetInsertPoint(checkBlock);
        std::pair<llvm::Value*, BasicTypeInfo*> counterPtr2 = getGlobalModule()->addSubscription(
                    checkBlock,
                    counterInfo->getAllocaInst(), counterInfo->getType(),
                    {});
        std::pair<llvm::Value*, BasicTypeInfo*> counterValueInCheckBlock = getGlobalModule()->addLoadOperation(checkBlock,
                                             counterPtr2.first, counterPtr2.second
                                             );
        std::pair<llvm::Value*, BasicTypeInfo*> testValue;
        if (direction == RWTO)
        {
                testValue = getGlobalModule()->addBinaryOperation(checkBlock,
                                                                  counterValueInCheckBlock.first, counterValueInCheckBlock.second,
                                                                  endValue, endTypeInfo,
                                                                  OPLE
                                                                  );
        }
        else
        {
                testValue = getGlobalModule()->addBinaryOperation(checkBlock,
                                                                 counterValueInCheckBlock.first, counterValueInCheckBlock.second,
                                                                 endValue, endTypeInfo,
                                                                 OPGE
                                                                 );
        }
        irBuilder.SetInsertPoint(checkBlock);
        llvm::Value* boolPred = irBuilder.CreateIntCast(testValue.first, llvm::Type::getInt1Ty(context), true);
        irBuilder.CreateCondBr(boolPred, bodyBlockPack.getFirstBlock(), afterBodyBlock);
        // Add instructions to body
        irBuilder.SetInsertPoint(bodyBlockPack.getFinalBlock());
        std::pair<llvm::Value*, BasicTypeInfo*> counterPtr3 = getGlobalModule()->addSubscription(
                    bodyBlockPack.getFinalBlock(),
                    counterInfo->getAllocaInst(), counterInfo->getType(),
                    {});
        std::pair<llvm::Value*, BasicTypeInfo*> counterValueInEndOfBody = getGlobalModule()->addLoadOperation(
                                                bodyBlockPack.getFinalBlock(),
                                                counterPtr3.first, counterPtr3.second
                                                );
        std::pair<llvm::Value*, BasicTypeInfo*> newCounterValue;
        if (direction == RWTO)
        {
                newCounterValue = getGlobalModule()->addBinaryOperation(bodyBlockPack.getFinalBlock(),
                                                                  counterValueInEndOfBody.first, counterValueInEndOfBody.second,
                                                                  llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1, true), getGlobalModule()->getTypeInfo("integer"),
                                                                  OPPLUS
                                                                  );
        }
        else
        {
                newCounterValue = getGlobalModule()->addBinaryOperation(bodyBlockPack.getFinalBlock(),
                                                                  counterValueInEndOfBody.first, counterValueInEndOfBody.second,
                                                                  llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1, true), getGlobalModule()->getTypeInfo("integer"),
                                                                  OPMINUS
                                                                  );
        }
        getGlobalModule()->addStoreOperation(bodyBlockPack.getFinalBlock(),
                                             counterPtr3.first, counterPtr3.second,
                                             newCounterValue.first, newCounterValue.second);
        irBuilder.SetInsertPoint(bodyBlockPack.getFinalBlock());
        irBuilder.CreateBr(checkBlock);
        // Linke them all
        currentContext->getCurrentBlock()->setFinalBlock(afterBodyBlock);
    }
};
//  {LoopDirection,{RWDT}, TreeConstructor},
class LoopDirectionDT : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["direction"] = RWDT;

    }
};
//  {LoopDirection,{RWTO}, TreeConstructor},
class LoopDirectionTO : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["direction"] = RWTO;
    }
};

//  {ConditionalOperator,{RWIF,Expression,RWTH, CreateBlock, Statement2,ConditionalOperator1}, TreeConstructor},
class ConditionalOperatorAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;

        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack thenBlockPack = *(currentContext->getCurrentBlock());
        currentContext->popBlockPack();

        llvm::BasicBlock* previousBlock  = currentContext->getCurrentBlock()->getFinalBlock();
        llvm::BasicBlock* afterBodyBlock = currentContext->registerBasicBlock("after_if@" + getGlobalModule()->getAnonimousName());
        llvm::IRBuilder<> irBuilder(thenBlockPack.getFinalBlock());
        irBuilder.CreateBr(afterBodyBlock);

        auto& context = getGlobalModule()->getLLVMModule().getContext();

        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::pair<llvm::Value*, BasicTypeInfo*> predicateValuePair = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[2], "Value");

        if (predicateValuePair.second->getType() != BasicTypeInfo::INTEGER) // TODO check second adgument
        {
            throw std::runtime_error("Only integer supported as counter in for statement");
        }
        irBuilder.SetInsertPoint(previousBlock);
        llvm::Value* boolPred = irBuilder.CreateIntCast(predicateValuePair.first, llvm::Type::getInt1Ty(context), true);
        bool flag = cast_item<bool>(m_stack[6], "ElseFlag");
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        // If else exist
        if (cast_item<bool>(m_stack[6], "ElseFlag"))
        {
            std::cout <<  "__PRETTY_FUNCTION__" << std::endl;
            std::cout <<  __PRETTY_FUNCTION__ << std::endl;
            BlockPack elseBlodyPack = cast_item<BlockPack>( m_stack[6], "ElseBlock");
            irBuilder.SetInsertPoint(previousBlock);
            irBuilder.CreateCondBr(boolPred, thenBlockPack.getFirstBlock(), elseBlodyPack.getFirstBlock());
            irBuilder.SetInsertPoint(elseBlodyPack.getFinalBlock());
            irBuilder.CreateBr(afterBodyBlock);
        }
        else
        {
            std::cout <<  "__PRETTY_FUNCTION__" << std::endl;
            irBuilder.SetInsertPoint(previousBlock);
            irBuilder.CreateCondBr(boolPred, thenBlockPack.getFirstBlock(), afterBodyBlock);
        }
        // Linke them all
        currentContext->getCurrentBlock()->setFinalBlock(afterBodyBlock);
    }
};
//  {ConditionalOperator1,{RWEL, CreateBlock, Statement2}, TreeConstructor},
class ConditionalOperatorElseAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["ElseFlag"] = true;
        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack bodyBlockPack = *(currentContext->getCurrentBlock());
        currentContext->popBlockPack();
        (*m_value)["ElseBlock"] = bodyBlockPack;
    }
};
//  {ConditionalOperator1,{EPSILON}, TreeConstructor},
class ConditionalOperatorNoElseAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["ElseFlag"] = false;
    }
};

//  {InputOperator,{RWRD,SRLP,LeftHandVar,InputOperator1,SRRP}, TreeConstructor},
class ReadLeftHandVarAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::pair<llvm::Value*, BasicTypeInfo*> value = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[3], "Value");

        if (value.second->getType() == BasicTypeInfo::INTEGER)
        {
            getGlobalModule()->addScanfIntCall(getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                value.first);
        }
        else
        {
            throw std::runtime_error("Printf not int value");
        }
    }
};

//  {OutputOperator,{RWWR,SRLP,Expression,SRRP}, TreeConstructor},
class PrintOperatorMain : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::vector<std::pair<llvm::Value*, BasicTypeInfo*>> expr_list = cast_item<std::vector<std::pair<llvm::Value*, BasicTypeInfo*>>>(m_stack[4], "expr_list");
        std::string text = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[3]));
        if (text.size() == 2)
        {
            getGlobalModule()->addPrintfENDL(getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock());
            return;
        }
        else
        {
            getGlobalModule()->addPrintfString(getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                getGlobalModule()->getCharStringPtr(text.substr(1, text.size()-2).c_str()));
        }
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        for (const auto& value: expr_list)
        {
            if (value.second->getType() == BasicTypeInfo::INTEGER)
            {
                getGlobalModule()->addPrintfIntCall(getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                    value.first);
            }
            else
            {
                throw std::runtime_error("Printf not int value");
            }
        }
    }
};
//  {OutputOperator1,{SRCA,Expression}, TreeConstructor},
class PrintOperatorRest : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        std::pair<llvm::Value*, BasicTypeInfo*> valuePair = cast_item<std::pair<llvm::Value*, BasicTypeInfo*>>(m_stack[2], "Value");
        if (valuePair.second->getType() == BasicTypeInfo::INTEGER)
        {
            throw std::runtime_error("Printf not int value");
        }
    }
};


}

#endif /* STATEMENTSACTIONS_HPP */
