#include <memory>
#include <boost/bimap.hpp>

#include "Lexer/Lexer.h"
#include "Lexer/TokenHandler.h"

// experimental
#include <iostream>
#include "Grammar.h"
#include "ControlTable.h"
#include "SyntaxAnalyzer.h"

#include <string>
#include <fstream>
#include "set.h"

enum TokenType
{
    TERMINAL_START_INDEX = 0,
    //
    ID,
    CJ,
    CI,
    //
    RWLA,
    RWAR,
    RWB,
    RWCM,
    RWCN,
    RWDO,
    RWDT,
    RWEL,
    RWEND,
    RWFOR,
    RWFR,
    RWGT,
    RWIF,
    RWINT,
    RWIR,
    RWLB,
    RWLN,
    RWOF,
    RWOP,
    RWLO,
    RWPR,
    RWRAT,
    RWRD,
    RWSM,
    RWTH,
    RWTO,
    RWTP,
    RWV,
    RWWR,
    RWCX,
    RWFUN,
    RWIM,
    RWRE,
    RWINTOP,
    RWPRC,
    RWDN,
    RWN,
    //
    OPPLUS,
    OPMINUS,
    OPMUL,
    OPDIV,
    OPGT,
    OPLT,
    OPGE,
    OPLE,
    OPEQ,
    OPNE,
    OPAS,
    //
    SRLP,
    SRRP,
    SRLB,
    SRRB,
    SRSM,
    SRCN,
    SRSP,
    SRCA,
    SRLCB,
    SRRCB,
    //
    ERROR,
    ENDOFFILE,
    //
    TERMINAL_FINISH_INDEX
};

enum NonterminalSymbols {
    NONTERMINAL_START_INDEX = 99,
    AssignmentOperator, // 100
    BinaryAdditiveOperator, // 101
    BinaryMulOperator, // 102
    BinaryOperatorOverload, // 103
    BinaryOperatorOverload1, // 104
    BinaryRelationOperator, // 105
    CompoundStatement, // 106
    CompoundStatement1, // 107
    CompoundStatement2, // 108
    ConcreteType, // 109
    ConditionalOperator, // 110
    ConditionalOperator1, // 111
    ConditionalOperator2, // 112
    ConstDefinition, // 113
    ConstExpr, // 114
    ConstExpr1, // 115
    ConstExpr2, // 116
    ConstSection, // 117
    ConstSection1, // 118
    DescriptionSection, // 119
    DescriptionSection1, // 120
    EnumType, // 121
    ExprList, // 122
    ExprList1, // 123
    Expression, // 124
    Expression1, // 125
    Expression2, // 126
    Factor, // 127
    Factor1, // 128
    Function, // 129
    FunctionArgs, // 130
    FunctionArgs1, // 131
    IdList, // 132
    IdList1, // 133
    IndexType, // 134
    InputOperator, // 135
    InputOperator1, // 136
    Label, // 137
    LabelSection, // 138
    LabelSection1, // 139
    LeftHandPostfixRationalOperator, // 140
    LeftHandVar, // 141
    LeftHandVar1, // 142
    LocalDefinition, // 143
    LocalDefinition1, // 144
    LogicAnd, // 145
    LogicAnd1, // 146
    LogicOr, // 147
    LogicOr1, // 148
    LoopDirection, // 149
    LoopWithParameterOperator, // 150
    Massive, // 151
    Massive1, // 152
    OneTypeArgs, // 153
    OneTypeArgs1, // 154
    OperatorOverloadSection, // 155
    OperatorOverloadSection1, // 156
    OperatorOverloadSection2, // 157
    OperatorOverloadSection3, // 158
    OutputOperator, // 159
    OutputOperator1, // 160
    OverloadBinaryOperation, // 161
    OverloadBinaryOperation1, // 162
    PostfixRationalOperator, // 163
    PostfixRationalOperator1, // 164
    Procedure, // 165
    ProcedureFunctions, // 166
    Program, // 167
    Program1, // 168
    RatianalType, // 169
    RatianalType1, // 170
    RatianalType2, // 171
    Relation, // 172
    Relation1, // 173
    ResultArgs, // 174
    RightHandPostfixRationalOperator, // 175
    RightHandVar, // 176
    RightHandVar1, // 177
    Statement, // 178
    Statement1, // 179
    Statement2, // 180
    Summand, // 181
    Summand1, // 182
    Type, // 183
    TypeDefinition, // 184
    TypeSection, // 185
    TypeSection1, // 186
    UnaryAdditiveOperator, // 187
    UnaryOperator, // 188
    UnaryOperator1, // 189
    UnaryOperatorOverload, // 190
    UnconditionalJumpOperator, // 191
    Var, // 192
    Var1, // 193
    Var2, // 194
    VarSection, // 195
    VarSection1, // 196
    VarSection2, // 197
    NONTERMINAL_FINISH_INDEX,
    EPSILON
};

