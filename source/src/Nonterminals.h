#ifndef NONTERMINALS_H
#define NONTERMINALS_H

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
    ConstExpr3, // 117
    ConstSection, // 118
    ConstSection1, // 119
    DescriptionSection, // 120
    DescriptionSection1, // 121
    EnumType, // 122
    ExprList, // 123
    ExprList1, // 124
    Expression, // 125
    Expression1, // 126
    Expression2, // 127
    Factor, // 128
    Factor1, // 129
    Function, // 130
    FunctionArgs, // 131
    FunctionArgs1, // 132
    IdList, // 133
    IdList1, // 134
    IndexType, // 135
    InputOperator, // 136
    InputOperator1, // 137
    Label, // 138
    LabelSection, // 139
    LabelSection1, // 140
    LeftHandPostfixRationalOperator, // 141
    LeftHandVar, // 142
    LeftHandVar1, // 143
    LocalDefinition, // 144
    LocalDefinition1, // 145
    LogicAnd, // 146
    LogicAnd1, // 147
    LogicOr, // 148
    LogicOr1, // 149
    LoopDirection, // 150
    LoopWithParameterOperator, // 151
    Massive, // 152
    Massive1, // 153
    OneTypeArgs, // 154
    OneTypeArgs1, // 155
    OperatorOverloadSection, // 156
    OperatorOverloadSection1, // 157
    OperatorOverloadSection2, // 158
    OperatorOverloadSection3, // 159
    OutputOperator, // 160
    OutputOperator1, // 161
    OverloadBinaryOperation, // 162
    OverloadBinaryOperation1, // 163
    PostfixRationalOperator, // 164
    PostfixRationalOperator1, // 165
    Procedure, // 166
    ProcedureFunctions, // 167
    Program, // 168
    Program1, // 169
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
    Var3, // 195
    Var4, // 196
    VarSection, // 197
    VarSection1, // 198
    VarSection2, // 199
    NONTERMINAL_FINISH_INDEX,
    ACTION_START_INDEX,
    ACTION1,
    ACTION2,
    ACTION_FINISH_INDEX,
    EPSILON
};

#endif // NONTERMINALS_H
