//
//  DFA.h
//  LexerUni
//
//  Created by Alnen on 08.10.15.
//  Copyright © 2015 Alnen. All rights reserved.
//

#ifndef DFA_h
#define DFA_h

#include <memory>
#include <utility>
#include <vector>

#include "Map.h"


template <class State, class Input>
class DFA
{
public:
    using StateIndex = size_t;
    
    using StateMapping = Map<State, StateIndex>;
    using FinalStateList = std::vector<StateIndex>;
    
    using InputCollection = Map<Input, StateIndex>;
    using StateTransitionTable = Map<StateIndex, std::unique_ptr<InputCollection>>;

protected:
    class Walker
    {
        using StateIndex = DFA::StateIndex;

        void reset();
        bool goNextState(Input input);

        StateIndex getCurrentState() const;
        bool isCurrentStateFinal() const;

        Walker(const Walker&) = delete;
        Walker& operator=(const Walker&) = delete;

    protected:
        Walker(DFA* pDFA);

        DFA* m_pDFA;
        StateIndex m_currentState;
    };

public:
    DFA();
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;
    
    StateIndex addState(State state);
    bool addFinalStateData(StateIndex index);
    bool addStateTransition(StateIndex begin_state, StateIndex end_state, Input value);
    
    StateIndex getStateIndex(State state) const;
    State getState(StateIndex state_index);
    
    void optimize();
    Walker getWalker() const;
    
    //combine operators
    
protected:
    StateIndex              m_beginStateIndex;
    StateMapping            m_states;
    FinalStateList          m_finalStateList;
    StateTransitionTable    m_STM;
    
    StateIndex              m_stateIndexGenerator = 0;
};


//DFA
template <class State, class Input>
DFA<State, Input>::DFA()
{

}

template <class State, class Input>
typename DFA<State, Input>::StateIndex
DFA<State, Input>::addState(State state)
{
    StateIndex index = m_stateIndexGenerator++;
    m_states.emplace_back(state, m_stateIndexGenerator);
    return index;
}

template <class State, class Input>
bool
DFA<State, Input>::addFinalStateData(StateIndex index)
{
    m_finalStateList.emplace_back(index);
    return true;
}

template <class State, class Input>
bool
DFA<State, Input>::addStateTransition(StateIndex begin_state, StateIndex end_state, Input value)
{
    (*m_STM[begin_state])[value].push_back(end_state);
    return true;
    
}

template <class State, class Input>
typename DFA<State, Input>::StateIndex
DFA<State, Input>::getStateIndex(State state) const
{
    auto position = std::find_if(
                m_states.begin(),
                m_states.end(),
                [&state](auto pair)
                {
                    return pair.first == state;
                }
    );
    return (StateIndex) std::distance(m_states.begin(), position);
}

template <class State, class Input>
State
DFA<State, Input>::getState(StateIndex state_index)
{
    auto position = std::find_if(
                m_states.begin(),
                m_states.end(),
                [state_index](auto pair)
                {
                    return pair.second == state_index;
                }
    );
    return (StateIndex) std::distance(m_states.begin(), position);
}

template <class State, class Input>
void
DFA<State, Input>::optimize()
{
    std::sort(
                m_states.begin(),
                m_states.end(),
                [](auto& lhs, auto& rhs)
                {
                    return std::less<>(lhs.second, rhs.second);
                }
    );
    std::sort(m_finalStateList.begin(), m_finalStateList.end());
    std::sort(
                m_STM.begin(),
                m_STM.begin(),
                [](auto& lhs, auto& rhs)
                {
                    return std::less<>(lhs.first, rhs.firsh);
                }
    );
    for (auto it = m_STM.begin(); it != m_STM.end(); ++it)
    {
        auto& current_input_collection = (*(*it).second);
        std::sort(
                    current_input_collection.begin() ,
                    current_input_collection.end(), [](auto& lhs, auto& rhs)
                    {
                        return std::less<>(lhs.first, rhs.firsh);
                    }
        );
    }
}
template <class State, class Input>
typename
DFA<State, Input>::Walker DFA<State, Input>::getWalker() const
{
    return Walker(this);
}

//DFA::WALKER
template <class State, class Input>
void
DFA<State, Input>::Walker::reset()
{
    m_currentState = m_pDFA->m_beginStateIndex;
}

template <class State, class Input>
bool
DFA<State, Input>::Walker::goNextState(Input input)
{
    State next_state = (*m_pDFA).m_STM[m_currentState];\
    m_currentState = next_state;
    return true;
}

template <class State, class Input>
typename DFA<State, Input>::Walker::StateIndex
DFA<State, Input>::Walker::getCurrentState() const
{
    return m_currentState;
}

template <class State, class Input>
bool
DFA<State, Input>::Walker::isCurrentStateFinal() const
{
    auto position = std::find((*m_pDFA).m_finalStateList.begin(), (*m_pDFA).m_finalStateList.end(), m_currentState);
    return (position != (*m_pDFA).m_finalStateList.end());
}

#endif /* DFA_h */
