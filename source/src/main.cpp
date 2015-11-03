#include <memory>
#include <boost/bimap.hpp>

#include "Lexer.h"

enum class TokenType
{
    //
    ID,
    CR,
    CI,
    CX,
    //
    RWLA,
    RWAR,
    RWB,
    RWCM,
    RWCN,
    RWDO,
    RWDT,
    RWEL,
    RWEND,
    RWFOR,
    RWFR,
    RWGT,
    RWIF,
    RWINT,
    RWIR,
    RWLB,
    RWLN,
    RWOF,
    RWOP,
    RWLO,
    RWPR,
    RWRAT,
    RWRD,
    RWSM,
    RWTH,
    RWTO,
    RWTP,
    RWV,
    RWWR,
    //
    RWDN,
    RWN,
    //
    OPPLUS,
    OPMINUS,
    OPMUL,
    OPDIV,
    OPGT,
    OPLT,
    OPGE,
    OPLE,
    OPEQ,
    OPNE,
    OPAS,
    //
    SRLP,
    SRRP,
    SRLB,
    SRRB,
    SRSM,
    SRCN,
    SRSP,
    SRCA
};

int main (int argc, char** argv)
{
    using Lexer::IDHandler;

    // reserwed words map
    std::vector<boost::bimap<std::string, TokenType>::value_type> reserved_words_map_items =
    {
        { "and",        TokenType::RWLA  },
        { "array",      TokenType::RWAR  },
        { "begin",      TokenType::RWB   },
        { "common",     TokenType::RWCM  },
        { "const",      TokenType::RWCN  },
        { "do",         TokenType::RWDO  },
        { "denominator",TokenType::RWDN  },
        { "downto",     TokenType::RWDT  },
        { "else",       TokenType::RWEL  },
        { "end",        TokenType::RWEND },
        { "for",        TokenType::RWFOR },
        { "frac",       TokenType::RWFR  },
        { "goto",       TokenType::RWGT  },
        { "if",         TokenType::RWIF  },
        { "integer",    TokenType::RWINT },
        { "irregular",  TokenType::RWIR  },
        { "label",      TokenType::RWLB  },
        { "numenator",  TokenType::RWN   },
        { "not",        TokenType::RWLN  },
        { "of",         TokenType::RWOF  },
        { "operator",   TokenType::RWOP  },
        { "or",         TokenType::RWLO  },
        { "program",    TokenType::RWPR  },
        { "rational",   TokenType::RWRAT },
        { "read",       TokenType::RWRD  },
        { "simplify",   TokenType::RWSM  },
        { "then",       TokenType::RWTH  },
        { "to",         TokenType::RWTO  },
        { "type",       TokenType::RWTP  },
        { "var",        TokenType::RWV   },
        { "write",      TokenType::RWWR  }
    };
    boost::bimap<std::string, TokenType> reserved_words(reserved_words_map_items.begin(), reserved_words_map_items.end());

    //rules
    std::vector<Lexer::Lexer<TokenType>::Rule> rules;
    rules.emplace_back(TokenType::OPPLUS,   std::string("+#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPMINUS,  std::string("-#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPMUL,    std::string("\\*#"),                            std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPDIV,    std::string("/#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPGT,     std::string(">#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPLT,     std::string("<#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPGE,     std::string(">=#"),                             std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPLE,     std::string("<=#"),                             std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPEQ,     std::string("=#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPNE,     std::string(":=#"),                             std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::OPAS,     std::string("<>#"),                             std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRLP,     std::string("\\(#"),                            std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRRP,     std::string("\\)#"),                            std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRLB,     std::string("\\[#"),                            std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRRB,     std::string("\\]#"),                            std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRSM,     std::string(";#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRCN,     std::string(":#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRSP,     std::string(".#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::SRCA,     std::string(",#"),                              std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::CR,       std::string("([1-9][0-9]*|0).0*[1-9][0-9]*#"),    std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::CI,       std::string("[1-9][0-9]*#"),                    std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::CX,       std::string("([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*)j#"),                    std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::CX,       std::string("{ *([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*) *, *([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*) *}#"),                    std::move(std::make_unique<IDHandler>()));
    rules.emplace_back(TokenType::ID,       std::string("[a-zA-Z][a-zA-Z0-9]*#"),           std::move(std::make_unique<IDHandler>()));
    Lexer::Lexer<TokenType> lexer( std::move(rules), "");

    return 0;
}
