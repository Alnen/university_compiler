#ifndef PASCALRULES_H
#define PASCALRULES_H

#include "Parser/Grammar.h"
#include "Nonterminals.h"
#include "Terminals.h"

using namespace PascalCompiler::Grammar;

Parser::Grammar<TokenType, NonterminalSymbols>::RuleList grammar_rules = {
    {Program, {RWPR, ID, SRSM, GlobalDeclaration, Program1}, TreeConstructor},
    {Program1, {FunctionDeclaration, CompoundStatement, SRSP}, FinishMainFuntion},
    {Program1, {ProcedureDeclaration, CompoundStatement, SRSP}, FinishMainFuntion},
    {Program1, {CompoundStatement, SRSP}, FinishMainFuntion},

// GLOBAL DECLARATION
    {GlobalDeclaration, {EPSILON}, TreeConstructor},
    {GlobalDeclaration, {GlobalDeclaration1, GlobalDeclaration}, TreeConstructor},
    {GlobalDeclaration1, {LabelSection}, TreeConstructor},
    {GlobalDeclaration1, {ConstSection}, TreeConstructor},
    {GlobalDeclaration1, {TypeSection}, TreeConstructor},
    {GlobalDeclaration1, {VarSection}, TreeConstructor},
    {GlobalDeclaration1, {OperatorOverloadSection}, TreeConstructor},

// LABEL SECTION (DONE)
    {LabelSection, {RWLB, Label, LabelSection1, SRSM}, LabelRegistration},
    {LabelSection1, {EPSILON}, TreeConstructor},
    {LabelSection1, {SRCA, Label, LabelSection1}, LabelRegistration},
    {Label, {ID}, LabelID},

// CONST SECTION (NOT SUPPORTED)
    {ConstSection, {RWCN, ConstDefinition, SRSM, ConstSection1}, TreeConstructor},
    {ConstSection1, {EPSILON}, TreeConstructor},
    {ConstSection1, {ConstDefinition, SRSM, ConstSection1}, TreeConstructor},
    {ConstDefinition, {ID, OPEQ, Expression}, TreeConstructor}, // Expression => ConstExpression

// TYPE SECTION (DONE)
    {TypeSection, {RWTP, TypeDefinition, SRSM, TypeSection1}, TreeConstructor},
    {TypeSection1, {EPSILON}, TreeConstructor},
    {TypeSection1, {TypeDefinition, SRSM, TypeSection1}, TreeConstructor},
    {TypeDefinition, {ID, OPEQ, Type}, TypeRegistrationAction},
    {Type, {BuiltInType}, TypePropagation},
    {Type, {Massive}, TypePropagation},
    {Type, {EnumType}, TypePropagation},
    {Type, {RecordType}, TypePropagation}, // TODO RECORD
    {Type, {ID}, GetTypeAccordingToID},
    {DefiniteType, {BuiltInType}, TypePropagation},
    {DefiniteType, {ID}, GetTypeAccordingToID},
    {BuiltInType, {RWINT}, IntegerPass},
    {BuiltInType, {RWRAT}, RationalPass},
    {BuiltInType, {RWCX}, ComplexPass},
    {Massive, {RWAR, SRLB, Index, IndexList, SRRB, RWOF, DefiniteType}, ArrayCreationAction},
    {IndexList, {EPSILON}, IndexListInit},
    {IndexList, {SRCA, Index, IndexList}, IndexListAppendAtStart},
    {Index, {CI}, IndexTypeCXSize},
    {Index, {ID}, IndexTypeIDCase},
    {EnumType, {SRLP, IdList, SRRP}, EnumTypeConstruction},
    {RecordType, {RWRC, RecordFieldDefinition, SRSM, RecordFieldDefinition1, RWEND}, TreeConstructor}, // TODO RECORD
    {RecordFieldDefinition, {IdList, SRCN, Type}, TreeConstructor},
    {RecordFieldDefinition1, {EPSILON}, TreeConstructor}, // TODO RECORD
    {RecordFieldDefinition1, {RecordFieldDefinition, SRSM, RecordFieldDefinition1}, TreeConstructor}, // TODO RECORD

// BUILD-IN TYPE CONSTANTS
    {IntegerConst, {CI}, TreeConstructor},
    {RationalConst, {CI, SRSP, CI}, TreeConstructor},
    {ComplexConst, {ComplexConstPart, CJ}, TreeConstructor},
    {ComplexConst, {SRLCB, ComplexConstPart, SRCA, ComplexConstPart, SRRCB}, TreeConstructor},

// VAR (DONE)
    {VarSection, {RWV, VarSection1, VarSection2}, TreeConstructor},
    {VarSection1, {IdList, SRCN, Type, SRSM}, VariableDeclarationAction},
    {VarSection2, {VarSection1, VarSection2}, TreeConstructor},
    {VarSection2, {EPSILON}, TreeConstructor},

// ID LIST (DONE)
    {IdList, {ID, IdList1}, FinalAppendList},
    {IdList1, {SRCA, ID, IdList1}, AppendIdList},
    {IdList1, {EPSILON}, InitIdList},

// EXPRESSION
    {Expression, {LogicOr}, ValuePropagation},
    {LogicOr, {LogicAnd, LogicOr1}, ExpressionCombiner},
    {LogicOr1, {BinaryLogicOperatorOr, LogicAnd, LogicOr1}, SuperExpressionCombiner},
    {LogicOr1, {EPSILON}, TreeConstructor},

    {LogicAnd, {Relation, LogicAnd1}, ExpressionCombiner},
    {LogicAnd1, {BinaryLogicOperatorAnd, Relation, LogicAnd1}, SuperExpressionCombiner},
    {LogicAnd1, {EPSILON}, TreeConstructor},

    {Relation, {AdditiveExpression, Relation1}, ExpressionCombiner},
    {Relation1, {BinaryRelationOperator, AdditiveExpression, Relation1}, SuperExpressionCombiner},
    {Relation1, {EPSILON}, TreeConstructor},

    {AdditiveExpression, {MulExpression, AdditiveExpression1}, ExpressionCombiner},
    {AdditiveExpression1, {BinaryAdditiveOperator, MulExpression, AdditiveExpression1}, SuperExpressionCombiner},
    {AdditiveExpression1, {EPSILON}, TreeConstructor},

    {MulExpression, {GcdExpression, MulExpression1}, ExpressionCombiner},
    {MulExpression1, {BinaryMulOperator, GcdExpression, MulExpression1}, SuperExpressionCombiner},
    {MulExpression1, {EPSILON}, TreeConstructor},

    {GcdExpression, {Factor, GcdExpression1}, ExpressionCombiner},
    {GcdExpression1, {RWCM, Factor, GcdExpression1}, SuperExpressionCombiner},
    {GcdExpression1, {EPSILON}, TreeConstructor},

    {Factor, {UnaryOperator, UnsignedFactor}, UnaryOPValue},
    {Factor, {UnsignedFactor}, ValuePropagation},
    {Factor, {SRLP, LogicOr, SRRP}, BracketValuePropagation},

    {UnsignedFactor, {RightHandVar}, ValuePropagation},
    {UnsignedFactor, {NumericConstant}, TreeConstructor},

    {NumericConstant, {CI, NumericConstant1}, CreateConstComplex1},
    {NumericConstant, {SRLCB, ComplexConstPart, SRCA, ComplexConstPart, SRRCB}, CreateConstComplex3},
    {NumericConstant1, {EPSILON}, TreeConstructor},
    {NumericConstant1, {CJ}, JFlagTrue},
    {NumericConstant1, {SRSP, CI, NumericConstant2}, CreateConstComplex2},
    {NumericConstant2, {EPSILON}, TreeConstructor},
    {NumericConstant2, {CJ}, JFlagTrue},

    {ComplexConstPart, {CI, ComplexConstPart1}, TreeConstructor},
    {ComplexConstPart1, {EPSILON}, TreeConstructor},
    {ComplexConstPart1, {SRSP, CI}, TreeConstructor},

    {VarHandlerList, {SRLB, Expression, SRRB, VarHandlerList}, AppendIndexVarHandlerList},
    {VarHandlerList, {SRSP, ID, VarHandlerList}, AppendMemberVarHandlerList},
    {VarHandlerList, {EPSILON}, InitVarHandlerList},

    {LeftHandVar, {ID, VarHandlerList}, PropagateLeftHandVarInfo},
    {RightHandVar, {ID, VarHandlerList}, LoadRightHandVar},

// OPERATORS
    {BinaryAdditiveOperator, {OPPLUS}, UnaryOperatorOPPLUS},
    {BinaryAdditiveOperator, {OPMINUS}, UnaryOperatorOPMINUS},
    {BinaryMulOperator, {OPMUL}, UnaryOperatorOPMUL},
    {BinaryMulOperator, {OPDIV}, UnaryOperatorOPDIV},

    {UnaryOperator, {BinaryAdditiveOperator}, OPPropagation},
    {UnaryOperator, {UnaryOperator1}, OPPropagation},
    {UnaryOperator1, {RWINTOP}, UnaryOperatorRWINTOP},
    {UnaryOperator1, {RWIR}, UnaryOperatorRWIR},
    {UnaryOperator1, {RWSM}, UnaryOperatorRWSM},
    {UnaryOperator1, {RWFR}, UnaryOperatorRWFR},
    {UnaryOperator1, {RWLN}, UnaryOperatorRWLN},
    {UnaryOperator1, {RWIM}, UnaryOperatorRWIM},
    {UnaryOperator1, {RWRE}, UnaryOperatorRWRE},
    {UnaryAdditiveOperator, {BinaryAdditiveOperator}, OPPropagation},

    {BinaryRelationOperator, {OPGT}, BinaryRelationOperatorOPGT},
    {BinaryRelationOperator, {OPLT}, BinaryRelationOperatorOPLT},
    {BinaryRelationOperator, {OPGE}, BinaryRelationOperatorOPGE},
    {BinaryRelationOperator, {OPLE}, BinaryRelationOperatorOPLE},
    {BinaryRelationOperator, {OPEQ}, BinaryRelationOperatorOPEQ},
    {BinaryRelationOperator, {OPNE}, BinaryRelationOperatorOPNE},
    {BinaryLogicOperatorAnd, {RWLA}, BinaryLogicOperatorRWLA},
    {BinaryLogicOperatorOr, {RWLO}, BinaryLogicOperatorRWLO},

// STATEMENT
    {CompoundStatement, {RWB, Statement, CompoundStatement1}, TreeConstructor},
    {CompoundStatement1, {SRSM, CompoundStatement2}, TreeConstructor},
    {CompoundStatement1, {RWEND}, TreeConstructor},
    {CompoundStatement2, {RWEND}, TreeConstructor},
    {CompoundStatement2, {SRSM, CompoundStatement2}, TreeConstructor},
    {CompoundStatement2, {Statement2, CompoundStatement1}, TreeConstructor},

    {Statement, {EPSILON}, TreeConstructor},
    {Statement, {Statement2}, TreeConstructor},
    {Statement2, {ConditionalOperator}, TreeConstructor},
    {Statement2, {CompoundStatement}, TreeConstructor},
    {Statement2, {InputOperator}, TreeConstructor},
    {Statement2, {OutputOperator}, TreeConstructor},
    {Statement2, {UnconditionalJumpOperator}, TreeConstructor},
    {Statement2, {LoopWithParameterOperator}, TreeConstructor},
    {Statement2, {ID, IDPass, Statement1}, LabelOrAssignmentAction},
    {Statement45, {EPSILON}, LabelStatement},
    {Statement1, {IDPass2, Statement45, SRCN, Statement}, LabelAction},
    {Statement1, {VarHandlerList,OPAS,Expression}, AssignmentAction},
    {LoopWithParameterOperator, {RWFOR,ID,OPAS,Expression,LoopDirection,Expression,RWDO, CreateBlock, Statement}, LoopWithParameterAction},
    {LoopDirection, {RWDT}, LoopDirectionDT},
    {LoopDirection, {RWTO}, LoopDirectionTO},
    {UnconditionalJumpOperator, {RWGT,Label}, GotoLabel},
    {ConditionalOperator, {RWIF,Expression,RWTH, CreateBlock, Statement2,ConditionalOperator1}, ConditionalOperatorAction},
    {ConditionalOperator1, {EPSILON}, ConditionalOperatorNoElseAction},
    {ConditionalOperator1, {RWEL, CreateBlock, Statement2}, ConditionalOperatorElseAction},
    {InputOperator, {RWRD,SRLP,LeftHandVar,SRRP}, ReadLeftHandVarAction},
    {OutputOperator, {RWWR, SRLP, TEXT, ExprList1,SRRP}, PrintOperatorMain},
    //{OutputOperator1, {SRCA,Expression}, TreeConstructor},
    {CreateBlock, {EPSILON}, CreateBlockAction},

    {ExprList, {Expression, ExprList1}, FinalAppendExprList},
    {ExprList1, {EPSILON}, InitExprList},
    {ExprList1, {SRCA, Expression, ExprList1}, AppendExprList},

 // OPERATOR OVERLOADING
    {OperatorOverloadSection, {RWOP, OverloadOperator, ID, SRCN, Type, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {OverloadOperator, {BinaryAdditiveOperator, SRLP, ID, OverloadOperator1}, TreeConstructor},
    {OverloadOperator, {OverloadBinaryOperation1, SRLP, ID, BinaryOperatorOverload1}, TreeConstructor},
    {OverloadOperator, {UnaryOperator1, SRLP, ID, SRCN, Type, SRRP}, TreeConstructor},
    {OverloadOperator1, {BinaryOperatorOverload1}, TreeConstructor},
    {OverloadOperator1, {SRCN, Type, SRRP}, TreeConstructor},

    {LocalDeclaration, {EPSILON}, TreeConstructor},
    {LocalDeclaration, {LocalDeclaration1, LocalDeclaration}, TreeConstructor},
    {LocalDeclaration1, {LabelSection}, TreeConstructor},
    {LocalDeclaration1, {ConstSection}, TreeConstructor},
    {LocalDeclaration1, {VarSection}, TreeConstructor},

    {UnaryOperatorOverload, {UnaryOperator, SRLP, ID, SRCN, Type, SRRP}, TreeConstructor},
    {BinaryOperatorOverload, {OverloadBinaryOperation, SRLP, ID, BinaryOperatorOverload1}, TreeConstructor},
    {BinaryOperatorOverload1, {SRCA, ID, SRCN, Type, SRRP}, TreeConstructor},
    {BinaryOperatorOverload1, {SRCN, Type, SRSM, ID, SRCN, Type, SRRP}, TreeConstructor},

    {OverloadBinaryOperation, {BinaryAdditiveOperator}, TreeConstructor},
    {OverloadBinaryOperation, {OverloadBinaryOperation1}, TreeConstructor},
    {OverloadBinaryOperation1, {BinaryMulOperator}, TreeConstructor},
    {OverloadBinaryOperation1, {RWLO}, TreeConstructor},
    {OverloadBinaryOperation1, {RWLA}, TreeConstructor},
    {OverloadBinaryOperation1, {OPAS}, TreeConstructor},
    {OverloadBinaryOperation1, {RWCM}, TreeConstructor},

// FUNCTIONS AND PROCEDURES
    {FunctionDeclaration, {RWFUN, ID, SRLP, FunctionParameters, SRRP, SRCN, Type, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {FunctionParameters, {EPSILON}, TreeConstructor},
    {FunctionParameters, {OneTypeParameters, FunctionParameters1}, TreeConstructor},
    {FunctionParameters1, {EPSILON}, TreeConstructor},
    {FunctionParameters1, {SRSM, OneTypeParameters, FunctionParameters1}, TreeConstructor},
    {OneTypeParameters, {IdList, SRCN, Type}, TreeConstructor},
    {FunctionArgs, {EPSILON}, TreeConstructor},
    {FunctionArgs, {ExprList}, TreeConstructor},
    {FunctionCall, {ID, SRLP, FunctionArgs, SRRP}, TreeConstructor},

    {Procedure, {RWPRC, ID, SRLP, ProcedureParameters, SRRP, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {ProcedureParameters, {EPSILON}, TreeConstructor},
    {ProcedureParameters, {ResultParameters}, TreeConstructor},
    {ProcedureParameters, {OneTypeParameters, FunctionParameters1, ResultParameters1}, TreeConstructor},
    {ResultParameters, {RWV, FunctionParameters}, TreeConstructor},
    {ResultParameters1, {EPSILON}, TreeConstructor},
    {ResultParameters1, {ResultParameters}, TreeConstructor},
};

#endif // #define PASCALRULES_H
