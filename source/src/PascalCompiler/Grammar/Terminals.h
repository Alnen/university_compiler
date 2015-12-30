#ifndef TERMINALS_H
#define TERMINALS_H

#include <boost/container/flat_map.hpp>
#include <boost/bimap.hpp>
#include <string>
#include <vector>

namespace PascalCompiler {
namespace Grammar {

enum TokenType
{
    TERMINAL_START_INDEX           = 0,
    ID,                            // 1
    CJ,                            // 2
    CI,                            // 3
    RWLA,                          // 4
    RWAR,                          // 5
    RWB,                           // 6
    RWCM,                          // 7
    RWCN,                          // 8
    RWDO,                          // 9
    RWDT,                          // 10
    RWEL,                          // 11
    RWEND,                         // 12
    RWFOR,                         // 13
    RWFR,                          // 14
    RWGT,                          // 15
    RWIF,                          // 16
    RWINT,                         // 17
    RWIR,                          // 18
    RWLB,                          // 19
    RWLN,                          // 20
    RWOF,                          // 21
    RWOP,                          // 22
    RWLO,                          // 23
    RWPR,                          // 24
    RWRAT,                         // 25
    RWRD,                          // 26
    RWSM,                          // 27
    RWTH,                          // 28
    RWTO,                          // 29
    RWTP,                          // 30
    RWV,                           // 31
    RWWR,                          // 32
    RWCX,                          // 33
    RWFUN,                         // 34
    RWIM,                          // 35
    RWRE,                          // 36
    RWINTOP,                       // 37
    RWPRC,                         // 38
    RWDN,                          // 39
    RWN,                           // 40
    OPPLUS,                        // 41
    OPMINUS,                       // 42
    OPMUL,                         // 43
    OPDIV,                         // 44
    OPGT,                          // 45
    OPLT,                          // 46
    OPGE,                          // 47
    OPLE,                          // 48
    OPEQ,                          // 49
    OPNE,                          // 50
    OPAS,                          // 51
    SRLP,                          // 52
    SRRP,                          // 53
    SRLB,                          // 54
    SRRB,                          // 55
    SRSM,                          // 56
    SRCN,                          // 57
    SRSP,                          // 58
    SRCA,                          // 59
    SRLCB,                         // 60
    SRRCB,                         // 61
    ERROR,                         // 62
    ENDOFFILE,                     // 63
    TERMINAL_FINISH_INDEX          // 64
};


boost::bimap<std::string, TokenType>& reserved_words();
boost::container::flat_map<TokenType, std::string>& tokenTypeMapping();

}
}

#endif // TERMINALS_H
