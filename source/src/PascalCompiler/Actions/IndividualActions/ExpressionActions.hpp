#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "BaseHandlers.h"
#include "llvm/IR/Value.h"
#include "boost/any.hpp"
#include "../../Grammar/Terminals.h"
#include "../../IR/GlobalContext.h"
#include "../../IR/Module.h"
#include "../../IR/Context.hpp"

namespace PascalCompiler
{
using ValuePair = std::pair<llvm::Value*, BasicTypeInfo*>;

//----------------------------------------------
// {Expression,{LogicOr}},   DEFAULT
// {Factor,{Var1}},
// {Var1,{RightHandVar}},
class ValuePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["Value"] = cast_item<ValuePair>(m_stack[1], "Value");
    }
};

// {LogicOr,{LogicAnd,LogicOr1}},
// {LogicAnd,{Relation,LogicAnd1}},
// {Relation,{Expression1,Relation1}},
// {Expression1,{Summand,Expression2}},
// {Summand,{Factor,Factor1}},

class ExpressionCombiner : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        ValuePair first_nt_codegen = cast_item<ValuePair>(m_stack[1], "Value");
        ValuePair result_codegen;
        if (isOP(m_stack[2], "OP"))
        {
            TokenType second_nt_op = cast_item<TokenType>(m_stack[2], "OP");
            ValuePair second_nt_codogen = cast_item<ValuePair>(m_stack[2], "Value");

            result_codegen = getGlobalModule()->addBinaryOperation(
                                                    getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                    first_nt_codegen.first, first_nt_codegen.second,
                                                    second_nt_codogen.first, second_nt_codogen.second,
                                                    second_nt_op
                                                    );
            //PascalCompiler::IR::PascalModule* module = PascalCompiler::IR::getModule();
        }
        else
        {
            result_codegen = first_nt_codegen;
        }
        (*m_value)["Value"] = result_codegen;
    }
};

// {LogicOr1,{RWLO,LogicAnd,LogicOr1}},
// {LogicAnd1,{RWLA,Relation,LogicAnd1}},
// {Relation1,{BinaryRelationOperator,Expression1,Relation1}},
// {Expression2,{BinaryAdditiveOperator,Summand,Expression2}},
// {Summand1,{BinaryMulOperator,Factor,Factor1,Summand1}},
// {Factor1,{RWCM,Factor,Factor1}},


class SuperExpressionCombiner : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        ValuePair first_nt_codegen = cast_item<ValuePair>(m_stack[2], "Value");
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        ValuePair result_codegen;
        if (isOP(m_stack[3], "OP"))
        {
            TokenType second_nt_op = cast_item<TokenType>(m_stack[3], "OP");
            std::cout <<  __PRETTY_FUNCTION__ << std::endl;
            ValuePair second_nt_codogen = cast_item<ValuePair>(m_stack[3], "Value");
            std::cout <<  __PRETTY_FUNCTION__ << std::endl;

            result_codegen = getGlobalModule()->addBinaryOperation(
                                                    getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                    first_nt_codegen.first, first_nt_codegen.second,
                                                    second_nt_codogen.first, second_nt_codogen.second,
                                                    second_nt_op
                                                    );
        }
        else
        {
            result_codegen = first_nt_codegen;
        }
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = cast_item<TokenType>(m_stack[1], "OP");
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["Value"] = result_codegen;
    }
};


// {LogicOr1,{EPSILON}},
// {LogicAnd1,{EPSILON}},
// {Relation1,{EPSILON}},
// {Expression2,{EPSILON}},
// {Summand1,{EPSILON}},
// {Factor1,{EPSILON}},
// {LeftHandVar1,{EPSILON}},
// {RightHandVar1,{EPSILON}},
// {RatianalType2,{EPSILON}},
// DEFAULT


// {Factor,{SRLP,LogicOr,SRRP}},
class BracketValuePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["Value"] = cast_item<ValuePair>(m_stack[2], "Value");
    }
};

// {Factor,{UnaryOperator,Var1}},
class UnaryOPValue : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        TokenType second_nt_op = cast_item<TokenType>(m_stack[1], "OP");
        ValuePair first_nt_codegen = cast_item<ValuePair>(m_stack[2], "Value");
        ValuePair result_codegen;

        result_codegen = getGlobalModule()->addUnaryOperation(
                                                getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock(),
                                                first_nt_codegen.first, first_nt_codegen.second,
                                                second_nt_op
                                                );
        (*m_value)["Value"] = result_codegen;
    }
};


//---------------------------------------------- LOW LEVEL PART


