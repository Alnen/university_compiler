#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

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

using ActionContainer = boost::mpl::map<
    Rule<DEFAULT_INHERIT,           Parser::BaseInheritItem>,
    Rule<DEFAULT_SYNTHESIZE,        Parser::BaseSynthesizeItem>,
    Rule<TreeConstructor,           PascalCompiler::TreeConstructor>,
    Rule<EnumTypeConstruction,      PascalCompiler::EnumTypeConstruction>,
    Rule<IndexTypeIDCase,           PascalCompiler::IndexTypeIDCase>,
    Rule<IndexTypeCXSize,           PascalCompiler::IndexTypeCXSize>,
    Rule<Massive1AppendAtStart,     PascalCompiler::Massive1AppendAtStart>,
    Rule<Massive1InitVec,           PascalCompiler::Massive1InitVec>,
    Rule<ArrayCreationAction,       PascalCompiler::ArrayCreationAction>,
    Rule<ComplexPass,               PascalCompiler::ComplexPass>,
    Rule<RationalPass,              PascalCompiler::RationalPass>,
    Rule<IntegerPass,               PascalCompiler::IntegerPass>,
    Rule<GetTypeAccordingToID,      PascalCompiler::GetTypeAccordingToID>,
    Rule<TypePropagation,           PascalCompiler::TypePropagation>,
    Rule<TypeRegistrationAction,    PascalCompiler::TypeRegistrationAction>,
    Rule<FinalAppendList,           PascalCompiler::FinalAppendList>,
    Rule<AppendIdList,              PascalCompiler::AppendIdList>,
    Rule<InitIdList,                PascalCompiler::InitIdList>,
    Rule<LabelRegistration,         PascalCompiler::LabelRegistration>,
    Rule<VariableDeclarationAction, PascalCompiler::VariableDeclarationAction>,
    Rule<CreateBlockAction,         PascalCompiler::CreateBlockAction>,
    Rule<LabelOrAssignmentAction,   PascalCompiler::LabelOrAssignmentAction>,
    Rule<LabelAction,               PascalCompiler::LabelAction>,
    Rule<AssignmentAction,          PascalCompiler::AssignmentAction>,
    Rule<LoopDirectionTO,           PascalCompiler::LoopDirectionTO>,
    Rule<LoopDirectionDT,           PascalCompiler::LoopDirectionDT>,
    Rule<LoopWithParameterAction,   PascalCompiler::LoopWithParameterAction>,
    Rule<FinishMainFuntion,         PascalCompiler::FinishMainFuntion>
>;

#endif // ACTIONCONTAINER_H
