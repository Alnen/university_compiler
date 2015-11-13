#ifndef ACTIONHANDLERS_H
#define ACTIONHANDLERS_H

#include <memory>
#include <utility>

#include "Parser/SyntaxAnalyzer.h"
#include "Lexer/Lexer.h"
#include "Nonterminals.h"
#include "Terminals.h"

namespace PascalParser
{

using token_t = Lexer::Lexer<TokenType, NonterminalSymbols>::TokenType;
using token_ptr_t = std::shared_ptr<Lexer::Token<token_t>>;

#define TOKEN(X)        boost::any_cast<token_ptr_t>(X)
#define TOKEN_VALUE(X)  boost::any_cast<token_ptr_t>(X)->value()
#define STRING(X)       boost::any_cast<std::string>(X)
#define NONTERMINAL(X)  boost::any_cast<Parser::NonterminalInfo<NonterminalSymbols>>(X)

class TreeElement
{
public:
    enum class Type
    {
        NONTERMINAL,
        TOKEN
    };

    TreeElement(Type type): m_type(type) {}
    Type type() const { return m_type; }

    virtual void print(std::ostream& out, size_t level, bool first) = 0;
    virtual void print_uml(std::ostream& out, std::string father, int& generator) = 0;

    virtual ~TreeElement() {}

protected:
    Type m_type;
};

class TreeNode : public TreeElement
{
public:
    TreeNode(NonterminalSymbols symbol): TreeElement(Type::NONTERMINAL), m_symbol(symbol) {}

    void addChild(std::shared_ptr<TreeElement> child)
    {
        m_children.push_back(std::move(child));
    }

    NonterminalSymbols symbol() const
    {
        return m_symbol;
    }

    const std::vector<std::shared_ptr<TreeElement>>& children()
    {
        return m_children;
    }

    virtual void print(std::ostream& out, size_t level, bool first) override
    {
        if (!first)
        {
            out << std::string(level*29, ' ');
        }
        out << std::setw(25) << nonterminalTypeMapping[m_symbol] << std::string(4, ' ');

        if (!m_children.empty())
        {
            m_children[0]->print(out, level+1, true);
        }
        for (size_t i = 1; i < m_children.size(); ++i)
        {
            m_children[i]->print(out, level+1, false);
        }
    }

    virtual void print_uml(std::ostream& out, std::string father, int& generator) override
    {
        auto name = nonterminalTypeMapping[m_symbol];
        auto unique_name = name + "_" + std::to_string(++generator);
        out << "state \"" << name << "\" as " << unique_name << std::endl;
        out << father << " --> " << unique_name << std::endl;
        for (auto& child : m_children)
        {
            child->print_uml(out, unique_name, generator);
        }
    }

protected:
    std::vector<std::shared_ptr<TreeElement>> m_children;
    NonterminalSymbols m_symbol;
};

class TreeLeaf : public TreeElement
{
public:
    TreeLeaf(token_ptr_t token): TreeElement(Type::TOKEN), m_token(token) {}

    token_ptr_t token() const
    {
        return m_token;
    }

    virtual void print(std::ostream& out, size_t level, bool first) override
    {
        if (!first)
        {
            out << std::string(level*29, ' ');
        }
        out << std::setw(25) << tokenTypeMapping[m_token->type()] << std::endl;
    }

    virtual void print_uml(std::ostream& out, std::string father, int& generator) override
    {
        auto name = tokenTypeMapping[m_token->type()];
        auto unique_name = name + "_" + std::to_string(++generator);
        out << "state \"" << name << "\" as " << unique_name << std::endl;
        out << unique_name << " : " << boost::any_cast<std::string>(m_token->value()) << std::endl;
        out << father << " --> " << unique_name << std::endl;
    }

protected:
    token_ptr_t m_token;
};

void print_uml(TreeElement* elem, std::ostream& out)
{
    out << "@startuml\n";
    int number_of_elements = 0;
    elem->print_uml(out, "[*]", number_of_elements);
    out << "@enduml\n";
}

class Action1 : public Parser::BaseSynthesizeItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "SYNTHESIZE program name is " << STRING(TOKEN_VALUE(*m_stack[2])) << std::endl;
    }
};

class Action2 : public Parser::BaseSynthesizeItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "ACTION2" << std::endl;
    }
};

class Action3 : public Parser::BaseInheritItem
{
public:
    virtual void executeHandler() override
    {
        //std::cout << "INHERITED program name is " << boost::any_cast<std::string>(boost::any_cast<Lexer::Token<int>>(*m_stack[2]).value()) << std::endl;
    }
};

