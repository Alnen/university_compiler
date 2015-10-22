#ifndef REGEXASTTODFA_H
#define REGEXASTTODFA_H

#include <memory>

#include "RegexASTNode.h"

namespace Lexer {

template <class DeterministicFiniteAutomaton>
class RegexASTToDFA
{
public:
    using DFA = DeterministicFiniteAutomaton;
    using State = typename DFA::State;

    RegexASTToDFA(DFA& dfa): m_dfa(dfa) {}

    template <class Handler>
    bool covertAST(State start_state, RegexAST::BasicNode* ast, Handler handler);

protected:
    DFA& m_dfa;
};

}

#endif // REGEXASTTODFA_H
