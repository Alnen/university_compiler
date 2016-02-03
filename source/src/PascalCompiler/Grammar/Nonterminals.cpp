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
            { NonterminalSymbols::InputOperator,                   "InputOperator"                   }, // 136
            { NonterminalSymbols::Label,                           "Label"                           }, // 137
            { NonterminalSymbols::LabelOrAssignmentStatement,      "LabelOrAssignmentStatement"      }, // 138
            { NonterminalSymbols::LabelSection,                    "LabelSection"                    }, // 139
            { NonterminalSymbols::LeftHandVar,                     "LeftHandVar"                     }, // 140
            { NonterminalSymbols::LocalDeclaration,                "LocalDeclaration"                }, // 141
            { NonterminalSymbols::LogicAnd,                        "LogicAnd"                        }, // 142
            { NonterminalSymbols::LogicOr,                         "LogicOr"                         }, // 143
            { NonterminalSymbols::LoopDirection,                   "LoopDirection"                   }, // 144
            { NonterminalSymbols::LoopWithParameterOperator,       "LoopWithParameterOperator"       }, // 145
            { NonterminalSymbols::Massive,                         "Massive"                         }, // 146
            { NonterminalSymbols::MulExpression,                   "MulExpression"                   }, // 147
            { NonterminalSymbols::NextAdditiveExpression,          "NextAdditiveExpression"          }, // 148
            { NonterminalSymbols::NextConstDefinition,             "NextConstDefinition"             }, // 149
            { NonterminalSymbols::NextGcdExpression,               "NextGcdExpression"               }, // 150
            { NonterminalSymbols::NextIndex,                       "NextIndex"                       }, // 151
            { NonterminalSymbols::NextLabel,                       "NextLabel"                       }, // 152
            { NonterminalSymbols::NextLogicAnd,                    "NextLogicAnd"                    }, // 153
            { NonterminalSymbols::NextLogicOr,                     "NextLogicOr"                     }, // 154
            { NonterminalSymbols::NextMulExpression,               "NextMulExpression"               }, // 155
            { NonterminalSymbols::NextRecordFieldDefinition,       "NextRecordFieldDefinition"       }, // 156
            { NonterminalSymbols::NextRelationalExpression,        "NextRelationalExpression"        }, // 157
            { NonterminalSymbols::NextTypeDefinition,              "NextTypeDefinition"              }, // 158
            { NonterminalSymbols::NextVarDefinition,               "NextVarDefinition"               }, // 159
            { NonterminalSymbols::NotEmptyExprList,                "NotEmptyExprList"                }, // 160
            { NonterminalSymbols::NotEmptyGlobalDeclaration,       "NotEmptyGlobalDeclaration"       }, // 161
            { NonterminalSymbols::NotEmptyIdList,                  "NotEmptyIdList"                  }, // 162
            { NonterminalSymbols::NotEmptyLocalDeclaration,        "NotEmptyLocalDeclaration"        }, // 163
            { NonterminalSymbols::NotEmptyResultParameters,        "NotEmptyResultParameters"        }, // 164
            { NonterminalSymbols::NotEmptyStatement,               "NotEmptyStatement"               }, // 165
            { NonterminalSymbols::NumericConstant,                 "NumericConstant"                 }, // 166
            { NonterminalSymbols::OneTypeParameters,               "OneTypeParameters"               }, // 167
            { NonterminalSymbols::OperatorOverloadSection,         "OperatorOverloadSection"         }, // 168
            { NonterminalSymbols::OutputOperator,                  "OutputOperator"                  }, // 169
            { NonterminalSymbols::OverloadBinaryOperator,          "OverloadBinaryOperator"          }, // 170
            { NonterminalSymbols::OverloadOperator,                "OverloadOperator"                }, // 171
            { NonterminalSymbols::OverloadOperatorEnd,             "OverloadOperatorEnd"             }, // 172
            { NonterminalSymbols::ProcedureDeclaration,            "ProcedureDeclaration"            }, // 173
            { NonterminalSymbols::ProcedureParameters,             "ProcedureParameters"             }, // 174
            { NonterminalSymbols::Program,                         "Program"                         }, // 175
            { NonterminalSymbols::RationalComplexPart,             "RationalComplexPart"             }, // 176
            { NonterminalSymbols::RationalNumberPart,              "RationalNumberPart"              }, // 177
            { NonterminalSymbols::RecordFieldDefinition,           "RecordFieldDefinition"           }, // 178
            { NonterminalSymbols::RecordType,                      "RecordType"                      }, // 179
            { NonterminalSymbols::RelationalExpression,            "RelationalExpression"            }, // 180
            { NonterminalSymbols::ResultParameters,                "ResultParameters"                }, // 181
            { NonterminalSymbols::RightHandVar,                    "RightHandVar"                    }, // 182
            { NonterminalSymbols::Statement,                       "Statement"                       }, // 183
            { NonterminalSymbols::StatementList,                   "StatementList"                   }, // 184
            { NonterminalSymbols::Type,                            "Type"                            }, // 185
            { NonterminalSymbols::TypeDefinition,                  "TypeDefinition"                  }, // 186
            { NonterminalSymbols::TypeSection,                     "TypeSection"                     }, // 187
            { NonterminalSymbols::UnaryAdditiveOperator,           "UnaryAdditiveOperator"           }, // 188
            { NonterminalSymbols::UnaryNotAdditiveOperator,        "UnaryNotAdditiveOperator"        }, // 189
            { NonterminalSymbols::UnaryOperator,                   "UnaryOperator"                   }, // 190
            { NonterminalSymbols::UnaryOperatorOverload,           "UnaryOperatorOverload"           }, // 191
            { NonterminalSymbols::UnconditionalJumpOperator,       "UnconditionalJumpOperator"       }, // 192
            { NonterminalSymbols::UnsignedFactor,                  "UnsignedFactor"                  }, // 193
            { NonterminalSymbols::VarDefinition,                   "VarDefinition"                   }, // 194
            { NonterminalSymbols::VarHandlerList,                  "VarHandlerList"                  }, // 195
            { NonterminalSymbols::VarSection,                      "VarSection"                      }, // 196
            { NonterminalSymbols::NONTERMINAL_FINISH_INDEX,        "NONTERMINAL_FINISH_INDEX"        }, // 197

            { NonterminalSymbols::ACTION_START_INDEX,              "ACTION_START_INDEX"              }, // 198
            { NonterminalSymbols::DEFAULT_INHERIT,                 "DEFAULT_INHERIT"                 }, // 224
            { NonterminalSymbols::DEFAULT_SYNTHESIZE,              "DEFAULT_SYNTHESIZE"              }, // 225
            { NonterminalSymbols::TreeConstructor,                 "TreeConstructor"                 }, // 264
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