//
class TreeConstructor : public Parser::BaseSynthesizeItem
{
public:
    virtual void executeHandler() override
    {
        std::cout << "TreeConstructor" << std::endl;
        std::cout << m_stack.size() << std::endl;
        std::shared_ptr<TreeNode> head = std::make_shared<TreeNode>(NONTERMINAL(*m_stack[0]).symbol());

        for (size_t i = 1; i < m_stack.size(); ++i)
        {
            if (isNeterminal(m_stack[i]))
            {
                std::cout<< "N" << std::endl;
                head->addChild(boost::any_cast<std::shared_ptr<TreeNode>>(*m_stack[i]));
            }
            else if (isTerminal(m_stack[i]))
            {
                std::cout<< "T" << std::endl;
                head->addChild(std::make_shared<TreeLeaf>(TOKEN(*m_stack[i])));
            }
            else
            {
                std::cout << m_stack.size() << std::endl;
                throw std::runtime_error("WTF!");
            }
        }
        *m_value = head;
    }

protected:
    bool isNeterminal(std::shared_ptr<boost::any>& value)
    {
        try
        {
            boost::any_cast<std::shared_ptr<TreeNode>>(*value);
            return true;
        }
        catch(const boost::bad_any_cast &)
        {
            return false;
        }
    }

