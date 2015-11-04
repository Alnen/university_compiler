#ifndef NFA_h
#define NFA_h

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <boost/container/flat_map.hpp>

#include "DFA.h"
#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "Utility.h"



template <class State, class _Checker>
class NFA
{
public:
    using StateType = State;
    using CheckerType = _Checker;
    using InputChecker = Lexer::InputCheckerType;
    using InputCollection = boost::container::flat_map<CheckerType, std::vector<State>, PointerLess>;
    using StateTransitionTable = boost::container::flat_map<State, std::unique_ptr<InputCollection>>; // TODO : delete pointer comparison

    using DFAConvertAnswer = std::pair<
                                    DFA<State, CheckerType>,
                                    boost::container::flat_map<State, std::vector<State>>
                                      >;

    NFA();
    NFA(const NFA&) = delete;
    NFA& operator=(const NFA&) = delete;

    NFA(NFA&& rhs);
    NFA& operator=(NFA&& rhs);


    void setStartState(StateType state);
    void addState(StateType state);
    void addFinalState(StateType index);
    bool addStateTransition(StateType begin_state, CheckerType&& value, StateType end_state);
    bool addStateTransition(StateType begin_state, const CheckerType& value, StateType end_state);
    void print(std::ostream& out);
    DFAConvertAnswer convert();

protected:
    StateType                   m_startingState;
    std::vector<StateType>      m_states;
    std::vector<StateType>      m_finalStates;
    std::vector<CheckerType>    m_checkers;
    StateTransitionTable        m_STM;
};

//NFA
template <class State, class Checker>
std::ostream& operator<<(std::ostream& out, NFA<State, Checker>& nfa)
{
    nfa.print(out);
    return  out;
}

template <class State, class Checker>
void NFA<State, Checker>::print(std::ostream& out)
{
    out << "Starting state: " << m_startingState << std::endl;
    //
    out << "States: ";
    for (State state: m_states)
    {
        out << state << " ";
    }
    out << std::endl;
    //
    out << "Final states: ";
    for (State state: m_finalStates)
    {
        out << state << " ";
    }
    out << std::endl;
    //
    out << "STM" << std::endl;
    for (const auto& from_state : m_STM)
    {
        out << from_state.first << " =>" << std::endl;
        for (const auto& input_mapping : *from_state.second)
        {
            out << std::string(4, ' ');
            input_mapping.first.get()->print(out);
            out <<  " =>" << input_mapping.second <<  std::endl;
        }
    }
}

