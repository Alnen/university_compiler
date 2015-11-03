#ifndef REGEXASTTONFA_H
#define REGEXASTTONFA_H

#include "RegexASTNode.h"
#include "FiniteAutomata/NFA.h"
#include <boost/container/flat_map.hpp>
#include "RegexASTAnnotationEvaluator.h"

#include <vector>
#include "Utility.h"

#include <iostream>

namespace Lexer {

template <class _State, class _Checker>
class RegexASTtoNFA
{
public:
    using State = _State;
    using Checker = _Checker;

    RegexASTtoNFA(NFA<State, Checker> *nfa, State startState, State stateGenerator = 1);
    std::vector<State> convertASTtoNFA(RegexAST::BasicNode* ast,
                         RegexAST::BasicLeaf::LeafId firstLeafId,
                         const boost::container::flat_map<std::shared_ptr<IInputChecker>, std::vector<RegexAST::BasicLeaf::LeafId>, PointerLess>& inputToIdMapping, // TODO replace IInputChecker with Checker
                         const std::vector<std::vector<RegexAST::BasicLeaf::LeafId>>& followposTable);

protected:
    NFA<State, Checker> *m_nfa;
    State       m_startState;
    State       m_stateGenerator;
};

template <class State, class Checker>
RegexASTtoNFA<State, Checker>::RegexASTtoNFA(NFA<State, Checker> *nfa, State startState, State stateGenerator):
    m_nfa(nfa), m_startState(startState), m_stateGenerator(stateGenerator)
{
}

// ID
template <class State, class Checker>
std::vector<State> RegexASTtoNFA<State, Checker>::convertASTtoNFA(RegexAST::BasicNode* ast,
                     RegexAST::BasicLeaf::LeafId firstLeafId,
                     const boost::container::flat_map<std::shared_ptr<IInputChecker>, std::vector<RegexAST::BasicLeaf::LeafId>, PointerLess>& inputToIdMapping,
                     const std::vector<std::vector<RegexAST::BasicLeaf::LeafId>>& followposTable)
{
    using StateName = std::vector<State>;
    using Utility::set_union;

    std::vector<State> final_states;

    std::vector<std::pair<StateName, State>> stack;
    boost::container::flat_map<StateName, State> created_states;

    std::pair<StateName, State> current_state = std::make_pair(ast->m_firstpos, m_startState);
    created_states[ast->m_firstpos] = m_startState;
    stack.push_back(current_state);

    do
    {
        current_state = stack.back();
        stack.pop_back();
        //std::cout << "next unmarked state" << current_state.first << ":" << current_state.second << std::endl;
        for (const auto& key_value : inputToIdMapping)
        {
            std::cout << "for each inputs : " << key_value.first << std::endl;
            std::pair<StateName, State> nextState;
            bool finalState = false;
            for (auto leafId : key_value.second)
            {
                if (std::find(current_state.first.begin(), current_state.first.end(), leafId) != current_state.first.end())
                {
                    set_union(nextState.first, followposTable[leafId - firstLeafId]);
                }
            }
            if (std::find(nextState.first.begin(), nextState.first.end(), followposTable.size() + firstLeafId - 1) != nextState.first.end())
            {
                finalState = true;
            }
            if (nextState.first.empty()) continue;
            typename boost::container::flat_map<StateName, State>::const_iterator pos;
            if ((pos = created_states.find(nextState.first)) == created_states.end())
            {
                std::cout << "new state" << std::endl;
                nextState.second = m_stateGenerator++;
                stack.push_back(nextState);
                created_states[nextState.first] = nextState.second;
                std::cout << "alocated states" << std::endl;
                if (finalState)
                {
                    //std::cout << "new final state " << nextState.first << ":" << nextState.second << std::endl;
                    m_nfa->addFinalState(nextState.second);
                    final_states.push_back(nextState.second);
                }
                else
                {
                    //std::cout << "new ordinary state " << nextState.first << ":" << nextState.second << std::endl;
                    m_nfa->addState(nextState.second);
                }
            }
            else
            {
                nextState.second = (*pos).second;
            }
            m_nfa->addStateTransition(current_state.second, key_value.first, nextState.second);
            std::cout << "creating new transition: " << current_state.second << " => " << nextState.second  << std::endl;
        }
    }
    while(!stack.empty());
    return final_states;
}

}

#endif // REGEXASTTONFA_H
