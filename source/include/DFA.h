#ifndef DFA_h
#define DFA_h

#include <algorithm>
#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/container/flat_map.hpp>

template <class State, class _Checker>
class DFA
{
public:     
    using StateType = State;
    using CheckerType = _Checker;
    using InputCollection = boost::container::flat_map<CheckerType, State>;
    using StateTransitionTable = boost::container::flat_map<State, std::shared_ptr<InputCollection>>;

    DFA();
    DFA(DFA&& rhs);
    DFA& operator=(DFA&& rhs);

    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;

    void setStartState(StateType state);
    void addState(StateType state);
    void addFinalState(StateType index);
    bool addStateTransition(StateType begin_state, CheckerType&& value, StateType end_state);
    bool addStateTransition(StateType begin_state, const CheckerType&  value, StateType end_state);
    
    void print(std::ostream& out) const;
    void print_plantuml(std::ostream& out) const;
    StateType startState() const { return m_startingState; }

    std::pair<const InputCollection*, bool> operator[](State state) const;

protected:
    StateType                   m_startingState;
    std::vector<StateType>      m_states;
    std::vector<StateType>      m_finalStates;
    StateTransitionTable        m_STM;
};


//DFA
template <class State, class Checker>
DFA<State, Checker>::DFA(DFA&& rhs):
    m_startingState(std::move(rhs.m_startingState)),
    m_states(std::move(rhs.m_states)),
    m_finalStates(std::move(rhs.m_finalStates)),
    m_STM(std::move(rhs.m_STM))
{
}

template <class State, class Checker>
DFA<State, Checker>&
DFA<State, Checker>::operator=(DFA&& rhs)
{
    m_startingState = rhs.m_startingState;
    m_states = std::move(rhs.m_states);
    m_finalStates = std::move(rhs.m_finalStates);
    m_STM = std::move(rhs.m_STM);
    return *this;
}

template <class State, class Checker>
std::ostream&
operator<<(std::ostream& out, const DFA<State, Checker>& dfa)
{
    dfa.print(out);
    return  out;
}

template <class State, class Checker>
void DFA<State, Checker>::print(std::ostream& out) const
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
            input_mapping.first->print(out);
            out <<  " =>" << input_mapping.second <<  std::endl;
        }
    }
}

template <class State, class Checker>
void DFA<State, Checker>::print_plantuml(std::ostream& out) const
{
    out << "@startuml\n";
    out << "[*] --> 0\n";
    for (const auto& from_state : m_STM)
    {
        for (const auto& input_mapping : *from_state.second)
        {
            out << from_state.first << " -->" << input_mapping.second << ":";
            input_mapping.first->print(out);
            out << "\n";
            if ( from_state.first != input_mapping.second &&
                 std::find(m_finalStates.begin(), m_finalStates.end(), input_mapping.second) != m_finalStates.end())
            {
                out << input_mapping.second << " --> [*] : Success\n";
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
DFA<State, Checker>::addStateTransition(StateType begin_state, const CheckerType& value, StateType end_state)
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
std::pair<const typename DFA<State, Checker>::InputCollection*, bool>
DFA<State, Checker>::operator[](State state) const
{
    const InputCollection* input_mapping = m_STM[state].get();
    if (input_mapping == nullptr)
    {
        return std::make_pair(input_mapping, false);
    }
    else
    {
        return std::make_pair(input_mapping, true);
    }
}
#endif /* DFA_h */