template <class State, class Checker>
typename NFA<State, Checker>::DFAConvertAnswer
NFA<State, Checker>::convert()
{
    // TODO : rework this later.
    using DFAState = std::vector<State>;
    using StateRecord = std::pair<DFAState, State>;

    DFA<State, Checker> dfa;
    std::vector<StateRecord> stack;
    State stateGenerator = 1;
    boost::container::flat_map<DFAState, State> created_states;
    boost::container::flat_map<State, std::vector<State>> nfa_to_dfa_final_state_map;

    dfa.addState(m_startingState);
    dfa.setStartState(m_startingState);
    stack.emplace_back(DFAState{m_startingState}, 0);
    created_states[DFAState{0}] = 0;

    std::cout << "Starting converting NFA to DFA" << std::endl;
    while (!stack.empty())
    {
        std::vector<Checker> ckecked_inputs;
        StateRecord currentState = stack.back();
        stack.pop_back();

        std::cout << "next unmarked state" << currentState.first << ":" << currentState.second << std::endl;
        std::cout << "already checked inputs" << ckecked_inputs << std::endl;

        for (auto it = currentState.first.begin(); it != currentState.first.end(); ++it)
        {
            State state = *it;
            std::cout << "currently checked state " << state << " " << m_STM[state].get() << std::endl;
            if (m_STM[state].get() == nullptr) continue;
            auto& val = (*m_STM[state]);
            for (auto input : val)
            {
                StateRecord nextState;
                std::cout << "currently checked input " << input.first << std::endl;
                if (std::find_if(ckecked_inputs.begin(), ckecked_inputs.end(), [&input](const Checker& inputInCont){ return *input.first == *inputInCont; }) == ckecked_inputs.end())
                {
                    std::cout << "new input " << std::endl;
                    ckecked_inputs.push_back(input.first);
                    // get new state
                    Utility::set_union(nextState.first, input.second);
                    for (auto it2 = ++currentState.first.begin(); it2 != currentState.first.end(); ++it2)
                    {
                        if (m_STM[*it2] == nullptr) continue;
                        auto& input_seq = (*m_STM[*it2]);

                        if (input_seq.find(input.first) != input_seq.end())
                        {
                            Utility::set_union(nextState.first, (*m_STM[*it2])[input.first]);
                        }
                    }
                    std::cout << "next state" << nextState.first << std::endl;
                    // record it
                    typename boost::container::flat_map<DFAState, State>::const_iterator pos;
                    if ((pos = created_states.find(nextState.first)) == created_states.end())
                    {
                        nextState.second = stateGenerator++;
                        stack.push_back(nextState);
                        created_states[nextState.first] = nextState.second;
                        std::cout << "alocated states" << std::endl;
                        //Check if it is fina state
                        StateRecord finalStateTransition;
                        finalStateTransition.second = nextState.second;
                        bool finalState = false;
                        for (auto state : nextState.first)
                        {
                            for (auto final_state : m_finalStates)
                            {
                                if (state == final_state)
                                {
                                    finalStateTransition.first.push_back(final_state);
                                }
                            }
                        }
                        //
                        if (!finalStateTransition.first.empty())
                        {
                            std::cout << "new final state " << nextState.second << std::endl;
                            dfa.addFinalState(nextState.second);
                            finalStateTransition.second = nextState.second;
                            nfa_to_dfa_final_state_map[finalStateTransition.second] = finalStateTransition.first;
                        }
                        else
                        {
                            std::cout << "new ordinary state " << nextState.second << std::endl;
                            dfa.addState(nextState.second);
                        }
                    }
                    else
                    {
                        nextState.second = (*pos).second;
                    }
                    dfa.addStateTransition(currentState.second, input.first, nextState.second);
                    std::cout << "creating new transition: " << currentState.second << " => " << nextState.second  << std::endl;
                }
            }
        }
    }
    return std::make_pair(std::move(dfa), std::move(nfa_to_dfa_final_state_map));
}

template <class State, class _Checker>
NFA<State, _Checker>& NFA<State, _Checker>::operator=(NFA<State, _Checker>&& rhs)
{
    m_startingState = rhs.m_startingState;
    m_states = std::move(rhs.m_states);
    m_finalStates = std::move(rhs.m_finalStates);
    m_STM = std::move(rhs.m_STM);
}

template <class State, class _Checker>
NFA<State, _Checker>::NFA(NFA&& rhs):
    m_startingState(rhs.m_startingState),
    m_states(std::move(rhs.m_states)),
    m_finalStates(std::move(rhs.m_finalStates)),
    m_STM(std::move(rhs.m_STM))
{
}

template <class State, class _Checker>
NFA<State, _Checker>::NFA()
{

}

template <class State, class _Checker>
void
NFA<State, _Checker>::setStartState(StateType state)
{
    m_startingState = state;
}

template <class State, class _Checker>
void
NFA<State, _Checker>::addState(StateType state)
{
    m_states.emplace_back(state);
}

template <class State, class _Checker>
void
NFA<State, _Checker>::addFinalState(StateType state)
{
    m_states.emplace_back(state);
    m_finalStates.emplace_back(state);
}

template <class State, class Checker>
bool
NFA<State, Checker>::addStateTransition(StateType begin_state, Checker&& value, StateType end_state)
{
    (*m_STM[begin_state].get())[std::move(value)].push_back(end_state);
    return true;
}

template <class State, class Checker>
bool
NFA<State, Checker>::addStateTransition(StateType begin_state, const Checker& value, StateType end_state)
{
    auto& input = m_STM[begin_state];
    if (input.get() == nullptr) input.reset(new InputCollection);
    auto& val = (*input)[value];
    val.push_back(end_state);
    return true;
}

#endif /* NFA_h */
