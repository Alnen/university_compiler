#ifndef NFATODFA_h
#define NFATODFA_h

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "Map.h"
#include "DFA.h"
#include "NFA.h"
#include "RegexASTAnnotationEvaluator.h"
#include <iostream>

class NFAtoDFA
{
public:
    template<class State, class Checker>
    DFA<State, Checker> convert(NFA<State, Checker> nfa)
    {
        DFA<State, Checker> dfa;






        return dfa;
    }
};


#endif // NFATODFA_h
