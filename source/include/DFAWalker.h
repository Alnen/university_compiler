#ifndef DFAWALKER_H
#define DFAWALKER_H

#include "DFA.h"

template <class State, class Checker>
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

// DFA::WALKER
template <class State, class Checker>
void
Walker<State, Checker>::reset()
{
    m_currentState = m_pDFA->m_startingState;
}

template <class State, class Checker>
bool
Walker<State, Checker>::goNextState(char input)
{

    State next_state = (*m_pDFA).m_STM[m_currentState];
    m_currentState = next_state;
    return true;
}

template <class State, class Checker>
typename DFA<State, Checker>::StateType
Walker<State, Checker>::getCurrentState() const
{
    return m_currentState;
}

template <class State, class Checker>
bool
Walker<State, Checker>::isCurrentStateFinal() const
{
    auto position = std::find((*m_pDFA).m_finalStateList.begin(), (*m_pDFA).m_finalStateList.end(), m_currentState);
    return (position != (*m_pDFA).m_finalStateList.end());
}

#endif // DFAWALKER_H
