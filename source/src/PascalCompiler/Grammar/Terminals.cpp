#include "Terminals.h"

namespace PascalCompiler {
namespace Grammar {


boost::bimap<std::string, TokenType>& reserved_words()
{
    static boost::bimap<std::string, TokenType> reserved_words;
    static bool first_call = true;
    if (first_call)
    {
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
            { "downto",     TokenType::RWDT   },
            { "else",       TokenType::RWEL   },
            { "end",        TokenType::RWEND  },
            { "for",        TokenType::RWFOR  },
            { "frac",       TokenType::RWFR   },
            { "function",   TokenType::RWFUN  },
            { "goto",       TokenType::RWGT   },
            { "if",         TokenType::RWIF   },
            { "int",        TokenType::RWINTOP},
            { "integer",    TokenType::RWINT  },
            { "irregular",  TokenType::RWIR   },
            { "j",          TokenType::CJ     },
            { "label",      TokenType::RWLB   },
            { "not",        TokenType::RWLN   },
            { "of",         TokenType::RWOF   },
            { "operator",   TokenType::RWOP   },
            { "or",         TokenType::RWLO   },
            { "program",    TokenType::RWPR   },
            { "procedure",  TokenType::RWPRC  },
            { "rational",   TokenType::RWRAT  },
            { "record",     TokenType::RWRC   },
            { "read",       TokenType::RWRD   },
            { "simplify",   TokenType::RWSM   },
            { "then",       TokenType::RWTH   },
            { "to",         TokenType::RWTO   },
            { "type",       TokenType::RWTP   },
            { "var",        TokenType::RWV    },
            { "write",      TokenType::RWWR   }
        };
        for (const auto& pair : reserved_words_map_items)
        {
            reserved_words.insert(pair);
        }
        first_call = false;
    }
    return reserved_words;
}

boost::container::flat_map<TokenType, std::string>& tokenTypeMapping()
{
    static boost::container::flat_map<TokenType, std::string> tokenTypeMapping;
    static bool first_call = true;
    if (first_call)
    {
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
            { TokenType::RWINTOP,   "RWINTOP" }, // 37
            { TokenType::RWPRC,     "RWPRC" }, // 38
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
            { TokenType::TEXT,      "TEXT" },      // 62
            { TokenType::ENDOFFILE, "ENDOFFILE" }   // 63
        };
        for (const auto& pair : tokenTypePairsMapping)
        {
            tokenTypeMapping[pair.first] = pair.second;
        }
        first_call = false;
    }
    return tokenTypeMapping;
}

}
}
