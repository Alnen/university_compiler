#include "Nonterminals.h"

namespace PascalCompiler {
namespace Grammar {

boost::container::flat_map<NonterminalSymbols, std::string>& getNonterminalTypeMapping()
{
    static boost::container::flat_map<NonterminalSymbols, std::string> nonterminalTypeMapping;
    static bool first_time = true;
    if (first_time)
    {
        std::vector<boost::container::flat_map<NonterminalSymbols, std::string>::value_type> nonterminalTypePairsMapping =
        {
            { NonterminalSymbols::NONTERMINAL_START_INDEX,                 "NONTERMINAL_START_INDEX"                }, // 99,
            { NonterminalSymbols::AssignmentOperator,                      "AssignmentOperator"                     }, // 100
            { NonterminalSymbols::BinaryAdditiveOperator,                  "BinaryAdditiveOperator"                 }, // 101
            { NonterminalSymbols::BinaryMulOperator,                       "BinaryMulOperator"                      }, // 102
            { NonterminalSymbols::BinaryOperatorOverload,                  "BinaryOperatorOverload"                 }, // 103
            { NonterminalSymbols::BinaryOperatorOverload1,                 "BinaryOperatorOverload1"                }, // 104
            { NonterminalSymbols::BinaryRelationOperator,                  "BinaryRelationOperator"                 }, // 105
            { NonterminalSymbols::CompoundStatement,                       "CompoundStatement"                      }, // 106
            { NonterminalSymbols::CompoundStatement1,                      "CompoundStatement1"                     }, // 107
            { NonterminalSymbols::CompoundStatement2,                      "CompoundStatement2"                     }, // 108
            { NonterminalSymbols::ConcreteType,                            "ConcreteType"                           }, // 109
            { NonterminalSymbols::ConditionalOperator,                     "ConditionalOperator"                    }, // 110
            { NonterminalSymbols::ConditionalOperator1,                    "ConditionalOperator1"                   }, // 111
            { NonterminalSymbols::ConstDefinition,                         "ConstDefinition"                        }, // 113
            { NonterminalSymbols::ConstExpr,                               "ConstExpr"                              }, // 114
            { NonterminalSymbols::ConstExpr1,                              "ConstExpr1"                             }, // 115
            { NonterminalSymbols::ConstExpr2,                              "ConstExpr2"                             }, // 116
            { NonterminalSymbols::ConstExpr3,                              "ConstExpr3"                             }, // 117
            { NonterminalSymbols::ConstSection,                            "ConstSection"                           }, // 118
            { NonterminalSymbols::ConstSection1,                           "ConstSection1"                          }, // 119
            { NonterminalSymbols::DescriptionSection,                      "DescriptionSection"                     }, // 120
            { NonterminalSymbols::DescriptionSection1,                     "DescriptionSection1"                    }, // 121
            { NonterminalSymbols::EnumType,                                "EnumType"                               }, // 122
            { NonterminalSymbols::ExprList,                                "ExprList"                               }, // 123
            { NonterminalSymbols::ExprList1,                               "ExprList1"                              }, // 124
            { NonterminalSymbols::Expression,                              "Expression"                             }, // 125
            { NonterminalSymbols::Expression1,                             "Expression1"                            }, // 126
            { NonterminalSymbols::Expression2,                             "Expression2"                            }, // 127
            { NonterminalSymbols::Factor,                                  "Factor"                                 }, // 128
            { NonterminalSymbols::Factor1,                                 "Factor1"                                }, // 129
            { NonterminalSymbols::Function,                                "Function"                               }, // 130
            { NonterminalSymbols::FunctionArgs,                            "FunctionArgs"                           }, // 131
            { NonterminalSymbols::FunctionArgs1,                           "FunctionArgs1"                          }, // 132
            { NonterminalSymbols::IdList,                                  "IdList"                                 }, // 133
            { NonterminalSymbols::IdList1,                                 "IdList1"                                }, // 134
            { NonterminalSymbols::IndexType,                               "IndexType"                              }, // 135
            { NonterminalSymbols::InputOperator,                           "InputOperator"                          }, // 136
            { NonterminalSymbols::InputOperator1,                          "InputOperator1"                         }, // 137
            { NonterminalSymbols::Label,                                   "Label"                                  }, // 138
            { NonterminalSymbols::LabelSection,                            "LabelSection"                           }, // 139
            { NonterminalSymbols::LabelSection1,                           "LabelSection1"                          }, // 140
            { NonterminalSymbols::LeftHandPostfixRationalOperator,         "LeftHandPostfixRationalOperator"        }, // 141
            { NonterminalSymbols::LeftHandVar,                             "LeftHandVar"                            }, // 142
            { NonterminalSymbols::LeftHandVar1,                            "LeftHandVar1"                           }, // 143
            { NonterminalSymbols::LocalDefinition,                         "LocalDefinition"                        }, // 144
            { NonterminalSymbols::LocalDefinition1,                        "LocalDefinition1"                       }, // 145
            { NonterminalSymbols::LogicAnd,                                "LogicAnd"                               }, // 146
            { NonterminalSymbols::LogicAnd1,                               "LogicAnd1"                              }, // 147
            { NonterminalSymbols::LogicOr,                                 "LogicOr"                                }, // 148
            { NonterminalSymbols::LogicOr1,                                "LogicOr1"                               }, // 149
            { NonterminalSymbols::LoopDirection,                           "LoopDirection"                          }, // 150
            { NonterminalSymbols::LoopWithParameterOperator,               "LoopWithParameterOperator"              }, // 151
            { NonterminalSymbols::Massive,                                 "Massive"                                }, // 152
            { NonterminalSymbols::Massive1,                                "Massive1"                               }, // 153
            { NonterminalSymbols::OneTypeArgs,                             "OneTypeArgs"                            }, // 154
            { NonterminalSymbols::OneTypeArgs1,                            "OneTypeArgs1"                           }, // 155
            { NonterminalSymbols::OperatorOverloadSection,                 "OperatorOverloadSection"                }, // 156
            { NonterminalSymbols::OperatorOverloadSection1,                "OperatorOverloadSection1"               }, // 157
            { NonterminalSymbols::OperatorOverloadSection2,                "OperatorOverloadSection2"               }, // 158
            { NonterminalSymbols::OperatorOverloadSection3,                "OperatorOverloadSection3"               }, // 159
            { NonterminalSymbols::OutputOperator,                          "OutputOperator"                         }, // 160
            { NonterminalSymbols::OutputOperator1,                         "OutputOperator1"                        }, // 161
            { NonterminalSymbols::OverloadBinaryOperation,                 "OverloadBinaryOperation"                }, // 162
            { NonterminalSymbols::OverloadBinaryOperation1,                "OverloadBinaryOperation1"               }, // 163
            { NonterminalSymbols::PostfixRationalOperator,                 "PostfixRationalOperator"                }, // 164
            { NonterminalSymbols::PostfixRationalOperator1,                "PostfixRationalOperator1"               }, // 165
            { NonterminalSymbols::Procedure,                               "Procedure"                              }, // 166
            { NonterminalSymbols::ProcedureFunctions,                      "ProcedureFunctions"                     }, // 167
            { NonterminalSymbols::Program,                                 "Program"                                }, // 168
            { NonterminalSymbols::Program1,                                "Program1"                               }, // 169
            { NonterminalSymbols::RatianalType1,                           "RatianalType1"                          }, // 170
            { NonterminalSymbols::RatianalType2,                           "RatianalType2"                          }, // 171
            { NonterminalSymbols::Relation,                                "Relation"                               }, // 172
            { NonterminalSymbols::Relation1,                               "Relation1"                              }, // 173
            { NonterminalSymbols::ResultArgs,                              "ResultArgs"                             }, // 174
            { NonterminalSymbols::RightHandPostfixRationalOperator,        "RightHandPostfixRationalOperator"       }, // 175
            { NonterminalSymbols::RightHandVar,                            "RightHandVar"                           }, // 176
            { NonterminalSymbols::RightHandVar1,                           "RightHandVar1"                          }, // 177
            { NonterminalSymbols::Statement,                               "Statement"                              }, // 178
            { NonterminalSymbols::Statement1,                              "Statement1"                             }, // 179
            { NonterminalSymbols::Statement2,                              "Statement2"                             }, // 180
            { NonterminalSymbols::Summand,                                 "Summand"                                }, // 181
            { NonterminalSymbols::Summand1,                                "Summand1"                               }, // 182
            { NonterminalSymbols::Type,                                    "Type"                                   }, // 183
            { NonterminalSymbols::TypeDefinition,                          "TypeDefinition"                         }, // 184
            { NonterminalSymbols::TypeSection,                             "TypeSection"                            }, // 185
            { NonterminalSymbols::TypeSection1,                            "TypeSection1"                           }, // 186
            { NonterminalSymbols::UnaryAdditiveOperator,                   "UnaryAdditiveOperator"                  }, // 187
            { NonterminalSymbols::UnaryOperator,                           "UnaryOperator"                          }, // 188
            { NonterminalSymbols::UnaryOperator1,                          "UnaryOperator1"                         }, // 189
            { NonterminalSymbols::UnaryOperatorOverload,                   "UnaryOperatorOverload"                  }, // 190
            { NonterminalSymbols::UnconditionalJumpOperator,               "UnconditionalJumpOperator"              }, // 191
            { NonterminalSymbols::Var,                                     "Var"                                    }, // 192
            { NonterminalSymbols::Var1,                                    "Var1"                                   }, // 193
            { NonterminalSymbols::Var2,                                    "Var2"                                   }, // 194
            { NonterminalSymbols::Var3,                                    "Var3"                                   }, // 195
            { NonterminalSymbols::Var4,                                    "Var4"                                   }, // 196
            { NonterminalSymbols::VarSection,                              "VarSection"                             }, // 197
            { NonterminalSymbols::VarSection1,                             "VarSection1"                            }, // 198
            { NonterminalSymbols::VarSection2,                             "VarSection2"                            }, // 199
            { NonterminalSymbols::NONTERMINAL_FINISH_INDEX,                "NONTERMINAL_FINISH_INDEX"               }, // 199
            { NonterminalSymbols::ACTION_START_INDEX,                      "ACTION_START_INDEX"                     }, // 199
            { NonterminalSymbols::DEFAULT_SYNTHESIZE,                      "DEFAULT_SYNTHESIZE"                     }, // 199
            { NonterminalSymbols::DEFAULT_INHERIT,                         "DEFAULT_INHERIT"                        }, // 199
            { NonterminalSymbols::TreeConstructor,                         "TreeConstructor"                        }, // 199
            { NonterminalSymbols::ACTION_FINISH_INDEX,                     "ACTION_FINISH_INDEX"                    }, // 199
            { NonterminalSymbols::EPSILON,                                 "EPSILON"                                }, // 199
        };
        for (const auto& pair : nonterminalTypePairsMapping)
        {
            nonterminalTypeMapping[pair.first] = pair.second;
        }
        first_time = false;
    }
    return nonterminalTypeMapping;
}

}
}
