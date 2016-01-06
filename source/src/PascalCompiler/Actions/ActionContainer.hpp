#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <boost/hana/tuple.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/integral_constant.hpp>

#include "IndividualActions/BaseHandlers.h"
#include "IndividualActions/TypeSectionActions.hpp"
#include "IndividualActions/IDListActions.hpp"
#include "IndividualActions/LabelSectionActions.hpp"
#include "IndividualActions/VarDeclarationSection.hpp"
#include "IndividualActions/StatementsActions.hpp"
#include "IndividualActions/ProgramActions.hpp"
#include "IndividualActions/ExpressionActions.hpp"

template <int TokenType, class ActionType>
using Rule = boost::mpl::pair<boost::mpl::int_<TokenType>, ActionType>;

auto ActionContainer = boost::hana::make_tuple(
    boost::hana::make_pair(boost::hana::int_c<DEFAULT_INHERIT>,           boost::hana::type_c<Parser::BaseInheritItem>),
    boost::hana::make_pair(boost::hana::int_c<DEFAULT_SYNTHESIZE>,        boost::hana::type_c<Parser::BaseSynthesizeItem>),
    boost::hana::make_pair(boost::hana::int_c<TreeConstructor>,           boost::hana::type_c<PascalCompiler::TreeConstructor>),
    boost::hana::make_pair(boost::hana::int_c<EnumTypeConstruction>,      boost::hana::type_c<PascalCompiler::EnumTypeConstruction>),
    boost::hana::make_pair(boost::hana::int_c<IndexTypeIDCase>,           boost::hana::type_c<PascalCompiler::IndexTypeIDCase>),
    boost::hana::make_pair(boost::hana::int_c<IndexTypeCXSize>,           boost::hana::type_c<PascalCompiler::IndexTypeCXSize>),
    boost::hana::make_pair(boost::hana::int_c<Massive1AppendAtStart>,     boost::hana::type_c<PascalCompiler::Massive1AppendAtStart>),
    boost::hana::make_pair(boost::hana::int_c<Massive1InitVec>,           boost::hana::type_c<PascalCompiler::Massive1InitVec>),
    boost::hana::make_pair(boost::hana::int_c<ArrayCreationAction>,       boost::hana::type_c<PascalCompiler::ArrayCreationAction>),
    boost::hana::make_pair(boost::hana::int_c<ComplexPass>,               boost::hana::type_c<PascalCompiler::ComplexPass>),
    boost::hana::make_pair(boost::hana::int_c<RationalPass>,              boost::hana::type_c<PascalCompiler::RationalPass>),
    boost::hana::make_pair(boost::hana::int_c<IntegerPass>,               boost::hana::type_c<PascalCompiler::IntegerPass>),
    boost::hana::make_pair(boost::hana::int_c<GetTypeAccordingToID>,      boost::hana::type_c<PascalCompiler::GetTypeAccordingToID>),
    boost::hana::make_pair(boost::hana::int_c<TypePropagation>,           boost::hana::type_c<PascalCompiler::TypePropagation>),
    boost::hana::make_pair(boost::hana::int_c<TypeRegistrationAction>,    boost::hana::type_c<PascalCompiler::TypeRegistrationAction>),
    boost::hana::make_pair(boost::hana::int_c<FinalAppendList>,           boost::hana::type_c<PascalCompiler::FinalAppendList>),
    boost::hana::make_pair(boost::hana::int_c<AppendIdList>,              boost::hana::type_c<PascalCompiler::AppendIdList>),
    boost::hana::make_pair(boost::hana::int_c<InitIdList>,                boost::hana::type_c<PascalCompiler::InitIdList>),
    boost::hana::make_pair(boost::hana::int_c<LabelRegistration>,         boost::hana::type_c<PascalCompiler::LabelRegistration>),
    boost::hana::make_pair(boost::hana::int_c<VariableDeclarationAction>, boost::hana::type_c<PascalCompiler::VariableDeclarationAction>),
    boost::hana::make_pair(boost::hana::int_c<CreateBlockAction>,         boost::hana::type_c<PascalCompiler::CreateBlockAction>),
    boost::hana::make_pair(boost::hana::int_c<LabelOrAssignmentAction>,   boost::hana::type_c<PascalCompiler::LabelOrAssignmentAction>),
    boost::hana::make_pair(boost::hana::int_c<LabelAction>,               boost::hana::type_c<PascalCompiler::LabelAction>),
    boost::hana::make_pair(boost::hana::int_c<AssignmentAction>,          boost::hana::type_c<PascalCompiler::AssignmentAction>),
    boost::hana::make_pair(boost::hana::int_c<LoopDirectionTO>,           boost::hana::type_c<PascalCompiler::LoopDirectionTO>),
    boost::hana::make_pair(boost::hana::int_c<LoopDirectionDT>,           boost::hana::type_c<PascalCompiler::LoopDirectionDT>),
    boost::hana::make_pair(boost::hana::int_c<LoopWithParameterAction>,   boost::hana::type_c<PascalCompiler::LoopWithParameterAction>),
    boost::hana::make_pair(boost::hana::int_c<FinishMainFuntion>,         boost::hana::type_c<PascalCompiler::FinishMainFuntion>),
// Expression
    boost::hana::make_pair(boost::hana::int_c<ValuePropagation>,          boost::hana::type_c<PascalCompiler::ValuePropagation>),
    boost::hana::make_pair(boost::hana::int_c<ExpressionCombiner>,        boost::hana::type_c<PascalCompiler::ExpressionCombiner>),
    boost::hana::make_pair(boost::hana::int_c<SuperExpressionCombiner>,   boost::hana::type_c<PascalCompiler::SuperExpressionCombiner>),
    boost::hana::make_pair(boost::hana::int_c<BracketValuePropagation>,   boost::hana::type_c<PascalCompiler::BracketValuePropagation>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOPValue>,              boost::hana::type_c<PascalCompiler::UnaryOPValue>),
    boost::hana::make_pair(boost::hana::int_c<CreateConstComplex1>,       boost::hana::type_c<PascalCompiler::CreateConstComplex1>),
    boost::hana::make_pair(boost::hana::int_c<CreateConstComplex2>,       boost::hana::type_c<PascalCompiler::CreateConstComplex2>),
    boost::hana::make_pair(boost::hana::int_c<CreateConstComplex3>,       boost::hana::type_c<PascalCompiler::CreateConstComplex3>),
    boost::hana::make_pair(boost::hana::int_c<RationalStart>,             boost::hana::type_c<PascalCompiler::RationalStart>),
    boost::hana::make_pair(boost::hana::int_c<RationalEnd>,               boost::hana::type_c<PascalCompiler::RationalEnd>),
    boost::hana::make_pair(boost::hana::int_c<JFlagTrue>,                 boost::hana::type_c<PascalCompiler::JFlagTrue>),
    boost::hana::make_pair(boost::hana::int_c<JFlagFalse>,                boost::hana::type_c<PascalCompiler::JFlagFalse>),
    boost::hana::make_pair(boost::hana::int_c<LoadRightHandVar>,          boost::hana::type_c<PascalCompiler::LoadRightHandVar>),
    boost::hana::make_pair(boost::hana::int_c<PropagateLeftHandVarInfo>,  boost::hana::type_c<PascalCompiler::PropagateLeftHandVarInfo>),
    boost::hana::make_pair(boost::hana::int_c<FinalAppendExprList>,       boost::hana::type_c<PascalCompiler::FinalAppendExprList>),
    boost::hana::make_pair(boost::hana::int_c<AppendExprList>,            boost::hana::type_c<PascalCompiler::AppendExprList>),
    boost::hana::make_pair(boost::hana::int_c<InitExprList>,              boost::hana::type_c<PascalCompiler::InitExprList>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorOPPLUS>,       boost::hana::type_c<PascalCompiler::UnaryOperatorOPPLUS>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorOPMINUS>,      boost::hana::type_c<PascalCompiler::UnaryOperatorOPMINUS>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorOPMUL>,        boost::hana::type_c<PascalCompiler::UnaryOperatorOPMUL>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWINTOP>,      boost::hana::type_c<PascalCompiler::UnaryOperatorRWINTOP>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorOPDIV>,        boost::hana::type_c<PascalCompiler::UnaryOperatorOPDIV>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWIR>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWIR>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWSM>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWSM>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWFR>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWFR>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWLN>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWLN>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWIM>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWIM>),
    boost::hana::make_pair(boost::hana::int_c<UnaryOperatorRWRE>,         boost::hana::type_c<PascalCompiler::UnaryOperatorRWRE>),
    boost::hana::make_pair(boost::hana::int_c<OPPropagation>,             boost::hana::type_c<PascalCompiler::OPPropagation>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPGT>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPGT>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPLT>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPLT>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPGE>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPGE>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPLE>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPLE>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPEQ>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPEQ>),
    boost::hana::make_pair(boost::hana::int_c<BinaryRelationOperatorOPNE>,boost::hana::type_c<PascalCompiler::BinaryRelationOperatorOPNE>),
    boost::hana::make_pair(boost::hana::int_c<PostfixRationalOperatorRWDNN>,boost::hana::type_c<PascalCompiler::PostfixRationalOperatorRWDNN>),
    boost::hana::make_pair(boost::hana::int_c<PostfixRationalOperatorRWDN>,boost::hana::type_c<PascalCompiler::PostfixRationalOperatorRWDN>),
    boost::hana::make_pair(boost::hana::int_c<PostfixRationalOperatorRWN>,boost::hana::type_c<PascalCompiler::PostfixRationalOperatorRWN>),
    boost::hana::make_pair(boost::hana::int_c<AppendIndexVarHandlerList>,boost::hana::type_c<PascalCompiler::AppendIndexVarHandlerList>),
    boost::hana::make_pair(boost::hana::int_c<AppendMemberVarHandlerList>,boost::hana::type_c<PascalCompiler::AppendMemberVarHandlerList>),
    boost::hana::make_pair(boost::hana::int_c<InitVarHandlerList>,boost::hana::type_c<PascalCompiler::InitVarHandlerList>),
    boost::hana::make_pair(boost::hana::int_c<BinaryLogicOperatorRWLA>,boost::hana::type_c<PascalCompiler::BinaryLogicOperatorRWLA>),
    boost::hana::make_pair(boost::hana::int_c<BinaryLogicOperatorRWLO>,boost::hana::type_c<PascalCompiler::BinaryLogicOperatorRWLO>),
    boost::hana::make_pair(boost::hana::int_c<ConditionalOperatorAction>,boost::hana::type_c<PascalCompiler::ConditionalOperatorAction>),
    boost::hana::make_pair(boost::hana::int_c<ConditionalOperatorElseAction>,boost::hana::type_c<PascalCompiler::ConditionalOperatorElseAction>),
    boost::hana::make_pair(boost::hana::int_c<ConditionalOperatorNoElseAction>,boost::hana::type_c<PascalCompiler::ConditionalOperatorNoElseAction>),
    boost::hana::make_pair(boost::hana::int_c<LabelID>,boost::hana::type_c<PascalCompiler::LabelID>),
    boost::hana::make_pair(boost::hana::int_c<GotoLabel>,boost::hana::type_c<PascalCompiler::GotoLabel>),
    boost::hana::make_pair(boost::hana::int_c<IDPass>,boost::hana::type_c<PascalCompiler::IDPass>),
    boost::hana::make_pair(boost::hana::int_c<IDPass2>,boost::hana::type_c<PascalCompiler::IDPass2>),
    boost::hana::make_pair(boost::hana::int_c<LabelStatement>,boost::hana::type_c<PascalCompiler::LabelStatement>)
);

#endif // ACTIONCONTAINER_H
