#ifndef TERMINALS_H
#define TERMINALS_H

#include <boost/container/flat_map.hpp>
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

std::vector<boost::container::flat_map<TokenType, std::string>::value_type> tokenTypePairsMapping =
{
    { TokenType::ID,        "ID" }, // 1
    { TokenType::CJ,        "CJ" }, // 2
    { TokenType::CI,        "CI" }, // 3
    { TokenType::RWLA,      "RWLA" }, // 4
    { TokenType::RWAR,      "RWAR" }, // 5
    { TokenType::RWB,       "RWB" }, // 6
    { TokenType::RWCM,      "RWCM" }, // 7
    { TokenType::RWCN,      "RWCN" }, // 8
    { TokenType::RWDO,      "RWDO" }, // 9
    { TokenType::RWDT,      "RWDT" }, // 10
    { TokenType::RWEL,      "RWEL" }, // 11
    { TokenType::RWEND,     "RWEND" }, // 12
    { TokenType::RWFOR,     "RWFOR" }, // 13
    { TokenType::RWFR,      "RWFR" }, // 14
    { TokenType::RWGT,      "RWGT" }, // 15
    { TokenType::RWIF,      "RWIF" }, // 16
    { TokenType::RWINT,     "RWINT" }, // 17
    { TokenType::RWIR,      "RWIR" }, // 18
    { TokenType::RWLB,      "RWLB" }, // 19
    { TokenType::RWLN,      "RWLN" }, // 20
    { TokenType::RWOF,      "RWOF" }, // 21
    { TokenType::RWOP,      "RWOP" }, // 22
    { TokenType::RWLO,      "RWLO" }, // 23
    { TokenType::RWPR,      "RWPR" }, // 24
    { TokenType::RWRAT,     "RWRAT" }, // 25
    { TokenType::RWRD,      "RWRD" }, // 26
    { TokenType::RWSM,      "RWSM" }, // 27
    { TokenType::RWTH,      "RWTH" }, // 28
    { TokenType::RWTO,      "RWTO" }, // 29
    { TokenType::RWTP,      "RWTP" }, // 30
    { TokenType::RWV,       "RWV" }, // 31
    { TokenType::RWWR,      "RWWR" }, // 32
    { TokenType::RWCX,      "RWCX" }, // 33
    { TokenType::RWFUN,     "RWFUN" }, // 34
    { TokenType::RWIM,      "RWIM" }, // 35
    { TokenType::RWRE,      "RWRE" }, // 36
    { TokenType::RWINTOP,   "RWINTOP" }, // 37
    { TokenType::RWPRC,     "RWPRC" }, // 38
    { TokenType::RWDN,      "RWDN" }, // 39
    { TokenType::RWN,       "RWN" }, // 40
    { TokenType::OPPLUS,    "OPPLUS" }, // 41
    { TokenType::OPMINUS,   "OPMINUS" }, // 42
    { TokenType::OPMUL,     "OPMUL" }, // 43
    { TokenType::OPDIV,     "OPDIV" }, // 44
    { TokenType::OPGT,      "OPGT" }, // 45
    { TokenType::OPLT,      "OPLT" }, // 46
    { TokenType::OPGE,      "OPGE" }, // 47
    { TokenType::OPLE,      "OPLE" }, // 48
    { TokenType::OPEQ,      "OPEQ" }, // 49
    { TokenType::OPNE,      "OPNE" }, // 50
    { TokenType::OPAS,      "OPAS" }, // 51
    { TokenType::SRLP,      "SRLP" }, // 52
    { TokenType::SRRP,      "SRRP" }, // 53
    { TokenType::SRLB,      "SRLB" }, // 54
    { TokenType::SRRB,      "SRRB" },       // 55
    { TokenType::SRSM,      "SRSM" },       // 56
    { TokenType::SRCN,      "SRCN" },       // 57
    { TokenType::SRSP,      "SRSP" },       // 58
    { TokenType::SRCA,      "SRCA" },       // 59
    { TokenType::SRLCB,     "SRLCB" },      // 60
    { TokenType::SRRCB,     "SRRCB" },      // 61
    { TokenType::ERROR,     "ERROR" },      // 62
    { TokenType::ENDOFFILE, "ENDOFFILE" }   // 63
};
boost::container::flat_map<TokenType, std::string> tokenTypeMapping(tokenTypePairsMapping.begin(), tokenTypePairsMapping.end());

// reserwed words map
std::vector<boost::bimap<std::string, TokenType>::value_type> reserved_words_map_items =
{
    { "and",        TokenType::RWLA   },
    { "array",      TokenType::RWAR   },
    { "begin",      TokenType::RWB    },
    { "common",     TokenType::RWCM   },
    { "complex",    TokenType::RWCX   },
    { "const",      TokenType::RWCN   },
    { "do",         TokenType::RWDO   },
    { "denominator",TokenType::RWDN   },
    { "downto",     TokenType::RWDT   },
    { "else",       TokenType::RWEL   },
    { "end",        TokenType::RWEND  },
    { "for",        TokenType::RWFOR  },
    { "frac",       TokenType::RWFR   },
    { "function",   TokenType::RWFUN  },
    { "goto",       TokenType::RWGT   },
    { "if",         TokenType::RWIF   },
    { "im",         TokenType::RWIM   },
    { "int",        TokenType::RWINTOP},
    { "integer",    TokenType::RWINT  },
    { "irregular",  TokenType::RWIR   },
    { "im",         TokenType::RWIM   },
    { "j",          TokenType::CJ     },
    { "label",      TokenType::RWLB   },
    { "numerator",  TokenType::RWN    },
    { "not",        TokenType::RWLN   },
    { "of",         TokenType::RWOF   },
    { "operator",   TokenType::RWOP   },
    { "or",         TokenType::RWLO   },
    { "program",    TokenType::RWPR   },
    { "procedure",  TokenType::RWPRC  },
    { "rational",   TokenType::RWRAT  },
    { "re",         TokenType::RWRE   },
    { "read",       TokenType::RWRD   },
    { "simplify",   TokenType::RWSM   },
    { "then",       TokenType::RWTH   },
    { "to",         TokenType::RWTO   },
    { "type",       TokenType::RWTP   },
    { "var",        TokenType::RWV    },
    { "write",      TokenType::RWWR   }
};
boost::bimap<std::string, TokenType> reserved_words(reserved_words_map_items.begin(), reserved_words_map_items.end());

}
}

#endif // TERMINALS_H
