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
            { NonterminalSymbols::NONTERMINAL_START_INDEX,         "NONTERMINAL_START_INDEX"         }, // 99
            { NonterminalSymbols::AdditiveExpression,              "AdditiveExpression"              }, // 100
            { NonterminalSymbols::AssignmentOperator,              "AssignmentOperator"              }, // 101
            { NonterminalSymbols::BinaryAdditiveOperator,          "BinaryAdditiveOperator"          }, // 102
            { NonterminalSymbols::BinaryLogicOperatorAnd,          "BinaryLogicOperatorAnd"          }, // 103
            { NonterminalSymbols::BinaryLogicOperatorOr,           "BinaryLogicOperatorOr"           }, // 104
            { NonterminalSymbols::BinaryMulOperator,               "BinaryMulOperator"               }, // 105
            { NonterminalSymbols::BinaryNotAdditiveOperator,       "BinaryNotAdditiveOperator"       }, // 106
            { NonterminalSymbols::BinaryOperatorOverload,          "BinaryOperatorOverload"          }, // 107
            { NonterminalSymbols::BinaryOperatorOverloadEnd,       "BinaryOperatorOverloadEnd"       }, // 108
            { NonterminalSymbols::BinaryRelationOperator,          "BinaryRelationOperator"          }, // 109
            { NonterminalSymbols::BuiltInType,                     "BuiltInType"                     }, // 110
            { NonterminalSymbols::ComplexNumberPart,               "ComplexNumberPart"               }, // 111
            { NonterminalSymbols::ComplexNumberSuffix,             "ComplexNumberSuffix"             }, // 112
            { NonterminalSymbols::CompoundStatement,               "CompoundStatement"               }, // 113
            { NonterminalSymbols::CompoundStatementEnd,            "CompoundStatementEnd"            }, // 114
            { NonterminalSymbols::ConditionalOperator,             "ConditionalOperator"             }, // 115
            { NonterminalSymbols::ConditionalOperatorEnd,          "ConditionalOperatorEnd"          }, // 116
            { NonterminalSymbols::ConstDefinition,                 "ConstDefinition"                 }, // 117
            { NonterminalSymbols::ConstSection,                    "ConstSection"                    }, // 118
            { NonterminalSymbols::CreateBlock,                     "CreateBlock"                     }, // 119
            { NonterminalSymbols::DefiniteType,                    "DefiniteType"                    }, // 120
            { NonterminalSymbols::EmptyStatement,                  "EmptyStatement"                  }, // 121
            { NonterminalSymbols::EnumType,                        "EnumType"                        }, // 122
            { NonterminalSymbols::ExprList,                        "ExprList"                        }, // 123
            { NonterminalSymbols::ExprListTail,                    "ExprListTail"                    }, // 124
            { NonterminalSymbols::Expression,                      "Expression"                      }, // 125
            { NonterminalSymbols::Factor,                          "Factor"                          }, // 126
            { NonterminalSymbols::FunctionCall,                    "FunctionCall"                    }, // 127
            { NonterminalSymbols::FunctionDeclaration,             "FunctionDeclaration"             }, // 128
            { NonterminalSymbols::FunctionParameters,              "FunctionParameters"              }, // 129
            { NonterminalSymbols::FunctionParametersTail,          "FunctionParametersTail"          }, // 130
            { NonterminalSymbols::GcdExpression,                   "GcdExpression"                   }, // 131
            { NonterminalSymbols::GlobalDeclaration,               "GlobalDeclaration"               }, // 132
            { NonterminalSymbols::IdList,                          "IdList"                          }, // 133
            { NonterminalSymbols::IdListTail,                      "IdListTail"                      }, // 134
            { NonterminalSymbols::Index,                           "Index"                           }, // 135
            { NonterminalSymbols::IndexList,                       "IndexList"                       }, // 136
            { NonterminalSymbols::IndexListTail,                   "IndexListTail"                   }, // 137
            { NonterminalSymbols::InputOperator,                   "InputOperator"                   }, // 138
            { NonterminalSymbols::Label,                           "Label"                           }, // 139
            { NonterminalSymbols::LabelOrAssignmentStatement,      "LabelOrAssignmentStatement"      }, // 140
            { NonterminalSymbols::LabelSection,                    "LabelSection"                    }, // 141
            { NonterminalSymbols::LeftHandVar,                     "LeftHandVar"                     }, // 142
            { NonterminalSymbols::LocalDeclaration,                "LocalDeclaration"                }, // 143
            { NonterminalSymbols::LogicAnd,                        "LogicAnd"                        }, // 144
            { NonterminalSymbols::LogicOr,                         "LogicOr"                         }, // 145
            { NonterminalSymbols::LoopDirection,                   "LoopDirection"                   }, // 146
            { NonterminalSymbols::LoopWithParameterOperator,       "LoopWithParameterOperator"       }, // 147
            { NonterminalSymbols::Massive,                         "Massive"                         }, // 148
            { NonterminalSymbols::MulExpression,                   "MulExpression"                   }, // 149
            { NonterminalSymbols::NextAdditiveExpression,          "NextAdditiveExpression"          }, // 150
            { NonterminalSymbols::NextConstDefinition,             "NextConstDefinition"             }, // 151
            { NonterminalSymbols::NextGcdExpression,               "NextGcdExpression"               }, // 152
            { NonterminalSymbols::NextLabel,                       "NextLabel"                       }, // 153
            { NonterminalSymbols::NextLogicAnd,                    "NextLogicAnd"                    }, // 154
            { NonterminalSymbols::NextLogicOr,                     "NextLogicOr"                     }, // 155
            { NonterminalSymbols::NextMulExpression,               "NextMulExpression"               }, // 156
            { NonterminalSymbols::NextRecordFieldDefinition,       "NextRecordFieldDefinition"       }, // 157
            { NonterminalSymbols::NextRelationalExpression,        "NextRelationalExpression"        }, // 158
            { NonterminalSymbols::NextTypeDefinition,              "NextTypeDefinition"              }, // 159
            { NonterminalSymbols::NextVarDefinition,               "NextVarDefinition"               }, // 160
            { NonterminalSymbols::NotEmptyExprList,                "NotEmptyExprList"                }, // 161
            { NonterminalSymbols::NotEmptyGlobalDeclaration,       "NotEmptyGlobalDeclaration"       }, // 162
            { NonterminalSymbols::NotEmptyIdList,                  "NotEmptyIdList"                  }, // 163
            { NonterminalSymbols::NotEmptyIndexList,               "NotEmptyIndexList"               }, // 164
            { NonterminalSymbols::NotEmptyLocalDeclaration,        "NotEmptyLocalDeclaration"        }, // 165
            { NonterminalSymbols::NotEmptyResultParameters,        "NotEmptyResultParameters"        }, // 166
            { NonterminalSymbols::NotEmptyStatement,               "NotEmptyStatement"               }, // 167
            { NonterminalSymbols::NumericConstant,                 "NumericConstant"                 }, // 168
            { NonterminalSymbols::OneTypeParameters,               "OneTypeParameters"               }, // 169
            { NonterminalSymbols::OperatorOverloadSection,         "OperatorOverloadSection"         }, // 170
            { NonterminalSymbols::OutputOperator,                  "OutputOperator"                  }, // 171
            { NonterminalSymbols::OverloadBinaryOperator,          "OverloadBinaryOperator"          }, // 172
            { NonterminalSymbols::OverloadOperator,                "OverloadOperator"                }, // 173
            { NonterminalSymbols::OverloadOperatorEnd,             "OverloadOperatorEnd"             }, // 174
            { NonterminalSymbols::ProcedureDeclaration,            "ProcedureDeclaration"            }, // 175
            { NonterminalSymbols::ProcedureParameters,             "ProcedureParameters"             }, // 176
            { NonterminalSymbols::Program,                         "Program"                         }, // 177
            { NonterminalSymbols::RationalComplexPart,             "RationalComplexPart"             }, // 178
            { NonterminalSymbols::RationalNumberPart,              "RationalNumberPart"              }, // 179
            { NonterminalSymbols::RecordFieldDefinition,           "RecordFieldDefinition"           }, // 180
            { NonterminalSymbols::RecordType,                      "RecordType"                      }, // 181
            { NonterminalSymbols::RelationalExpression,            "RelationalExpression"            }, // 182
            { NonterminalSymbols::ResultParameters,                "ResultParameters"                }, // 183
            { NonterminalSymbols::RightHandVar,                    "RightHandVar"                    }, // 184
            { NonterminalSymbols::Statement,                       "Statement"                       }, // 185
            { NonterminalSymbols::StatementList,                   "StatementList"                   }, // 186
            { NonterminalSymbols::Type,                            "Type"                            }, // 187
            { NonterminalSymbols::TypeDefinition,                  "TypeDefinition"                  }, // 188
            { NonterminalSymbols::TypeSection,                     "TypeSection"                     }, // 189
            { NonterminalSymbols::UnaryAdditiveOperator,           "UnaryAdditiveOperator"           }, // 190
            { NonterminalSymbols::UnaryNotAdditiveOperator,        "UnaryNotAdditiveOperator"        }, // 191
            { NonterminalSymbols::UnaryOperator,                   "UnaryOperator"                   }, // 192
            { NonterminalSymbols::UnaryOperatorOverload,           "UnaryOperatorOverload"           }, // 193
            { NonterminalSymbols::UnconditionalJumpOperator,       "UnconditionalJumpOperator"       }, // 194
            { NonterminalSymbols::UnsignedFactor,                  "UnsignedFactor"                  }, // 195
            { NonterminalSymbols::VarDefinition,                   "VarDefinition"                   }, // 196
            { NonterminalSymbols::VarHandlerList,                  "VarHandlerList"                  }, // 197
            { NonterminalSymbols::VarSection,                      "VarSection"                      }, // 198
            { NonterminalSymbols::NONTERMINAL_FINISH_INDEX,        "NONTERMINAL_FINISH_INDEX"        }, // 199
            { NonterminalSymbols::ACTION_START_INDEX,              "ACTION_START_INDEX"              }, // 200
            { NonterminalSymbols::AppendExprList,                  "AppendExprList"                  }, // 201
            { NonterminalSymbols::AppendIdList,                    "AppendIdList"                    }, // 202
            { NonterminalSymbols::AppendIndexVarHandlerList,       "AppendIndexVarHandlerList"       }, // 203
            { NonterminalSymbols::AppendMemberVarHandlerList,      "AppendMemberVarHandlerList"      }, // 204
            { NonterminalSymbols::ArrayCreationAction,             "ArrayCreationAction"             }, // 205
            { NonterminalSymbols::AssignmentAction,                "AssignmentAction"                }, // 206
            { NonterminalSymbols::BinaryLogicOperatorRWLA,         "BinaryLogicOperatorRWLA"         }, // 207
            { NonterminalSymbols::BinaryLogicOperatorRWLO,         "BinaryLogicOperatorRWLO"         }, // 208
            { NonterminalSymbols::BinaryRelationOperatorOPEQ,      "BinaryRelationOperatorOPEQ"      }, // 209
            { NonterminalSymbols::BinaryRelationOperatorOPGE,      "BinaryRelationOperatorOPGE"      }, // 210
            { NonterminalSymbols::BinaryRelationOperatorOPGT,      "BinaryRelationOperatorOPGT"      }, // 211
            { NonterminalSymbols::BinaryRelationOperatorOPLE,      "BinaryRelationOperatorOPLE"      }, // 212
            { NonterminalSymbols::BinaryRelationOperatorOPLT,      "BinaryRelationOperatorOPLT"      }, // 213
            { NonterminalSymbols::BinaryRelationOperatorOPNE,      "BinaryRelationOperatorOPNE"      }, // 214
            { NonterminalSymbols::BracketValuePropagation,         "BracketValuePropagation"         }, // 215
            { NonterminalSymbols::ComplexPass,                     "ComplexPass"                     }, // 216
            { NonterminalSymbols::ConditionalOperatorAction,       "ConditionalOperatorAction"       }, // 217
            { NonterminalSymbols::ConditionalOperatorElseAction,   "ConditionalOperatorElseAction"   }, // 218
            { NonterminalSymbols::ConditionalOperatorNoElseAction, "ConditionalOperatorNoElseAction" }, // 219
            { NonterminalSymbols::CreateBlockAction,               "CreateBlockAction"               }, // 220
            { NonterminalSymbols::CreateConstComplex1,             "CreateConstComplex1"             }, // 221
            { NonterminalSymbols::CreateConstComplex2,             "CreateConstComplex2"             }, // 222
            { NonterminalSymbols::CreateConstComplex3,             "CreateConstComplex3"             }, // 223
            { NonterminalSymbols::DEFAULT_INHERIT,                 "DEFAULT_INHERIT"                 }, // 224
            { NonterminalSymbols::DEFAULT_SYNTHESIZE,              "DEFAULT_SYNTHESIZE"              }, // 225
            { NonterminalSymbols::EnumTypeConstruction,            "EnumTypeConstruction"            }, // 226
            { NonterminalSymbols::ExpressionCombiner,              "ExpressionCombiner"              }, // 227
            { NonterminalSymbols::FinalAppendExprList,             "FinalAppendExprList"             }, // 228
            { NonterminalSymbols::FinalAppendList,                 "FinalAppendList"                 }, // 229
            { NonterminalSymbols::FinishMainFuntion,               "FinishMainFuntion"               }, // 230
            { NonterminalSymbols::GetTypeAccordingToID,            "GetTypeAccordingToID"            }, // 231
            { NonterminalSymbols::GotoLabel,                       "GotoLabel"                       }, // 232
            { NonterminalSymbols::IDPass,                          "IDPass"                          }, // 233
            { NonterminalSymbols::IDPass2,                         "IDPass2"                         }, // 234
            { NonterminalSymbols::IndexListAppendAtStart,          "IndexListAppendAtStart"          }, // 235
            { NonterminalSymbols::IndexListInit,                   "IndexListInit"                   }, // 236
            { NonterminalSymbols::IndexTypeCXSize,                 "IndexTypeCXSize"                 }, // 237
            { NonterminalSymbols::IndexTypeIDCase,                 "IndexTypeIDCase"                 }, // 238
            { NonterminalSymbols::InitExprList,                    "InitExprList"                    }, // 239
            { NonterminalSymbols::InitIdList,                      "InitIdList"                      }, // 240
            { NonterminalSymbols::InitVarHandlerList,              "InitVarHandlerList"              }, // 241
            { NonterminalSymbols::IntegerPass,                     "IntegerPass"                     }, // 242
            { NonterminalSymbols::JFlagFalse,                      "JFlagFalse"                      }, // 243
            { NonterminalSymbols::JFlagTrue,                       "JFlagTrue"                       }, // 244
            { NonterminalSymbols::LabelAction,                     "LabelAction"                     }, // 245
            { NonterminalSymbols::LabelID,                         "LabelID"                         }, // 246
            { NonterminalSymbols::LabelOrAssignmentAction,         "LabelOrAssignmentAction"         }, // 247
            { NonterminalSymbols::LabelRegistration,               "LabelRegistration"               }, // 248
            { NonterminalSymbols::LabelStatement,                  "LabelStatement"                  }, // 249
            { NonterminalSymbols::LoadRightHandVar,                "LoadRightHandVar"                }, // 250
            { NonterminalSymbols::LoopDirectionDT,                 "LoopDirectionDT"                 }, // 251
            { NonterminalSymbols::LoopDirectionTO,                 "LoopDirectionTO"                 }, // 252
            { NonterminalSymbols::LoopWithParameterAction,         "LoopWithParameterAction"         }, // 253
            { NonterminalSymbols::OPPropagation,                   "OPPropagation"                   }, // 254
            { NonterminalSymbols::PostfixRationalOperatorRWDNN,    "PostfixRationalOperatorRWDNN"    }, // 255
            { NonterminalSymbols::PrintOperatorMain,               "PrintOperatorMain"               }, // 256
            { NonterminalSymbols::PrintOperatorRest,               "PrintOperatorRest"               }, // 257
            { NonterminalSymbols::PropagateLeftHandVarInfo,        "PropagateLeftHandVarInfo"        }, // 258
            { NonterminalSymbols::RationalEnd,                     "RationalEnd"                     }, // 259
            { NonterminalSymbols::RationalPass,                    "RationalPass"                    }, // 260
            { NonterminalSymbols::RationalStart,                   "RationalStart"                   }, // 261
            { NonterminalSymbols::ReadLeftHandVarAction,           "ReadLeftHandVarAction"           }, // 262
            { NonterminalSymbols::SuperExpressionCombiner,         "SuperExpressionCombiner"         }, // 263
            { NonterminalSymbols::TreeConstructor,                 "TreeConstructor"                 }, // 264
            { NonterminalSymbols::TypePropagation,                 "TypePropagation"                 }, // 265
            { NonterminalSymbols::TypeRegistrationAction,          "TypeRegistrationAction"          }, // 266
            { NonterminalSymbols::UnaryOPValue,                    "UnaryOPValue"                    }, // 267
            { NonterminalSymbols::UnaryOperatorOPDIV,              "UnaryOperatorOPDIV"              }, // 268
            { NonterminalSymbols::UnaryOperatorOPMINUS,            "UnaryOperatorOPMINUS"            }, // 269
            { NonterminalSymbols::UnaryOperatorOPMUL,              "UnaryOperatorOPMUL"              }, // 270
            { NonterminalSymbols::UnaryOperatorOPPLUS,             "UnaryOperatorOPPLUS"             }, // 271
            { NonterminalSymbols::UnaryOperatorRWFR,               "UnaryOperatorRWFR"               }, // 272
            { NonterminalSymbols::UnaryOperatorRWINTOP,            "UnaryOperatorRWINTOP"            }, // 273
            { NonterminalSymbols::UnaryOperatorRWIR,               "UnaryOperatorRWIR"               }, // 274
            { NonterminalSymbols::UnaryOperatorRWLN,               "UnaryOperatorRWLN"               }, // 275
            { NonterminalSymbols::UnaryOperatorRWSM,               "UnaryOperatorRWSM"               }, // 276
            { NonterminalSymbols::ValuePropagation,                "ValuePropagation"                }, // 277
            { NonterminalSymbols::VariableDeclarationAction,       "VariableDeclarationAction"       }, // 278
            { NonterminalSymbols::ACTION_FINISH_INDEX,             "ACTION_FINISH_INDEX"             }, // 279
            { NonterminalSymbols::EPSILON,                         "EPSILON"                         }  // 280
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
