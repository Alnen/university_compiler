#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "ActionContainer.hpp"

template <int TokenType, class ActionType>
using Rule = boost::mpl::pair<boost::mpl::int_<TokenType>, ActionType>;

using ActionContainer = boost::mpl::map<
    Rule<DEFAULT_INHERIT,       Parser::BaseInheritItem>,
    Rule<DEFAULT_SYNTHESIZE,    Parser::BaseSynthesizeItem>,
    Rule<ACTION1,               PascalParser::Action1>,
    Rule<TreeConstructor,       PascalParser::TreeConstructor>,
    Rule<ACTION3,               PascalParser::Action3>
>;

#endif // ACTIONCONTAINER_H