class IDHandler : public Lexer::TokenHandler<TokenType>
{
public:
    virtual TokenPtr operator()(TokenPtr a) override
    {
        a->value() = boost::any_cast<std::string>(a->value()) + "ANY";
        return a;
    }
};

int main (int argc, char** argv)
{
    Parser::Set<TokenType> terminals;
    for (size_t i = TERMINAL_START_INDEX + 1;
    i < TERMINAL_FINISH_INDEX; ++i)
    {
        terminals.add(TokenType(i));
    }
    Parser::Set<NonterminalSymbols> nonterminals;
    for (size_t i = NONTERMINAL_START_INDEX + 1;
        i < NONTERMINAL_FINISH_INDEX; ++i)
    {
        nonterminals.add(NonterminalSymbols(i));
    }

    Parser::Grammar<TokenType, NonterminalSymbols>::RuleList grammar_rules = {
        {Program,{RWPR,ID,SRSM,Program1}},
        {Program1,{ProcedureFunctions,CompoundStatement,SRSP}},
        {Program1,{DescriptionSection1,DescriptionSection,ProcedureFunctions,CompoundStatement,SRSP}},
        {DescriptionSection,{EPSILON}},
        {DescriptionSection,{DescriptionSection1,DescriptionSection}},
        {DescriptionSection1,{LabelSection}},
        {DescriptionSection1,{ConstSection}},
        {DescriptionSection1,{TypeSection}},
        {DescriptionSection1,{VarSection}},
        {DescriptionSection1,{OperatorOverloadSection}},
        {ProcedureFunctions,{Function,ProcedureFunctions}},
        {ProcedureFunctions,{Procedure,ProcedureFunctions}},
        {ProcedureFunctions,{EPSILON}},
        {LabelSection,{RWLB,Label,LabelSection1,SRSM}},
        {LabelSection1,{EPSILON}},
        {LabelSection1,{SRCA,Label,LabelSection1}},
        {Label,{ID}},
        {ConstSection,{RWCN,ConstDefinition,SRSM,ConstSection1}},
        {ConstSection1,{EPSILON}},
        {ConstSection1,{ConstDefinition,SRSM,ConstSection1}},
        {ConstDefinition,{ID,OPEQ,Expression}},
        {TypeSection,{RWTP,TypeDefinition,SRSM,TypeSection1}},
        {TypeSection1,{EPSILON}},
        {TypeSection1,{TypeDefinition,SRSM,TypeSection1}},
        {TypeDefinition,{ID,OPEQ,Type}},
        {Type,{ConcreteType}},
        {Type,{Massive}},
        {Type,{EnumType}},
        {Type,{ID}},
        {ConcreteType,{RWINT}},
        {ConcreteType,{RWRAT}},
        {Massive,{RWAR,SRLB,IndexType,Massive1,SRRB,RWOF,Type}},
        {Massive1,{EPSILON}},
        {Massive1,{SRCA,IndexType,Massive1}},
        {IndexType,{EnumType}},
        {IndexType,{ID}},
        {EnumType,{SRLP,IdList,SRRP}},
        {ConstExpr,{UnaryAdditiveOperator,ConstExpr1}},
        {ConstExpr,{ConstExpr1}},
        {ConstExpr1,{ID}},
        {ConstExpr1,{CI,ConstExpr2}},
        {ConstExpr1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},
        {ConstExpr2,{RatianalType2,CJ}},
        {ConstExpr2,{EPSILON}},
        {RatianalType,{CI,SRSP,CI}},
        {RatianalType1,{CI,RatianalType2}},
        {RatianalType2,{SRSP,CI}},
        {RatianalType2,{EPSILON}},
        {VarSection,{RWV,VarSection1,VarSection2}},
        {VarSection1,{IdList,SRCN,Type,SRSM}},
        {VarSection2,{VarSection1,VarSection2}},
        {VarSection2,{EPSILON}},
        {IdList,{ID,IdList1}},
        {IdList1,{SRCA,ID,IdList1}},
        {IdList1,{EPSILON}},
        {Expression,{LogicOr}},
        {LogicOr,{LogicAnd,LogicOr1}},
        {LogicOr1,{RWLO,LogicAnd,LogicOr1}},
        {LogicOr1,{EPSILON}},
        {LogicAnd,{Relation,LogicAnd1}},
        {LogicAnd1,{RWLA,Relation,LogicAnd1}},
        {LogicAnd1,{EPSILON}},
        {Relation,{Expression1,Relation1}},
        {Relation1,{BinaryRelationOperator,Expression1,Relation1}},
        {Relation1,{EPSILON}},
        {Expression1,{Summand,Expression2}},
        {Expression2,{BinaryAdditiveOperator,Summand,Expression2}},
        {Expression2,{EPSILON}},
        {Summand,{Factor,Factor1,Summand1}},
        {Summand1,{BinaryMulOperator,Factor,Factor1,Summand1}},
        {Summand1,{EPSILON}},
        {Factor1,{RWCM,Factor,Factor1}},
        {Factor1,{EPSILON}},
        {Factor,{UnaryOperator,Var1}},
        {Factor,{Var1}},
        {Factor,{SRLP,LogicOr,SRRP}},
        {Var1,{RightHandVar}},
        {Var1,{CI,Var2}},
        {Var1,{SRLCB,RatianalType1,SRCA,RatianalType1,SRRCB}},
        {Var2,{RatianalType2,CJ}},
        {Var2,{EPSILON}},
        {Var,{ID,ExprList}},
        {ExprList,{EPSILON}},
        {ExprList,{SRLB,Expression,ExprList1,SRRB}},
        {ExprList1,{EPSILON}},
        {ExprList1,{SRCA,Expression,ExprList1}},
        {LeftHandVar,{Var,LeftHandVar1}},
        {LeftHandVar1,{LeftHandPostfixRationalOperator}},
        {LeftHandVar1,{EPSILON}},
        {RightHandVar,{Var,RightHandVar1}},
        {RightHandVar1,{RightHandPostfixRationalOperator}},
        {RightHandVar1,{EPSILON}},
        {BinaryAdditiveOperator,{OPPLUS}},
        {BinaryAdditiveOperator,{OPMINUS}},
        {BinaryMulOperator,{OPMUL}},
        {BinaryMulOperator,{OPDIV}},
        {UnaryOperator,{BinaryAdditiveOperator}},
        {UnaryOperator,{UnaryOperator1}},
        {UnaryOperator1,{RWINTOP}},
        {UnaryOperator1,{RWIR}},
        {UnaryOperator1,{RWSM}},
        {UnaryOperator1,{RWFR}},
        {UnaryOperator1,{RWLN}},
        {UnaryOperator1,{RWIM}},
        {UnaryOperator1,{RWRE}},
        {UnaryAdditiveOperator,{BinaryAdditiveOperator}},
        {BinaryRelationOperator,{OPGT}},
        {BinaryRelationOperator,{OPLT}},
        {BinaryRelationOperator,{OPGE}},
        {BinaryRelationOperator,{OPLE}},
        {BinaryRelationOperator,{OPEQ}},
        {BinaryRelationOperator,{OPNE}},
        {PostfixRationalOperator,{SRSP,PostfixRationalOperator1}},
        {PostfixRationalOperator1,{RWDN}},
        {PostfixRationalOperator1,{RWN}},
        {LeftHandPostfixRationalOperator,{PostfixRationalOperator}},
        {RightHandPostfixRationalOperator,{PostfixRationalOperator}},
        {CompoundStatement,{RWB,Statement,CompoundStatement1}},
        {CompoundStatement1,{SRSM,CompoundStatement2}},
        {CompoundStatement1,{RWEND}},
        {CompoundStatement2,{RWEND}},
        {CompoundStatement2,{SRSM,CompoundStatement2}},
        {CompoundStatement2,{Statement2,CompoundStatement1}},
        {Statement,{EPSILON}},
        {Statement,{Statement2}},
        {Statement2,{ConditionalOperator}},
        {Statement2,{CompoundStatement}},
        {Statement2,{InputOperator}},
        {Statement2,{OutputOperator}},
        {Statement2,{UnconditionalJumpOperator}},
        {Statement2,{LoopWithParameterOperator}},
        {Statement2,{ID,Statement1}},
        {Statement1,{SRCN,Statement}},
        {Statement1,{ExprList,LeftHandVar1,OPAS,Expression}},
        {LoopWithParameterOperator,{RWFOR,ID,OPAS,Expression,LoopDirection,Expression,RWDO,Statement}},
        {LoopDirection,{RWDT}},
        {LoopDirection,{RWTO}},
        {AssignmentOperator,{LeftHandVar,OPAS,Expression}},
        {UnconditionalJumpOperator,{RWGT,Label}},
        {ConditionalOperator,{RWIF,Expression,RWTH,ConditionalOperator1}},
        {ConditionalOperator1,{RWEL,Statement2}},
        {ConditionalOperator1,{Statement2,ConditionalOperator2}},
        {ConditionalOperator2,{RWEL,Statement2}},
        {ConditionalOperator2,{EPSILON}},
        {InputOperator,{RWRD,SRLP,LeftHandVar,InputOperator1,SRRP}},
        {InputOperator1,{SRCA,LeftHandVar}},
        {OutputOperator,{RWWR,SRLP,Expression,OutputOperator1,SRRP}},
        {OutputOperator1,{SRCA,Expression}},
        {OperatorOverloadSection,{RWOP,OperatorOverloadSection1}},
        {OperatorOverloadSection1,{BinaryAdditiveOperator,SRLP,ID,OperatorOverloadSection2}},
        {OperatorOverloadSection1,{OverloadBinaryOperation1,SRLP,ID,BinaryOperatorOverload1,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
        {OperatorOverloadSection1,{UnaryOperator1,SRLP,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,OperatorOverloadSection1,CompoundStatement,SRSM}},
        {OperatorOverloadSection2,{SRCA,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
        {OperatorOverloadSection2,{SRCN,Type,OperatorOverloadSection3}},
        {OperatorOverloadSection3,{SRSM,ID,SRCN,Type,SRRP,ID,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
        {OperatorOverloadSection3,{SRRP,ID,SRCN,Type,SRSM,OperatorOverloadSection1,CompoundStatement,SRSM}},
        {LocalDefinition1,{EPSILON}},
        {LocalDefinition1,{LocalDefinition,OperatorOverloadSection1}},
        {LocalDefinition,{TypeSection}},
        {LocalDefinition,{VarSection}},
        {LocalDefinition,{ConstSection}},
        {LocalDefinition,{LabelSection}},
        {UnaryOperatorOverload,{UnaryOperator,SRLP,ID,SRCN,Type,SRRP}},
        {BinaryOperatorOverload,{OverloadBinaryOperation,SRLP,ID,BinaryOperatorOverload1}},
        {BinaryOperatorOverload1,{SRCA,ID,SRCN,Type,SRRP}},
        {BinaryOperatorOverload1,{SRCN,Type,SRSM,ID,SRCN,Type,SRRP}},
        {OverloadBinaryOperation,{BinaryAdditiveOperator}},
        {OverloadBinaryOperation,{OverloadBinaryOperation1}},
        {OverloadBinaryOperation1,{BinaryMulOperator}},
        {OverloadBinaryOperation1,{RWLO}},
        {OverloadBinaryOperation1,{RWLA}},
        {OverloadBinaryOperation1,{OPAS}},
        {OverloadBinaryOperation1,{RWCM}},
        {Function,{RWFUN,ID,SRLP,FunctionArgs,SRRP,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
        {FunctionArgs,{OneTypeArgs,FunctionArgs1}},
        {FunctionArgs1,{SRSM,FunctionArgs}},
        {FunctionArgs1,{EPSILON}},
        {OneTypeArgs,{ID,OneTypeArgs1}},
        {OneTypeArgs1,{SRCA,ID,IdList1,SRCN,Type}},
        {OneTypeArgs1,{SRCN,Type}},
        {Procedure,{RWPRC,ID,SRLP,FunctionArgs,ResultArgs,SRRP,SRCN,Type,SRSM,LocalDefinition1,CompoundStatement,SRSM}},
        {ResultArgs,{EPSILON}},
        {ResultArgs,{SRSM,RWV,FunctionArgs}}
    };
    Parser::Grammar<TokenType, NonterminalSymbols> grammar(grammar_rules, Program);
    std::cout << grammar.isLL1() << std::endl;
    Parser::SyntaxAnalyzer<TokenType, NonterminalSymbols> syntax_analyzer(grammar);
    int res = syntax_analyzer.readNextToken({ RWPR });
    std::cout << "SyntaxAnalyser result: " << res << std::endl;

    // reserwed words map
    std::vector<boost::bimap<std::string, TokenType>::value_type> reserved_words_map_items =
    {
        { "and",        TokenType::RWLA  },
        { "array",      TokenType::RWAR  },
        { "begin",      TokenType::RWB   },
        { "common",     TokenType::RWCM  },
        { "complex",    TokenType::RWCX  },
        { "const",      TokenType::RWCN  },
        { "do",         TokenType::RWDO  },
        { "denominator",TokenType::RWDN  },
        { "downto",     TokenType::RWDT  },
        { "else",       TokenType::RWEL  },
        { "end",        TokenType::RWEND },
        { "for",        TokenType::RWFOR },
        { "frac",       TokenType::RWFR  },
        { "function",   TokenType::RWFUN },
        { "goto",       TokenType::RWGT  },
        { "if",         TokenType::RWIF  },
        { "im",         TokenType::RWIM  },
        { "int",        TokenType::RWINTOP},
        { "integer",    TokenType::RWINT },
        { "irregular",  TokenType::RWIR  },
        { "im",         TokenType::RWIM  },
        { "j",          TokenType::CJ    },
        { "label",      TokenType::RWLB  },
        { "numerator",  TokenType::RWN   },
        { "not",        TokenType::RWLN  },
        { "of",         TokenType::RWOF  },
        { "operator",   TokenType::RWOP  },
        { "or",         TokenType::RWLO  },
        { "program",    TokenType::RWPR  },
        { "procedure",  TokenType::RWPRC },
        { "rational",   TokenType::RWRAT },
        { "re",         TokenType::RWRE  },
        { "read",       TokenType::RWRD  },
        { "simplify",   TokenType::RWSM  },
        { "then",       TokenType::RWTH  },
        { "to",         TokenType::RWTO  },
        { "type",       TokenType::RWTP  },
        { "var",        TokenType::RWV   },
        { "write",      TokenType::RWWR  }
    };
    boost::bimap<std::string, TokenType> reserved_words(reserved_words_map_items.begin(), reserved_words_map_items.end());

    //rules
    std::vector<Lexer::Lexer<TokenType>::Rule> rules;
    rules.emplace_back(TokenType::OPPLUS,   std::string("+")                     );
    rules.emplace_back(TokenType::OPMINUS,  std::string("-")                     );
    rules.emplace_back(TokenType::OPMUL,    std::string("\\*")                   );
    rules.emplace_back(TokenType::OPDIV,    std::string("/")                     );
    rules.emplace_back(TokenType::OPGT,     std::string(">")                     );
    rules.emplace_back(TokenType::OPLT,     std::string("<")                     );
    rules.emplace_back(TokenType::OPGE,     std::string(">=")                    );
    rules.emplace_back(TokenType::OPLE,     std::string("<=")                    );
    rules.emplace_back(TokenType::OPEQ,     std::string("=")                     );
    rules.emplace_back(TokenType::OPNE,     std::string(":=")                    );
    rules.emplace_back(TokenType::OPAS,     std::string("<>")                    );
    rules.emplace_back(TokenType::SRLP,     std::string("\\(")                   );
    rules.emplace_back(TokenType::SRRP,     std::string("\\)")                   );
    rules.emplace_back(TokenType::SRLB,     std::string("\\[")                   );
    rules.emplace_back(TokenType::SRRB,     std::string("\\]")                   );
    rules.emplace_back(TokenType::SRLB,     std::string("{")                     );
    rules.emplace_back(TokenType::SRRB,     std::string("}")                     );
    rules.emplace_back(TokenType::SRSM,     std::string(";")                     );
    rules.emplace_back(TokenType::SRCN,     std::string(":")                     );
    rules.emplace_back(TokenType::SRSP,     std::string(".")                     );
    rules.emplace_back(TokenType::SRCA,     std::string(",")                     );
    rules.emplace_back(TokenType::CI,       std::string("[1-9][0-9]*")           );
    rules.emplace_back(TokenType::ID,       std::string("[a-zA-Z][a-zA-Z0-9]*"), std::make_shared<IDHandler>());

    Lexer::Lexer<TokenType> lexer( std::move(rules));
    lexer.openInput("/home/alex/Projects/university_compiler/program.pas");
    std::unique_ptr<Lexer::Token<TokenType>> token;
    while ((token = lexer.getToken())->type() != TokenType::ENDOFFILE)
    {
        std::cout << "NEW TOKEN : " << boost::any_cast<std::string>(token->value()) << std::endl;

    }
    return 0;
}
