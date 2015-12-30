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
        TreeConstructor::executeHandler();
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
        TreeConstructor::executeHandler();
        std::cout << "LABLEOOM" << std::endl;
        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack* currentBlockPack = currentContext->getCurrentBlock();

        bool isLabel = cast_item<bool>(m_stack[2], "label");
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));

        if (isLabel)
        {
            llvm::BasicBlock* currentFinalBlock = currentBlockPack->getFinalBlock();
            llvm::BasicBlock* labelBlock = currentContext->getBasicBlock(id);
            llvm::IRBuilder<> builder(currentFinalBlock);
            builder.CreateBr(labelBlock);
            currentBlockPack->setFinalBlock(labelBlock);
        }
        else
        {
            //throw std::runtime_error("Assignment not suported");
        }
    }
};

//  {Statement1,{SRCN,Statement}, TreeConstructor},
class LabelAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["label"] = true;
    }
};

//  {Statement1,{ExprList,LeftHandVar1,OPAS,Expression}, TreeConstructor},
class AssignmentAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["label"] = false;
        //throw std::runtime_error("Assignment not suported");
    }
};

//  {UnconditionalJumpOperator,{RWGT,Label}, TreeConstructor},
//  UNSUPPORTED

//  {LoopWithParameterOperator,{RWFOR,ID,OPAS,Expression,LoopDirection,Expression,RWDO, CreateBlock, Statement}, TreeConstructor},
class LoopWithParameterAction : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        TokenType direction = cast_item<TokenType>(m_stack[5],"direction");

        Context* currentContext = getGlobalModule()->getCurrentContext();
        BlockPack bodyBlockPack = *(currentContext->getCurrentBlock());
        currentContext->popBlockPack();

        llvm::BasicBlock* previousBlock  = currentContext->getCurrentBlock()->getFinalBlock();
        llvm::BasicBlock* checkBlock     = currentContext->registerBasicBlock(getGlobalModule()->getAnonimousName());
        llvm::BasicBlock* afterBodyBlock = currentContext->registerBasicBlock(getGlobalModule()->getAnonimousName());

        llvm::IRBuilder<> irBuilder(previousBlock);

        VarInfo*         counterInfo = currentContext->getVariable(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[2])));
        llvm::Value*     initValue = cast_item<llvm::Value*>(m_stack[4], "expressionValue");
        BasicTypeInfo*   initTypeInfo = cast_item<BasicTypeInfo*>(m_stack[4], "expressionTypeInfo");

        if (counterInfo->getType()->getType() != BasicTypeInfo::INTEGER) // TODO check second adgument
        {
            throw std::runtime_error("Only integer supported as counter in for statement");
        }
        // Add to previous block
        getGlobalModule()->addBinaryOperation(previousBlock,
                                              counterInfo->getAllocaInst(), counterInfo->getType(),
                                              initValue, initTypeInfo,
                                              OPAS);
        irBuilder.SetInsertPoint(previousBlock);
        irBuilder.CreateBr(checkBlock);
        // Construct create block
        llvm::Value*     endValue = cast_item<llvm::Value*>(m_stack[6], "expressionValue");
        BasicTypeInfo*   endTypeInfo = cast_item<BasicTypeInfo*>(m_stack[6], "expressionTypeInfo");

        irBuilder.SetInsertPoint(checkBlock);
        llvm::Value* checkCounterPtr = irBuilder.CreateGEP(counterInfo->getAllocaInst(), {llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0)});
        llvm::Value* checkCounterValue = irBuilder.CreateLoad(checkCounterPtr);
        std::pair<llvm::Value*, BasicTypeInfo*> testValue;
        if (direction == RWTO)
        {
                testValue = getGlobalModule()->addBinaryOperation(checkBlock,
                                                                  checkCounterValue, counterInfo->getType(),
                                                                  endValue, endTypeInfo,
                                                                  OPLE
                                                                  );
        }
        else
        {
                testValue = getGlobalModule()->addBinaryOperation(checkBlock,
                                                                 checkCounterValue, counterInfo->getType(),
                                                                 endValue, endTypeInfo,
                                                                 OPGE
                                                                 );
        }
        irBuilder.SetInsertPoint(checkBlock);
        irBuilder.CreateCondBr(testValue.first, bodyBlockPack.getFirstBlock(), afterBodyBlock);
        // Add instructions to body

        irBuilder.SetInsertPoint(bodyBlockPack.getFinalBlock());
        llvm::Value* bodyCounterPtr = irBuilder.CreateGEP(counterInfo->getAllocaInst(), {llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0)});
        llvm::Value* bodyCounterValue = irBuilder.CreateLoad(bodyCounterPtr);
        std::pair<llvm::Value*, BasicTypeInfo*> newCounterValue;
        if (direction == RWTO)
        {
                newCounterValue = getGlobalModule()->addBinaryOperation(bodyBlockPack.getFinalBlock(),
                                                                  bodyCounterValue, counterInfo->getType(),
                                                                  llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 1, true), getGlobalModule()->getTypeInfo("integer"),
                                                                  OPPLUS
                                                                  );
        }
        else
        {
                newCounterValue = getGlobalModule()->addBinaryOperation(bodyBlockPack.getFinalBlock(),
                                                                  bodyCounterValue, counterInfo->getType(),
                                                                  llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 1, true), getGlobalModule()->getTypeInfo("integer"),
                                                                  OPMINUS
                                                                  );
        }
        irBuilder.SetInsertPoint(bodyBlockPack.getFinalBlock());
        irBuilder.CreateStore(newCounterValue.first, bodyCounterPtr);
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
        TreeConstructor::executeHandler();
        (*m_value)["direction"] = RWDT;

    }
};
//  {LoopDirection,{RWTO}, TreeConstructor},
class LoopDirectionTO : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["direction"] = RWTO;
    }
};

//  {ConditionalOperator,{RWIF,Expression,RWTH, CreateBlock, Statement2,ConditionalOperator1}, TreeConstructor},
//  {ConditionalOperator1,{RWEL, CreateBlock, Statement2}, TreeConstructor},
//  {ConditionalOperator1,{EPSILON}, TreeConstructor},

//  {InputOperator,{RWRD,SRLP,LeftHandVar,InputOperator1,SRRP}, TreeConstructor},
//  {InputOperator1,{SRCA,LeftHandVar}, TreeConstructor},

//  {OutputOperator,{RWWR,SRLP,Expression,OutputOperator1,SRRP}, TreeConstructor},
//  {OutputOperator1,{SRCA,Expression}, TreeConstructor},

}

#endif /* STATEMENTSACTIONS_HPP */
