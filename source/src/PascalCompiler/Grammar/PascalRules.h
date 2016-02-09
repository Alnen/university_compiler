#ifndef PASCALRULES_H
#define PASCALRULES_H

#include "Parser/Grammar.h"
#include "Nonterminals.h"
#include "Terminals.h"

using namespace PascalCompiler::Grammar;

Parser::Grammar<TokenType, NonterminalSymbols>::RuleList grammar_rules = {
    {Program, {RWPR, ID, SRSM, GlobalDeclaration, CompoundStatement, SRSP}, FinishMainFuntion},

// GLOBAL DECLARATION
    {GlobalDeclaration, {EPSILON}, TreeConstructor},
    {GlobalDeclaration, {NotEmptyGlobalDeclaration, GlobalDeclaration}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {LabelSection}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {ConstSection}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {TypeSection}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {VarSection}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {OperatorOverloadSection}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {FunctionDeclaration}, TreeConstructor},
    {NotEmptyGlobalDeclaration, {ProcedureDeclaration}, TreeConstructor},

// LABEL SECTION (DONE)
    {LabelSection, {RWLB, Label, NextLabel, SRSM}, LabelRegistration},
    {NextLabel, {EPSILON}, TreeConstructor},
    {NextLabel, {SRCA, Label, NextLabel}, LabelRegistration},
    {Label, {ID}, LabelID},

// CONST SECTION (NOT SUPPORTED)
    {ConstSection, {RWCN, ConstDefinition, SRSM, NextConstDefinition}, TreeConstructor},
    {NextConstDefinition, {EPSILON}, TreeConstructor},
    {NextConstDefinition, {ConstDefinition, SRSM, NextConstDefinition}, TreeConstructor},
    {ConstDefinition, {ID, OPEQ, Expression}, TreeConstructor}, // Expression => ConstExpression

// TYPE SECTION (DONE)
    {TypeSection, {RWTP, TypeDefinition, SRSM, NextTypeDefinition}, TreeConstructor},
    {NextTypeDefinition, {EPSILON}, TreeConstructor},
    {NextTypeDefinition, {TypeDefinition, SRSM, NextTypeDefinition}, TreeConstructor},
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
    {Massive, {RWAR, SRLB, IndexList, SRRB, RWOF, DefiniteType}, ArrayCreationAction},
    {IndexList, {EPSILON}, TreeConstructor},
    {IndexList, {NotEmptyIndexList}, TreeConstructor},
    {NotEmptyIndexList, {ID, IdListTail}, FinalAppendList},
    {IndexListTail, {EPSILON}, IndexListInit},
    {IndexListTail, {SRCA, Index, IndexListTail}, IndexListAppendAtStart},
    {Index, {CI}, IndexTypeCXSize}, // TODO: Index -> Expression (with var = CI | ID)
    {Index, {ID}, IndexTypeIDCase},
    {EnumType, {SRLP, NotEmptyIdList, SRRP}, EnumTypeConstruction},
    {RecordType, {RWRC, RecordFieldDefinition, SRSM, NextRecordFieldDefinition, RWEND}, TreeConstructor}, // TODO RECORD
    {RecordFieldDefinition, {NotEmptyIdList, SRCN, Type}, TreeConstructor},
    {NextRecordFieldDefinition, {EPSILON}, TreeConstructor}, // TODO RECORD
    {NextRecordFieldDefinition, {RecordFieldDefinition, SRSM, NextRecordFieldDefinition}, TreeConstructor}, // TODO RECORD

// VAR (DONE)
    {VarSection, {RWV, VarDefinition, NextVarDefinition}, TreeConstructor},
    {VarDefinition, {NotEmptyIdList, SRCN, Type, SRSM}, VariableDeclarationAction},
    {NextVarDefinition, {VarDefinition, NextVarDefinition}, TreeConstructor},
    {NextVarDefinition, {EPSILON}, TreeConstructor},

// ID LIST (DONE)
    {IdList, {EPSILON}, TreeConstructor},
    {IdList, {NotEmptyIdList}, TreeConstructor},
    {NotEmptyIdList, {ID, IdListTail}, FinalAppendList},
    {IdListTail, {SRCA, ID, IdListTail}, AppendIdList},
    {IdListTail, {EPSILON}, InitIdList},

// EXPRESSION
    {Expression, {LogicOr}, ValuePropagation},
    {LogicOr, {LogicAnd, NextLogicOr}, ExpressionCombiner},
    {NextLogicOr, {BinaryLogicOperatorOr, LogicAnd, NextLogicOr}, SuperExpressionCombiner},
    {NextLogicOr, {EPSILON}, TreeConstructor},

    {LogicAnd, {RelationalExpression, NextLogicAnd}, ExpressionCombiner},
    {NextLogicAnd, {BinaryLogicOperatorAnd, RelationalExpression, NextLogicAnd}, SuperExpressionCombiner},
    {NextLogicAnd, {EPSILON}, TreeConstructor},

    {RelationalExpression, {AdditiveExpression, NextRelationalExpression}, ExpressionCombiner},
    {NextRelationalExpression, {BinaryRelationOperator, AdditiveExpression, NextRelationalExpression}, SuperExpressionCombiner},
    {NextRelationalExpression, {EPSILON}, TreeConstructor},

    {AdditiveExpression, {MulExpression, NextAdditiveExpression}, ExpressionCombiner},
    {NextAdditiveExpression, {BinaryAdditiveOperator, MulExpression, NextAdditiveExpression}, SuperExpressionCombiner},
    {NextAdditiveExpression, {EPSILON}, TreeConstructor},

    {MulExpression, {GcdExpression, NextMulExpression}, ExpressionCombiner},
    {NextMulExpression, {BinaryMulOperator, GcdExpression, NextMulExpression}, SuperExpressionCombiner},
    {NextMulExpression, {EPSILON}, TreeConstructor},

    {GcdExpression, {Factor, NextGcdExpression}, ExpressionCombiner},
    {NextGcdExpression, {RWCM, Factor, NextGcdExpression}, SuperExpressionCombiner},
    {NextGcdExpression, {EPSILON}, TreeConstructor},

    {Factor, {UnaryOperator, UnsignedFactor}, UnaryOPValue},
    {Factor, {UnsignedFactor}, ValuePropagation},
    {Factor, {SRLP, LogicOr, SRRP}, BracketValuePropagation},

    {UnsignedFactor, {RightHandVar}, ValuePropagation},
    {UnsignedFactor, {NumericConstant}, TreeConstructor},
    //{UnsignedFactor, {ID, SRLP, ExprList, SRRP}, TreeConstructor}, // TODO: FunctionCall

    {NumericConstant, {CI, RationalComplexPart}, CreateConstComplex1},
    {NumericConstant, {SRLCB, ComplexNumberPart, SRCA, ComplexNumberPart, SRRCB}, CreateConstComplex3},

    {RationalComplexPart, {EPSILON}, TreeConstructor},
    {RationalComplexPart, {CJ}, JFlagTrue},
    {RationalComplexPart, {SRSP, CI, ComplexNumberSuffix}, CreateConstComplex2},
    {ComplexNumberSuffix, {EPSILON}, TreeConstructor},
    {ComplexNumberSuffix, {CJ}, JFlagTrue},

    {ComplexNumberPart, {CI, RationalNumberPart}, TreeConstructor},
    {RationalNumberPart, {EPSILON}, TreeConstructor},
    {RationalNumberPart, {SRSP, CI}, TreeConstructor},

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
    {UnaryOperator, {UnaryNotAdditiveOperator}, OPPropagation},
    {UnaryNotAdditiveOperator, {RWINTOP}, UnaryOperatorRWINTOP},
    {UnaryNotAdditiveOperator, {RWIR}, UnaryOperatorRWIR},
    {UnaryNotAdditiveOperator, {RWSM}, UnaryOperatorRWSM},
    {UnaryNotAdditiveOperator, {RWFR}, UnaryOperatorRWFR},
    {UnaryNotAdditiveOperator, {RWLN}, UnaryOperatorRWLN},
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
    {CompoundStatement, {RWB, Statement, StatementList, CompoundStatementEnd}, TreeConstructor},
    {CompoundStatementEnd, {RWEND}, TreeConstructor},
    {CompoundStatementEnd, {SRSM, RWEND}, TreeConstructor},
    {StatementList, {EPSILON}, TreeConstructor},
    {StatementList, {SRSM, Statement, StatementList}, TreeConstructor},

    {Statement, {EPSILON}, TreeConstructor},
    {Statement, {NotEmptyStatement}, TreeConstructor},
    {NotEmptyStatement, {CompoundStatement}, TreeConstructor},
    {NotEmptyStatement, {ConditionalOperator}, TreeConstructor},
    {NotEmptyStatement, {InputOperator}, TreeConstructor},
    {NotEmptyStatement, {OutputOperator}, TreeConstructor},
    {NotEmptyStatement, {UnconditionalJumpOperator}, TreeConstructor},
    {NotEmptyStatement, {LoopWithParameterOperator}, TreeConstructor},
    {NotEmptyStatement, {ID, IDPass, LabelOrAssignmentStatement}, LabelOrAssignmentAction},

    {LabelOrAssignmentStatement, {VarHandlerList, OPAS, Expression}, AssignmentAction},
    {LabelOrAssignmentStatement, {IDPass2, EmptyStatement, SRCN, Statement}, LabelAction},
    {EmptyStatement, {EPSILON}, LabelStatement},

    {LoopWithParameterOperator, {RWFOR, ID, OPAS, Expression, LoopDirection, Expression, RWDO, CreateBlock, Statement}, LoopWithParameterAction},
    {LoopDirection, {RWDT}, LoopDirectionDT},
    {LoopDirection, {RWTO}, LoopDirectionTO},
    {UnconditionalJumpOperator, {RWGT, Label}, GotoLabel},
    {AssignmentOperator, {LeftHandVar, OPAS, Expression}, TreeConstructor},
    {ConditionalOperator, {RWIF, Expression, RWTH, CreateBlock, NotEmptyStatement, ConditionalOperatorEnd}, ConditionalOperatorAction},
    {ConditionalOperatorEnd, {EPSILON}, ConditionalOperatorNoElseAction},
    {ConditionalOperatorEnd, {RWEL, CreateBlock, NotEmptyStatement}, ConditionalOperatorElseAction},
    {InputOperator, {RWRD, SRLP, LeftHandVar, SRRP}, ReadLeftHandVarAction},
    {OutputOperator, {RWWR, SRLP, TEXT, ExprListTail, SRRP}, PrintOperatorMain},
    {CreateBlock, {EPSILON}, CreateBlockAction},

    {ExprList, {EPSILON}, FinalAppendExprList},
    {ExprList, {NotEmptyExprList}, FinalAppendExprList},
    {NotEmptyExprList, {Expression, ExprListTail}, FinalAppendExprList},
    {ExprListTail, {EPSILON}, InitExprList},
    {ExprListTail, {SRCA, Expression, ExprListTail}, AppendExprList},

 // OPERATOR OVERLOADING
    {OperatorOverloadSection, {RWOP, OverloadOperator, ID, SRCN, Type, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {OverloadOperator, {BinaryAdditiveOperator, SRLP, ID, OverloadOperatorEnd}, TreeConstructor},
    {OverloadOperator, {BinaryNotAdditiveOperator, SRLP, ID, BinaryOperatorOverloadEnd}, TreeConstructor},
    {OverloadOperator, {UnaryNotAdditiveOperator, SRLP, ID, SRCN, Type, SRRP}, TreeConstructor},
    {OverloadOperatorEnd, {BinaryOperatorOverloadEnd}, TreeConstructor},
    {OverloadOperatorEnd, {SRCN, Type, SRRP}, TreeConstructor},

    {LocalDeclaration, {EPSILON}, TreeConstructor},
    {LocalDeclaration, {NotEmptyLocalDeclaration, LocalDeclaration}, TreeConstructor},
    {NotEmptyLocalDeclaration, {LabelSection}, TreeConstructor},
    {NotEmptyLocalDeclaration, {ConstSection}, TreeConstructor},
    {NotEmptyLocalDeclaration, {VarSection}, TreeConstructor},

    {UnaryOperatorOverload, {UnaryOperator, SRLP, ID, SRCN, Type, SRRP}, TreeConstructor},
    {BinaryOperatorOverload, {OverloadBinaryOperator, SRLP, ID, BinaryOperatorOverloadEnd}, TreeConstructor},
    {BinaryOperatorOverloadEnd, {SRCA, ID, SRCN, Type, SRRP}, TreeConstructor},
    {BinaryOperatorOverloadEnd, {SRCN, Type, SRSM, ID, SRCN, Type, SRRP}, TreeConstructor},

    {OverloadBinaryOperator, {BinaryAdditiveOperator}, TreeConstructor},
    {OverloadBinaryOperator, {BinaryNotAdditiveOperator}, TreeConstructor},
    {BinaryNotAdditiveOperator, {BinaryMulOperator}, TreeConstructor},
    {BinaryNotAdditiveOperator, {RWLO}, TreeConstructor},
    {BinaryNotAdditiveOperator, {RWLA}, TreeConstructor},
    {BinaryNotAdditiveOperator, {OPAS}, TreeConstructor},
    {BinaryNotAdditiveOperator, {RWCM}, TreeConstructor},

// FUNCTIONS AND PROCEDURES
    {FunctionDeclaration, {RWFUN, ID, SRLP, FunctionParameters, SRRP, SRCN, Type, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {FunctionParameters, {EPSILON}, TreeConstructor},
    {FunctionParameters, {OneTypeParameters, FunctionParametersTail}, TreeConstructor},
    {FunctionParametersTail, {EPSILON}, TreeConstructor},
    {FunctionParametersTail, {SRSM, OneTypeParameters, FunctionParametersTail}, TreeConstructor},
    {OneTypeParameters, {NotEmptyIdList, SRCN, Type}, TreeConstructor},
    {FunctionCall, {ID, SRLP, ExprList, SRRP}, TreeConstructor},

    {ProcedureDeclaration, {RWPRC, ID, SRLP, ProcedureParameters, SRRP, SRSM, LocalDeclaration, CompoundStatement, SRSM}, TreeConstructor},
    {ProcedureParameters, {EPSILON}, TreeConstructor},
    {ProcedureParameters, {NotEmptyResultParameters}, TreeConstructor},
    {ProcedureParameters, {OneTypeParameters, FunctionParametersTail, ResultParameters}, TreeConstructor},
    {NotEmptyResultParameters, {RWV, FunctionParameters}, TreeConstructor},
    {ResultParameters, {EPSILON}, TreeConstructor},
    {ResultParameters, {NotEmptyResultParameters}, TreeConstructor}
};

#endif // #define PASCALRULES_H
