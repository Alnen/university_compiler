#ifndef DFA_h
#define DFA_h

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <iosfwd>
#include "Map.h"
#include "RegexASTAnnotationEvaluator.h"

template <class State, class _Checker>
class DFA
{
public:     
    using StateType = State;
    using CheckerType = _Checker;
    using InputChecker = Lexer::IInputChecker;
    using InputCollection = Map<CheckerType, State>;
    using StateTransitionTable = Map<State, std::shared_ptr<InputCollection>>;

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
        Walker(DFA* pDFA);

        DFA* m_pDFA;
        StateType m_currentState;
    };

public:
    DFA();
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;

    DFA(DFA&& rhs):
        m_startingState(rhs.m_startingState),
        m_states(std::move(rhs.m_states)),
        m_finalStates(std::move(rhs.m_finalStates)),
        m_STM(std::move(rhs.m_STM))
    {
    }

    DFA& operator=(DFA&& rhs)
    {
        m_startingState = rhs.m_startingState;
        m_states = std::move(rhs.m_states);
        m_finalStates = std::move(rhs.m_finalStates);
        m_STM = std::move(rhs.m_STM);
        return *this;
    }

    void setStartState(StateType state);
    void addState(StateType state);
    void addFinalState(StateType index);
    bool addStateTransition(StateType begin_state, CheckerType&& value, StateType end_state);
    bool addStateTransition(StateType begin_state, CheckerType&  value, StateType end_state);
    
    std::ostream& operator<<(std::ostream& out);
    void print_plantuml(std::ostream& out);

    Walker getWalker() const;
    //combine operators
    
protected:
    void print(std::ostream& out);

    StateType                   m_startingState;
    std::vector<StateType>      m_states;
    std::vector<StateType>      m_finalStates;
    StateTransitionTable        m_STM;
};


//DFA
template <class State, class Checker>
std::ostream& operator<<(std::ostream& out, DFA<State, Checker>& dfa)
{
    dfa << out;
    return  out;
}


template <class State, class Checker>
std::ostream& DFA<State, Checker>::operator<<(std::ostream& out)
{
    print(out);
    return  out;
}


template <class State, class Checker>
void DFA<State, Checker>::print(std::ostream& out)
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
void DFA<State, Checker>::print_plantuml(std::ostream& out)
{
    out << "@startuml\n";
    out << "[*] --> 0\n";
    for (const auto& from_state : m_STM)
    {
        for (const auto& input_mapping : *boost::get<1>(from_state))
        {
            out << boost::get<0>(from_state) << " -->" << boost::get<1>(input_mapping) << ":";
            boost::get<0>(input_mapping).get()->print(out);
            out << "\n";
            if (std::find(m_finalStates.begin(), m_finalStates.end(), boost::get<1>(input_mapping)) != m_finalStates.end())
            {
                out << boost::get<1>(input_mapping) << " --> [*] : Success\n";
            }
        }
    }
    out << "@enduml\n";
}

template <class State, class Checker>
DFA<State, Checker>::DFA()
{

}

template <class State, class Checker>
void
DFA<State, Checker>::setStartState(StateType state)
{
    m_startingState = state;
}

template <class State, class Checker>
void
DFA<State, Checker>::addState(StateType state)
{
    m_states.emplace_back(state);
}

template <class State, class Checker>
void
DFA<State, Checker>::addFinalState(StateType state)
{
    m_states.emplace_back(state);
    m_finalStates.emplace_back(state);
}

template <class State, class Checker>
bool
DFA<State, Checker>::addStateTransition(StateType begin_state, CheckerType&& value, StateType end_state)
{
    (*m_STM[begin_state])[std::move(value)] = end_state;
    return true;
}

template <class State, class Checker>
bool
DFA<State, Checker>::addStateTransition(StateType begin_state, CheckerType& value, StateType end_state)
{
    auto& input = m_STM[begin_state];
    if (input.get() == nullptr) input.reset(new InputCollection);
    //std::cout << (*input).size() << std::endl;
    auto& val = (*input)[value];
    //std::cout << begin_state << end_state << std::endl;
    val = (end_state);
    return true;
}


template <class State, class Checker>
typename
DFA<State, Checker>::Walker DFA<State, Checker>::getWalker() const
{
    return Walker(this);
}

// DFA::WALKER
template <class State, class Checker>
void
DFA<State, Checker>::Walker::reset()
{
    m_currentState = m_pDFA->m_startingState;
}

template <class State, class Checker>
bool
DFA<State, Checker>::Walker::goNextState(char input)
{

    State next_state = (*m_pDFA).m_STM[m_currentState];
    m_currentState = next_state;
    return true;
}

template <class State, class Checker>
typename DFA<State, Checker>::StateType
DFA<State, Checker>::Walker::getCurrentState() const
{
    return m_currentState;
}

template <class State, class Checker>
bool
DFA<State, Checker>::Walker::isCurrentStateFinal() const
{
    auto position = std::find((*m_pDFA).m_finalStateList.begin(), (*m_pDFA).m_finalStateList.end(), m_currentState);
    return (position != (*m_pDFA).m_finalStateList.end());
}

#endif /* DFA_h */
