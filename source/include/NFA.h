#ifndef NFA_h
#define NFA_h

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <iosfwd>

#include "Map.h"
#include "RegexASTAnnotationEvaluator.h"
#include <iostream>
#include "DFA.h"
#include "Utility.h"

template <class State, class _Checker>
class NFA
{
public:
    using StateType = State;
    using CheckerType = _Checker;
    using InputChecker = Lexer::InputCheckerType;
    using InputCollection = Map<CheckerType, std::vector<State>, PointerEquality>;
    using StateTransitionTable = Map<State, std::unique_ptr<InputCollection>>; // TODO : delete pointer comparison

protected:
    class Walker
    {
        void reset();
        bool goNextState(char input);

        StateType getCurrentState() const;
        bool isCurrentStateFinal() const;

        Walker(const Walker&) = delete;
        Walker& operator=(const Walker&) = delete;

    protected:
        Walker(NFA* pNFA);

        NFA* m_pNFA;
        StateType m_currentState;
    };

public:
    NFA();
    NFA(const NFA&) = delete;
    NFA& operator=(const NFA&) = delete;

    NFA(NFA&& rhs);
    NFA& operator=(NFA&& rhs);


    void setStartState(StateType state);
    void addState(StateType state);
    void addFinalState(StateType index);
    bool addStateTransition(StateType begin_state, CheckerType&& value, StateType end_state);
    bool addStateTransition(StateType begin_state, CheckerType& value, StateType end_state);
    void print(std::ostream& out);
    DFA<State, CheckerType> convert();

    Walker getWalker() const;
    //combine operators

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
        out << boost::get<0>(from_state) << " =>" << std::endl;
        for (const auto& input_mapping : *boost::get<1>(from_state))
        {
            out << std::string(4, ' ');
            boost::get<0>(input_mapping).get()->print(out);
            out <<  " =>" << boost::get<1>(input_mapping) <<  std::endl;
        }
    }
}

template <class State, class Checker>
DFA<State, Checker> NFA<State, Checker>::convert()
{
    // TODO : rework this later.
    using DFAState = std::vector<State>;
    using StateRecord = std::pair<DFAState, State>;
    DFA<State, Checker> dfa;

    std::vector<StateRecord> stack;

    State stateGenerator = 1;

    dfa.addState(m_startingState);
    dfa.setStartState(m_startingState);
    stack.emplace_back(DFAState{m_startingState}, 0);


    Map<DFAState, State> created_states;
    created_states[DFAState{0}] = 0;
    std::vector<std::pair<DFAState, State>> final_states_mapping;

    std::cout << "Starting converting NFA to DFA" << std::endl;
    while (!stack.empty())
    {
        StateRecord currentState = stack.back();
        stack.pop_back();
        std::cout << "next unmarked state" << currentState.first << ":" << currentState.second << std::endl;

        std::vector<Checker> ckecked_inputs;
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
                std::cout << "currently checked input " << boost::get<0>(input) << std::endl;
                if (std::find_if(ckecked_inputs.begin(), ckecked_inputs.end(), [&input](const auto& inputInCont){ return boost::get<0>(input) == inputInCont; }) == ckecked_inputs.end())
                {
                    ckecked_inputs.push_back(boost::get<0>(input));
                    std::cout << "new input " << std::endl;
                    // get new state
                    Utility::set_union(nextState.first, boost::get<1>(input));
                    for (auto it2 = ++currentState.first.begin(); it2 != currentState.first.end(); ++it2)
                    {
                        auto& input_seq = (*m_STM[*it2]);
                        if (std::find_if(input_seq.key_begin(), input_seq.key_end(), [&input](const auto& inputInCont){ return boost::get<0>(input) == inputInCont; }) != input_seq.key_end())
                        {
                            Utility::set_union(nextState.first, (*m_STM[*it2])[boost::get<0>(input)]);
                        }
                    }
                    std::cout << "next state" << nextState.first << std::endl;
                    // record it
                    typename Map<DFAState, State>::key_iterator pos;
                    if ((pos = std::find(created_states.key_begin(), created_states.key_end(), nextState.first)) == created_states.key_end())
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
                            final_states_mapping.emplace_back(finalStateTransition);
                        }
                        else
                        {
                            std::cout << "new ordinary state " << nextState.second << std::endl;
                            dfa.addState(nextState.second);
                        }
                    }
                    else
                    {
                        std::cout << "found next state name :";
                        for (auto id : *pos) std:: cout << id << " ";
                        std::cout << std::endl;
                        std::cout << "offset " << std::distance(created_states.key_begin(), pos) << std::endl;
                        auto state_it = created_states.value_begin();
                        std::advance(state_it, std::distance(created_states.key_begin(), pos));
                        nextState.second = *state_it;
                        std::cout << "old starte " << nextState.second << std::endl;
                    }
                    dfa.addStateTransition(currentState.second, boost::get<0>(input), nextState.second);
                    std::cout << "creating new transition: " << currentState.second << " => " << nextState.second  << std::endl;
                }
            }
        }
    }

    return dfa;
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
NFA<State, Checker>::addStateTransition(StateType begin_state, Checker& value, StateType end_state)
{
    //std::cout << begin_state << end_state << std::endl;
    auto& input = m_STM[begin_state];
    if (input.get() == nullptr) input.reset(new InputCollection);
    //std::cout << (*input).size() << std::endl;
    auto& val = (*input)[value];
    //std::cout << begin_state << end_state << std::endl;
    val.push_back(end_state);
    return true;
}

template <class State, class _Checker>
typename
NFA<State, _Checker>::Walker NFA<State, _Checker>::getWalker() const
{
    return Walker(this);
}

// NFA::WALKER
template <class State, class _Checker>
void
NFA<State, _Checker>::Walker::reset()
{
    m_currentState = m_pNFA->m_startingState;
}

template <class State, class _Checker>
bool
NFA<State, _Checker>::Walker::goNextState(char input)
{

    State nextState = (*m_pNFA).m_STM[m_currentState];
    m_currentState = nextState;
    return true;
}

template <class State, class _Checker>
typename NFA<State, _Checker>::StateType
NFA<State, _Checker>::Walker::getCurrentState() const
{
    return m_currentState;
}

template <class State, class _Checker>
bool
NFA<State, _Checker>::Walker::isCurrentStateFinal() const
{
    auto position = std::find((*m_pNFA).m_finalStateList.begin(), (*m_pNFA).m_finalStateList.end(), m_currentState);
    return (position != (*m_pNFA).m_finalStateList.end());
}

#endif /* NFA_h */
