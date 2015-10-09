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


template <class State, class Input, class FinalStateData>
class DFA
{
public:
    using StateIndex = size_t;
    
    using StateMapping = Map<State, StateIndex>;
    using FinalStateDataMapping = Map<StateIndex, FinalStateData>;
    
    using StateIndexCollection = std::vector<StateIndex>;
    using InputCollection = Map<Input, std::unique_ptr<StateIndexCollection>>;
    using StateTransitionTable = Map<StateIndex, std::unique_ptr<InputCollection>>;
    //using Walker = ;
    
    DFA(const DFA&) = delete;
    DFA& operator=(const DFA&) = delete;
    
    StateIndex addState(State state);
    bool addFinalStateData(StateIndex index, FinalStateData data);
    bool addStateTransition(StateIndex begin_state, StateIndex end_state, Input value);
    
    StateIndex getStateIndex(State state) const;
    State getState(StateIndex state_index);
    FinalStateData getFinalStateData(StateIndex index) const;
    
    void optimize();
    
    //combine operators
    
protected:
    StateIndex              m_beginStateIndex;
    StateMapping            m_states;
    FinalStateDataMapping   m_finalStateDataMapping;
    StateTransitionTable    m_STM;
    
    StateIndex              m_stateIndexGenerator = 0;
    
protected:
    class Walker
    {
        using StateIndex = DFA::StateIndex;
        
        void reset();
        bool goNextState(Input input);
        
        StateIndex getCurrentState() const;
        
        Walker(const Walker&) = delete;
        Walker& operator=(const Walker&) = delete;
        
    protected:
        Walker(DFA* pDFA);
        
        DFA* m_pDFA;
        StateIndex m_currentState;
    };
    
};


//DFA

template <class State, class Input, class FinalStateData>
typename DFA<State, Input, FinalStateData>::StateIndex DFA<State, Input, FinalStateData>::addState(State state)
{
    StateIndex index = m_stateIndexGenerator++;
    m_states.emplace_back(state, m_stateIndexGenerator);
    return index;
}

template <class State, class Input, class FinalStateData>
bool DFA<State, Input, FinalStateData>::addFinalStateData(StateIndex index, FinalStateData data)
{
    m_finalStateDataMapping.emplace_back(index, data);
    return true;
}

template <class State, class Input, class FinalStateData>
bool DFA<State, Input, FinalStateData>::addStateTransition(StateIndex begin_state, StateIndex end_state, Input value)
{
    (*m_STM[begin_state])[value].push_back(end_state);
    return true;
    
}

template <class State, class Input, class FinalStateData>
typename DFA<State, Input, FinalStateData>::StateIndex DFA<State, Input, FinalStateData>::getStateIndex(State state) const
{
    auto position = std::find_if(m_states.begin(), m_states.end(), [&state](auto pair){ return pair.first == state; });
    return (StateIndex) std::distance(m_states.begin(), position);
}

template <class State, class Input, class FinalStateData>
State DFA<State, Input, FinalStateData>::getState(StateIndex state_index)
{
    auto position = std::find_if(m_states.begin(), m_states.end(), [state_index](auto pair){ return pair.second == state_index; });
    return (StateIndex) std::distance(m_states.begin(), position);
}

template <class State, class Input, class FinalStateData>
FinalStateData DFA<State, Input, FinalStateData>::getFinalStateData(StateIndex state_index) const
{
    auto position = std::find_if(m_finalStateDataMapping.begin(), m_finalStateDataMapping.end(), [state_index](auto pair){ return pair.first == state_index; });
    if (position != m_finalStateDataMapping.begin())
    {
        return (*position).second;
    }
    else
    {
        return FinalStateData();
    }
}

template <class State, class Input, class FinalStateData>
void DFA<State, Input, FinalStateData>::optimize()
{
    std::sort(m_states.begin(), m_states.end(), [](auto& lhs, auto& rhs){ return std::less<>(lhs.second, rhs.second); });
    std::sort(m_finalStateDataMapping.begin(), m_finalStateDataMapping.end(), [](auto& lhs, auto& rhs){ return std::less<>(lhs.first, rhs.firsh); });
    std::sort(m_STM.begin(), m_STM.begin(), [](auto& lhs, auto& rhs){ return std::less<>(lhs.first, rhs.firsh); });
    for (auto it = m_STM.begin(); it != m_STM.end(); ++it)
    {
        auto& current_input_collection = (*(*it).second);
        std::sort(current_input_collection.begin() , current_input_collection.end(), [](auto& lhs, auto& rhs){ return std::less<>(lhs.first, rhs.firsh); });
        for (auto inner_it = current_input_collection.begin(); inner_it != current_input_collection.end(); ++inner_it)
        {
            auto& current_end_state_collection = (*(*it).second);
            std::sort(current_end_state_collection.begin() , current_end_state_collection.end());
        }
    }
}

//DFA::WALKER
template <class State, class Input, class FinalStateData>
void DFA<State, Input, FinalStateData>::Walker::reset()
{
    m_currentState = m_pDFA->m_beginStateIndex;
}

template <class State, class Input, class FinalStateData>
bool DFA<State, Input, FinalStateData>::Walker::goNextState(Input input)
{
    return false;
    
}

template <class State, class Input, class FinalStateData>
typename DFA<State, Input, FinalStateData>::Walker::StateIndex DFA<State, Input, FinalStateData>::Walker::getCurrentState() const
{
    return 0;
}

#endif /* DFA_h */