// {Var1,{CI,Var3}},
class CreateConstComplex1 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        int first = std::stoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1])));
        if (cast_item<bool>(m_stack[2], "j_flag"))
        {
            // TODO
        }
        else
        {

        }
        ValuePair result;
        auto& context = PascalCompiler::getGlobalModule()->getLLVMModule().getContext();
        result.first = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), first, true);
        result.second = getGlobalModule()->getTypeInfo("integer");
        (*m_value)["Value"] = result;
    }
};
// {Var3,{SRSP,CI,Var2}},
class CreateConstComplex2 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        /*TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        int first = std::stoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1])));
        int rest = 1;
        if (isType<int>(m_stack[2], "rest"))
        {
            rest = cast_item<int>(m_stack[2], "rest");
        }*/
        // TODO
    }
};

// {Var1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},
class CreateConstComplex3 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        /*TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        int first = std::stoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1])));
        int rest = 1;
        if (isType<int>(m_stack[2], "rest"))
        {
            rest = cast_item<int>(m_stack[2], "rest");
        }*/
        // TODO
    }
};

// {RatianalType1,{CI,RatianalType2}, TreeConstructor},
class RationalStart : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        int first = std::stoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1])));
        int rest = 1;
        if (isType<int>(m_stack[2], "rest"))
        {
            rest = cast_item<int>(m_stack[2], "rest");
        }
        // TODO
    }
};

// {RatianalType2,{SRSP,CI}, TreeConstructor},
class RationalEnd : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["rest"] = std::stoi(boost::any_cast<std::string>(TOKEN_VALUE(m_stack[2])));
    }
};

// {Var3,{CJ}},
// {Var2,{CJ}},
class JFlagTrue : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["j_flag"] = true;
    }
};

// {Var2,{EPSILON}},
// {Var3,{EPSILON}},

class JFlagFalse : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["j_flag"] = false;

    }
};

// {LeftHandVar,{ID,VarHandlerList}},
class PropagateLeftHandVarInfo : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
        std::vector<VarModificator> modificators = cast_item<std::vector<VarModificator>>(m_stack[2], "VarHandlerList");
        VarInfo* idInfo = getGlobalModule()->getCurrentContext()->getVariable(id);
        llvm::BasicBlock* block = getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock();
        auto valuePair = getGlobalModule()->addSubscription(block,
                                                            idInfo->getAllocaInst(), idInfo->getType(),
                                                            modificators);
        (*m_value)["Value"] = valuePair;
    }
};

// {LeftHandVar3,{LeftHandVar1}},
// {LeftHandVar3,{ExprList,LeftHandVar1}},

// {LeftHandVar,{ID,LeftHandVar3}, TreeConstructor},
// {LeftHandVar3,{LeftHandVar1}, TreeConstructor},
// {LeftHandVar3,{SRLB, ExprList, SRRB, LeftHandVar1}, TreeConstructor},
// {LeftHandVar1,{EPSILON}, TreeConstructor},

// {RightHandVar,{ID,VarHandlerList}},
class LoadRightHandVar : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::string id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[1]));
        std::vector<VarModificator> modificators = cast_item<std::vector<VarModificator>>(m_stack[2], "VarHandlerList");
        VarInfo* idInfo = getGlobalModule()->getCurrentContext()->getVariable(id);
        llvm::BasicBlock* block = getGlobalModule()->getCurrentContext()->getCurrentBlock()->getFinalBlock();
        auto valuePair = getGlobalModule()->addSubscription(block,
                                                            idInfo->getAllocaInst(), idInfo->getType(),
                                                            modificators);
        (*m_value)["Value"] = getGlobalModule()->addLoadOperation(block, valuePair.first, valuePair.second);
    }
};

// {RightHandVar3,{RightHandVar1}},
/*class LoadRightHandVar2 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = cast_item<int>(m_stack[1], "OP");
        (*m_value)["expr_list"] =         (*m_value)["expr_list"] = std::vector<ValuePair>();
    }
};

// {RightHandVar3,{SRLB, ExprList, SRRB, RightHandVar1}, LoadRightHandVar3},
class LoadRightHandVar3 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["expr_list"] = cast_item<std::vector<ValuePair>>(m_stack[2], "expr_list");
        (*m_value)["OP"] = cast_item<int>(m_stack[4], "OP");
    }
};

// {RightHandVar1,{EPSILON}, LoadRightHandVar4},
class LoadRightHandVar4 : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = -1;
    }
};*/


// {ExprList,{SRLB,Expression,ExprList1,SRRB}},
class FinalAppendExprList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::vector<ValuePair> id_list;
        std::vector<ValuePair> id_rest = cast_item<std::vector<ValuePair>>(m_stack[2], "expr_list");
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        ValuePair new_id = cast_item<ValuePair>(m_stack[1], "Value");
        id_list.push_back(new_id);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["expr_list"] = id_list;
        std::cout <<  __PRETTY_FUNCTION__ << std::endl;
    }
};
// {ExprList1,{SRCA,Expression,ExprList1}},
class AppendExprList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::vector<ValuePair> id_list;
        std::vector<ValuePair> id_rest = cast_item<std::vector<ValuePair>>(m_stack[3], "expr_list");
        ValuePair new_id = cast_item<ValuePair>(m_stack[2], "Value");
        id_list.push_back(new_id);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        (*m_value)["expr_list"] = id_list;
    }
};

