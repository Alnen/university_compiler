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
class ValuePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
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
        TreeConstructor::executeHandler();
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
        TreeConstructor::executeHandler();
        ValuePair first_nt_codegen = cast_item<ValuePair>(m_stack[2], "Value");
        ValuePair result_codegen;
        if (isOP(m_stack[3], "OP"))
        {
            TokenType second_nt_op = cast_item<TokenType>(m_stack[3], "OP");
            ValuePair second_nt_codogen = cast_item<ValuePair>(m_stack[3], "Value");

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
        (*m_value)["OP"] = cast_item<TokenType>(m_stack[1], "OP");
        (*m_value)["Value"] = result_codegen;
    }
};


// {LogicOr1,{EPSILON}},
// {LogicAnd1,{EPSILON}},
// {Relation1,{EPSILON}},
// {Expression2,{EPSILON}},
// {Summand1,{EPSILON}},
// {Factor1,{EPSILON}},
// {Var2,{EPSILON}},
// {Var3,{EPSILON}},
// {LeftHandVar1,{EPSILON}},
// {RightHandVar1,{EPSILON}},
// DEFAULT


// {Factor,{SRLP,LogicOr,SRRP}},
class BracketValuePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["Value"] = cast_item<ValuePair>(m_stack[2], "Value");
    }
};

// {Factor,{UnaryOperator,Var1}},
class UnaryOPValue : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
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





class BracketValuePropagation : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["Value"] = cast_item<ValuePair>(m_stack[2], "Value");
    }
};

// {Var1,{CI,Var3}},
// {Var3,{SRSP,CI,Var2}},

// {Var1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},

// {Var3,{CJ}},
// {Var2,{CJ}},


// {Var1,{RightHandVar}},

// {LeftHandVar,{ID,LeftHandVar3}},
// {LeftHandVar3,{LeftHandVar1}},
// {LeftHandVar3,{ExprList,LeftHandVar1}},

// {RightHandVar,{ID,RightHandVar3}},
// {RightHandVar3,{RightHandVar1}},
// {RightHandVar3,{ExprList,RightHandVar1}},

// {ExprList,{EPSILON}},
// {ExprList,{SRLB,Expression,ExprList1,SRRB}},
// {ExprList1,{SRCA,Expression,ExprList1}},
// {ExprList1,{EPSILON}},



// {BinaryAdditiveOperator,{OPPLUS}},
class UnaryOperatorOPPLUS : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPPLUS;
    }
};
// {BinaryAdditiveOperator,{OPMINUS}},
class UnaryOperatorOPMINUS : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPMINUS;
    }
};
// {BinaryMulOperator,{OPMUL}},
class UnaryOperatorOPMUL : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPMUL;
    }
};
// {BinaryMulOperator,{OPDIV}},
class UnaryOperatorOPDIV : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPDIV;
    }
};

// {UnaryOperator1,{RWINTOP}},
class UnaryOperatorRWINTOP : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWINTOP;
    }
};
// {UnaryOperator1,{RWIR}},
class UnaryOperatorRWIR : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWIR;
    }
};
// {UnaryOperator1,{RWSM}},
class UnaryOperatorRWSM : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWSM;
    }
};
// {UnaryOperator1,{RWFR}},
class UnaryOperatorRWFR : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWFR;
    }
};
// {UnaryOperator1,{RWLN}},
class UnaryOperatorRWLN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWLN;
    }
};
// {UnaryOperator1,{RWIM}},
class UnaryOperatorRWIM : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWIM;
    }
};
// {UnaryOperator1,{RWRE}},
class UnaryOperatorRWRE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWRE;
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
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = cast_item<ValuePair>(m_stack[1], "OP");
    }
};

// {BinaryRelationOperator,{OPGT}},
class BinaryRelationOperatorOPGT : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPGT;
    }
};
// {BinaryRelationOperator,{OPLT}},
class BinaryRelationOperatorOPLT : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPLT;
    }
};
// {BinaryRelationOperator,{OPGE}},
class BinaryRelationOperatorOPGE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPGE;
    }
};
// {BinaryRelationOperator,{OPLE}},
class BinaryRelationOperatorOPLE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPLE;
    }
};
// {BinaryRelationOperator,{OPEQ}},
class BinaryRelationOperatorOPEQ : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPEQ;
    }
};
// {BinaryRelationOperator,{OPNE}},
class BinaryRelationOperatorOPNE : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = OPNE;
    }
};

// {PostfixRationalOperator,{SRSP,PostfixRationalOperator1}},
class PostfixRationalOperatorRWDNN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = cast_item<TokenType>(m_stack[1], "OP");
    }
};

// {PostfixRationalOperator1,{RWDN}},
class PostfixRationalOperatorRWDN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWDN;
    }
};
// {PostfixRationalOperator1,{RWN}},
class PostfixRationalOperatorRWN : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
        (*m_value)["OP"] = RWN;
    }
};



class Expression : public TreeConstructor
{
public:
    virtual void executeHandler() override
    {
        TreeConstructor::executeHandler();
    }
};

}

#endif // EXPRESSION_H
