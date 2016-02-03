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
    TERMINAL_START_INDEX   = 0,
    CI,                    // 1
    CJ,                    // 2
    ENDOFFILE,             // 3
    ERROR,                 // 4
    ID,                    // 5
    OPAS,                  // 6
    OPDIV,                 // 7
    OPEQ,                  // 8
    OPGE,                  // 9
    OPGT,                  // 10
    OPLE,                  // 11
    OPLT,                  // 12
    OPMINUS,               // 13
    OPMUL,                 // 14
    OPNE,                  // 15
    OPPLUS,                // 16
    PASS,                  // 17
    RWAR,                  // 18
    RWB,                   // 19
    RWCM,                  // 20
    RWCN,                  // 21
    RWCX,                  // 22
    RWDO,                  // 23
    RWDT,                  // 24
    RWEL,                  // 25
    RWEND,                 // 26
    RWFOR,                 // 27
    RWFR,                  // 28
    RWFUN,                 // 29
    RWGT,                  // 30
    RWIF,                  // 31
    RWINT,                 // 32
    RWINTOP,               // 33
    RWIR,                  // 34
    RWLA,                  // 35
    RWLB,                  // 36
    RWLN,                  // 37
    RWLO,                  // 38
    RWOF,                  // 39
    RWOP,                  // 40
    RWPR,                  // 41
    RWPRC,                 // 42
    RWRAT,                 // 43
    RWRC,                  // 44
    RWRD,                  // 45
    RWSM,                  // 46
    RWTH,                  // 47
    RWTO,                  // 48
    RWTP,                  // 49
    RWV,                   // 50
    RWWR,                  // 51
    SRCA,                  // 52
    SRCN,                  // 53
    SRLB,                  // 54
    SRLCB,                 // 55
    SRLP,                  // 56
    SRRB,                  // 57
    SRRCB,                 // 58
    SRRP,                  // 59
    SRSM,                  // 60
    SRSP,                  // 61
    TEXT,                  // 62
    TERMINAL_FINISH_INDEX  // 63
};


boost::bimap<std::string, TokenType>& reserved_words();
boost::container::flat_map<TokenType, std::string>& tokenTypeMapping();

}
}

#endif // TERMINALS_H
