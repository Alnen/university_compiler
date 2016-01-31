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
    RWRD,                          // 36
    RWRC,                          // 37
    RWRE,                          // 37
    RWINTOP,                       // 38
    RWPRC,                         // 39
    OPPLUS,                        // 40
    OPMINUS,                       // 41
    OPMUL,                         // 42
    OPDIV,                         // 43
    OPGT,                          // 44
    OPLT,                          // 45
    OPGE,                          // 46
    OPLE,                          // 47
    OPEQ,                          // 48
    OPNE,                          // 49
    OPAS,                          // 50
    SRLP,                          // 51
    SRRP,                          // 52
    SRLB,                          // 53
    SRRB,                          // 54
    SRSM,                          // 55
    SRCN,                          // 56
    SRSP,                          // 57
    SRCA,                          // 58
    SRLCB,                         // 59
    SRRCB,                         // 60
    TEXT,                          // 61
    PASS,                          // 62
    ERROR,                         // 63
    ENDOFFILE,                     // 64
    TERMINAL_FINISH_INDEX          // 65
};


boost::bimap<std::string, TokenType>& reserved_words();
boost::container::flat_map<TokenType, std::string>& tokenTypeMapping();

}
}

#endif // TERMINALS_H
