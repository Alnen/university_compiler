#ifndef DFAWALKER_H
#define DFAWALKER_H

#include "DFA.h"

template <class State, class Checker>
class Walker
{
public:
    void reset();
    bool goNextState(char input);

    State getCurrentState() const;
    bool isCurrentStateFinal() const;

    Walker(DFA<State, Checker>* pDFA);
    Walker(const Walker&) = delete;
    Walker& operator=(const Walker&) = delete;

protected:
    DFA<State, Checker>* m_pDFA;
    State m_currentState;
};

// DFA::WALKER
template <class State, class Checker>
Walker<State, Checker>::Walker(DFA<State, Checker>* pDFA):
    m_currentState(pDFA->m_startingState),
    m_pDFA(pDFA)
{

}

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
    const auto& input_mapping = (*m_pDFA).m_STM.find(m_currentState);
    if (input_mapping == (*m_pDFA).m_STM.end())
    {
        return false;
    }

    for (const auto& mapping : *(*input_mapping).second)
    {
        if ((*mapping.first)(input))
        {
            m_currentState = mapping.second;
            return true;
        }
    }

    return false;
}

template <class State, class Checker>
State Walker<State, Checker>::getCurrentState() const
{
    return m_currentState;
}

template <class State, class Checker>
bool
Walker<State, Checker>::isCurrentStateFinal() const
{
    return (std::find((*m_pDFA).m_finalStates.begin(), (*m_pDFA).m_finalStates.end(), m_currentState) != (*m_pDFA).m_finalStates.end());
}

#endif // DFAWALKER_H