    bool isTerminal(std::shared_ptr<boost::any>& value)
    {
        try
        {
            boost::any_cast<token_ptr_t>(*value);
            return true;
        }
        catch(const boost::bad_any_cast &)
        {
            return false;
        }
    }
};

   // {Program,{RWPR,ID,SRSM , ACTION3, Program1}, ACTION1},
   // {Program1,{ProcedureFunctions,CompoundStatement,SRSP}},
   // {Program1,{DescriptionSection1,DescriptionSection,ProcedureFunctions,CompoundStatement,SRSP}},
   // {DescriptionSection,{EPSILON}},
   // {DescriptionSection,{DescriptionSection1,DescriptionSection}},
   // {DescriptionSection1,{LabelSection}},
   // {DescriptionSection1,{ConstSection}},
   // {DescriptionSection1,{TypeSection}},
   // {DescriptionSection1,{VarSection}},
   // {DescriptionSection1,{OperatorOverloadSection}},
   // {ProcedureFunctions,{Function,ProcedureFunctions}},
   // {ProcedureFunctions,{Procedure,ProcedureFunctions}},
   // {ProcedureFunctions,{EPSILON}},
   // {LabelSection,{RWLB,Label,LabelSection1,SRSM}},
   // {LabelSection1,{EPSILON}},
   // {LabelSection1,{SRCA,Label,LabelSection1}},
   // {Label,{ID}},
   // {ConstSection,{RWCN,ConstDefinition,SRSM,ConstSection1}},
   // {ConstSection1,{EPSILON}},
   // {ConstSection1,{ConstDefinition,SRSM,ConstSection1}},
   // {ConstDefinition,{ID,OPEQ,Expression}},
   // {TypeSection,{RWTP,TypeDefinition,SRSM,TypeSection1}},
   // {TypeSection1,{EPSILON}},
   // {TypeSection1,{TypeDefinition,SRSM,TypeSection1}},
   // {TypeDefinition,{ID,OPEQ,Type}},
   // {Type,{ConcreteType}},
   // {Type,{Massive}},
   // {Type,{EnumType}},
   // {Type,{ID}},
   // {ConcreteType,{RWINT}},
   // {ConcreteType,{RWRAT}},
   // {ConcreteType,{RWCX}},
   // {Massive,{RWAR,SRLB,IndexType,Massive1,SRRB,RWOF,Type}},
   // {Massive1,{EPSILON}},
   // {Massive1,{SRCA,IndexType,Massive1}},
   // {IndexType,{EnumType}},
   // {IndexType,{ID}},
   // {EnumType,{SRLP,IdList,SRRP}},
   // {ConstExpr,{UnaryAdditiveOperator,ConstExpr1}},
   // {ConstExpr,{ConstExpr1}},
   // {ConstExpr1,{ID}},
   // {ConstExpr1,{CI,ConstExpr3}},
   // {ConstExpr1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},
   // {ConstExpr3,{CI,RatianalType2,CJ}},
   // {ConstExpr3,{SRSP,CI,ConstExpr2}},
   // {ConstExpr3,{CJ}},
   // {ConstExpr3,{EPSILON}},
   // {ConstExpr2,{CJ}},
   // {ConstExpr2,{EPSILON}},
   // {RatianalType1,{CI,RatianalType2}},
   // {RatianalType2,{SRSP,CI}},
   // {RatianalType2,{EPSILON}},
   // {VarSection,{RWV,VarSection1,VarSection2}},
   // {VarSection1,{IdList,SRCN,Type,SRSM}},
   // {VarSection2,{VarSection1,VarSection2}},
   // {VarSection2,{EPSILON}},
   // {IdList,{ID,IdList1}},
   // {IdList1,{SRCA,ID,IdList1}},
   // {IdList1,{EPSILON}},
   // {Expression,{LogicOr}},
   // {LogicOr,{LogicAnd,LogicOr1}},
   // {LogicOr1,{RWLO,LogicAnd,LogicOr1}},
   // {LogicOr1,{EPSILON}},
   // {LogicAnd,{Relation,LogicAnd1}},
   // {LogicAnd1,{RWLA,Relation,LogicAnd1}},
   // {LogicAnd1,{EPSILON}},
   // {Relation,{Expression1,Relation1}},
   // {Relation1,{BinaryRelationOperator,Expression1,Relation1}},
   // {Relation1,{EPSILON}},
   // {Expression1,{Summand,Expression2}},
   // {Expression2,{BinaryAdditiveOperator,Summand,Expression2}},
   // {Expression2,{EPSILON}},
   // {Summand,{Factor,Factor1,Summand1}},
   // {Summand1,{BinaryMulOperator,Factor,Factor1,Summand1}},
   // {Summand1,{EPSILON}},
   // {Factor1,{RWCM,Factor,Factor1}},
   // {Factor1,{EPSILON}},
   // {Factor,{UnaryOperator,Var1}},
   // {Factor,{Var1}},
   // {Factor,{SRLP,LogicOr,SRRP}},
   // {Var1,{ID,Var4}},
   // {Var1,{CI,Var3}},
   // {Var1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},
   // {Var3,{SRSP,CI,Var2}},
   // {Var3,{CJ}},
   // {Var3,{EPSILON}},
   // {Var2,{CJ}},
   // {Var2,{EPSILON}},
   // {Var4,{EPSILON}},
   // {Var4,{RightHandPostfixRationalOperator}},
   // {Var4,{SRLB,Expression,ExprList1,SRRB,RightHandVar1}},
   // {Var,{ID,ExprList}},
   // {ExprList,{EPSILON}},
   // {ExprList,{SRLB,Expression,ExprList1,SRRB}},
   // {ExprList1,{EPSILON}},
   // {ExprList1,{SRCA,Expression,ExprList1}},
   // {LeftHandVar,{Var,LeftHandVar1}},
   // {LeftHandVar1,{LeftHandPostfixRationalOperator}},
   // {LeftHandVar1,{EPSILON}},
   // {RightHandVar,{Var,RightHandVar1}},
   // {RightHandVar1,{RightHandPostfixRationalOperator}},
   // {RightHandVar1,{EPSILON}},
   // {BinaryAdditiveOperator,{OPPLUS}},
   // {BinaryAdditiveOperator,{OPMINUS}},
   // {BinaryMulOperator,{OPMUL}},
   // {BinaryMulOperator,{OPDIV}},
   // {UnaryOperator,{BinaryAdditiveOperator}},
   // {UnaryOperator,{UnaryOperator1}},
   // {UnaryOperator1,{RWINTOP}},
   // {UnaryOperator1,{RWIR}},
   // {UnaryOperator1,{RWSM}},
   // {UnaryOperator1,{RWFR}},
   // {UnaryOperator1,{RWLN}},
   // {UnaryOperator1,{RWIM}},
   // {UnaryOperator1,{RWRE}},
   // {UnaryAdditiveOperator,{BinaryAdditiveOperator}},
   // {BinaryRelationOperator,{OPGT}},
   // {BinaryRelationOperator,{OPLT}},
   // {BinaryRelationOperator,{OPGE}},
   // {BinaryRelationOperator,{OPLE}},
   // {BinaryRelationOperator,{OPEQ}},
   // {BinaryRelationOperator,{OPNE}},
   // {PostfixRationalOperator,{SRSP,PostfixRationalOperator1}},
   // {PostfixRationalOperator1,{RWDN}},
   // {PostfixRationalOperator1,{RWN}},
   // {LeftHandPostfixRationalOperator,{PostfixRationalOperator}},
   // {RightHandPostfixRationalOperator,{PostfixRationalOperator}},
   // {CompoundStatement,{RWB,Statement,CompoundStatement1}},
   // {CompoundStatement1,{SRSM,CompoundStatement2}},
   // {CompoundStatement1,{RWEND}},
   // {CompoundStatement2,{RWEND}},
   // {CompoundStatement2,{SRSM,CompoundStatement2}},
   // {CompoundStatement2,{Statement2,CompoundStatement1}},
   // {Statement,{EPSILON}},
   // {Statement,{Statement2}},
   // {Statement2,{ConditionalOperator}},
   // {Statement2,{CompoundStatement}},
   // {Statement2,{InputOperator}},
   // {Statement2,{OutputOperator}},
   // {Statement2,{UnconditionalJumpOperator}},
   // {Statement2,{LoopWithParameterOperator}},
   // {Statement2,{ID,Statement1}},
   // {Statement1,{SRCN,Statement}},
   // {Statement1,{ExprList,LeftHandVar1,OPAS,Expression}},
   // {LoopWithParameterOperator,{RWFOR,ID,OPAS,Expression,LoopDirection,Expression,RWDO,Statement}},
   // {LoopDirection,{RWDT}},
   // {LoopDirection,{RWTO}},
   // {AssignmentOperator,{LeftHandVar,OPAS,Expression}},
   // {UnconditionalJumpOperator,{RWGT,Label}},
   // {ConditionalOperator,{RWIF,Expression,RWTH,ConditionalOperator1}},
   // {ConditionalOperator1,{RWEL,Statement2}},
   // {ConditionalOperator1,{Statement2,ConditionalOperator2}},
   // {ConditionalOperator2,{RWEL,Statement2}},
   // {ConditionalOperator2,{EPSILON}},
   // {InputOperator,{RWRD,SRLP,LeftHandVar,InputOperator1,SRRP}},
   // {InputOperator1,{SRCA,LeftHandVar}},
   // {OutputOperator,{RWWR,SRLP,Expression,OutputOperator1,SRRP}},
   // {OutputOperator1,{SRCA,Expression}},
   // {OperatorOverloadSection,{RWOP,OperatorOverloadSection1}},
   // {OperatorOverloadSection1,{BinaryAdditiveOperator,SRLP,ID,OperatorOverloadSection2}},
   // {OperatorOverloadSection1,{OverloadBinaryOperation1,SRLP,ID,BinaryOperatorOverload1,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
   // {OperatorOverloadSection1,{UnaryOperator1,SRLP,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,OperatorOverloadSection1,CompoundStatement,SRSM}},
   // {OperatorOverloadSection2,{SRCA,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
   // {OperatorOverloadSection2,{SRCN,Type,OperatorOverloadSection3}},
   // {OperatorOverloadSection3,{SRSM,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
   // {OperatorOverloadSection3,{SRRP,ID,SRCN,Type,SRSM,OperatorOverloadSection1,CompoundStatement,SRSM}},
   // {LocalDefinition1,{EPSILON}},
   // {LocalDefinition1,{LocalDefinition,OperatorOverloadSection1}},
   // {LocalDefinition,{TypeSection}},
   // {LocalDefinition,{VarSection}},
   // {LocalDefinition,{ConstSection}},
   // {LocalDefinition,{LabelSection}},
   // {UnaryOperatorOverload,{UnaryOperator,SRLP,ID,SRCN,Type,SRRP}},
   // {BinaryOperatorOverload,{OverloadBinaryOperation,SRLP,ID,BinaryOperatorOverload1}},
   // {BinaryOperatorOverload1,{SRCA,ID,SRCN,Type,SRRP}},
   // {BinaryOperatorOverload1,{SRCN,Type,SRSM,ID,SRCN,Type,SRRP}},
   // {OverloadBinaryOperation,{BinaryAdditiveOperator}},
   // {OverloadBinaryOperation,{OverloadBinaryOperation1}},
   // {OverloadBinaryOperation1,{BinaryMulOperator}},
   // {OverloadBinaryOperation1,{RWLO}},
   // {OverloadBinaryOperation1,{RWLA}},
   // {OverloadBinaryOperation1,{OPAS}},
   // {OverloadBinaryOperation1,{RWCM}},
   // {Function,{RWFUN,ID,SRLP,FunctionArgs,SRRP,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
   // {FunctionArgs,{OneTypeArgs,FunctionArgs1}},
   // {FunctionArgs1,{SRSM,FunctionArgs}},
   // {FunctionArgs1,{EPSILON}},
   // {OneTypeArgs,{ID,OneTypeArgs1}},
   // {OneTypeArgs1,{SRCA,ID,IdList1,SRCN,Type}},
   // {OneTypeArgs1,{SRCN,Type}},
   // {Procedure,{RWPRC,ID,SRLP,FunctionArgs,ResultArgs,SRRP,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
   // {ResultArgs,{EPSILON}},
   // {ResultArgs,{SRSM,RWV,FunctionArgs}},

}

#endif // ACTIONHANDLERS_H