// {ExprList,{EPSILON}},
// {ExprList1,{EPSILON}},
class InitExprList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["expr_list"] = std::vector<ValuePair>();
    }
};

// {VarHandlerList,{SRLB, Expression, SRRB, VarHandlerList}, TreeConstructor},
class AppendIndexVarHandlerList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::vector<VarModificator> id_list;
        std::vector<VarModificator> id_rest = cast_item<std::vector<VarModificator>>(m_stack[4], "VarHandlerList");
        ValuePair indexValue = cast_item<ValuePair>(m_stack[2], "Value");
        VarModificator varModificator;
        varModificator.setIndexValue(indexValue);
        id_list.push_back(varModificator);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        (*m_value)["VarHandlerList"] = id_list;
    }
};
// {VarHandlerList,{SRSP, ID, VarHandlerList}, TreeConstructor},
class AppendMemberVarHandlerList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        std::vector<VarModificator> id_list;
        std::vector<VarModificator> id_rest = cast_item<std::vector<VarModificator>>(m_stack[3], "VarHandlerList");
        std::string new_id = boost::any_cast<std::string>(TOKEN_VALUE(m_stack[2]));
        VarModificator varModificator;
        varModificator.setName(new_id);
        id_list.push_back(varModificator);
        std::copy(id_rest.begin(), id_rest.end(), std::back_inserter(id_list));
        (*m_value)["VarHandlerList"] = id_list;
    }
};
// {VarHandlerList,{EPSILON},                TreeConstructor},
class InitVarHandlerList : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["VarHandlerList"] = std::vector<VarModificator>();
    }
};


// {BinaryAdditiveOperator,{OPPLUS}},
class UnaryOperatorOPPLUS : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPPLUS;
    }
};
// {BinaryAdditiveOperator,{OPMINUS}},
class UnaryOperatorOPMINUS : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPMINUS;
    }
};
// {BinaryMulOperator,{OPMUL}},
class UnaryOperatorOPMUL : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPMUL;
    }
};
// {BinaryMulOperator,{OPDIV}},
class UnaryOperatorOPDIV : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPDIV;
    }
};

// {UnaryOperator1,{RWINTOP}},
class UnaryOperatorRWINTOP : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWINTOP;
    }
};
// {UnaryOperator1,{RWIR}},
class UnaryOperatorRWIR : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWIR;
    }
};
// {UnaryOperator1,{RWSM}},
class UnaryOperatorRWSM : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWSM;
    }
};
// {UnaryOperator1,{RWFR}},
class UnaryOperatorRWFR : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWFR;
    }
};
// {UnaryOperator1,{RWLN}},
class UnaryOperatorRWLN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWLN;
    }
};

// {LeftHandVar1,{LeftHandPostfixRationalOperator}},
// {RightHandVar1,{RightHandPostfixRationalOperator}},
// {LeftHandPostfixRationalOperator,{PostfixRationalOperator}},
// {RightHandPostfixRationalOperator,{PostfixRationalOperator}},
// {UnaryOperator,{BinaryAdditiveOperator}},
// {UnaryOperator,{UnaryOperator1}},
// {UnaryAdditiveOperator,{BinaryAdditiveOperator}},
class OPPropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = cast_item<TokenType>(m_stack[1], "OP");
    }
};

// {BinaryRelationOperator,{OPGT}},
class BinaryRelationOperatorOPGT : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPGT;
    }
};
// {BinaryRelationOperator,{OPLT}},
class BinaryRelationOperatorOPLT : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPLT;
    }
};
// {BinaryRelationOperator,{OPGE}},
class BinaryRelationOperatorOPGE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPGE;
    }
};
// {BinaryRelationOperator,{OPLE}},
class BinaryRelationOperatorOPLE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPLE;
    }
};
// {BinaryRelationOperator,{OPEQ}},
class BinaryRelationOperatorOPEQ : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPEQ;
    }
};
// {BinaryRelationOperator,{OPNE}},
class BinaryRelationOperatorOPNE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = OPNE;
    }
};

// {PostfixRationalOperator,{SRSP,PostfixRationalOperator1}},
class PostfixRationalOperatorRWDNN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = cast_item<TokenType>(m_stack[1], "OP");
    }
};

// {BinaryLogicOperatorAnd,{RWLA}, BinaryLogicOperatorRWLA},
class BinaryLogicOperatorRWLA : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWLA;
    }
};

// {BinaryLogicOperatorOr,{RWLO}, BinaryLogicOperatorRWLO},
class BinaryLogicOperatorRWLO : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();std::cout <<  __PRETTY_FUNCTION__ << std::endl;
        (*m_value)["OP"] = RWLO;
    }
};

}

#endif // EXPRESSION_H
