#include <memory>
#include <boost/bimap.hpp>

#include "Lexer/Lexer.h"
#include "Lexer/TokenHandler.h"

enum class TokenType
{
    ID,
    CJ,
    CI,
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
    RWCX,
    RWFUN,
    RWIM,
    RWRE,
    RWINTOP,
    RWPRC,
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
    SRCA,
    SRLCB,
    SRRCB,
    //
    ERROR,
    ENDOFFILE
};

class IDHandler : public Lexer::TokenHandler<TokenType>
{
public:
    virtual TokenPtr operator()(TokenPtr a) override
    {
        a->value() = boost::any_cast<std::string>(a->value()) + "ANY";
        return a;
    }
};

int main (int argc, char** argv)
{
    // reserwed words map
    std::vector<boost::bimap<std::string, TokenType>::value_type> reserved_words_map_items =
    {
        { "and",        TokenType::RWLA  },
        { "array",      TokenType::RWAR  },
        { "begin",      TokenType::RWB   },
        { "common",     TokenType::RWCM  },
        { "complex",    TokenType::RWCX  },
        { "const",      TokenType::RWCN  },
        { "do",         TokenType::RWDO  },
        { "denominator",TokenType::RWDN  },
        { "downto",     TokenType::RWDT  },
        { "else",       TokenType::RWEL  },
        { "end",        TokenType::RWEND },
        { "for",        TokenType::RWFOR },
        { "frac",       TokenType::RWFR  },
        { "function",   TokenType::RWFUN },
        { "goto",       TokenType::RWGT  },
        { "if",         TokenType::RWIF  },
        { "im",         TokenType::RWIM  },
        { "int",        TokenType::RWINTOP},
        { "integer",    TokenType::RWINT },
        { "irregular",  TokenType::RWIR  },
        { "im",         TokenType::RWIM  },
        { "j",          TokenType::CJ    },
        { "label",      TokenType::RWLB  },
        { "numenator",  TokenType::RWN   },
        { "not",        TokenType::RWLN  },
        { "of",         TokenType::RWOF  },
        { "operator",   TokenType::RWOP  },
        { "or",         TokenType::RWLO  },
        { "program",    TokenType::RWPR  },
        { "procedure",  TokenType::RWPRC },
        { "rational",   TokenType::RWRAT },
        { "re",         TokenType::RWRE  },
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
    rules.emplace_back(TokenType::OPPLUS,   std::string("+")                     );
    rules.emplace_back(TokenType::OPMINUS,  std::string("-")                     );
    rules.emplace_back(TokenType::OPMUL,    std::string("\\*")                   );
    rules.emplace_back(TokenType::OPDIV,    std::string("/")                     );
    rules.emplace_back(TokenType::OPGT,     std::string(">")                     );
    rules.emplace_back(TokenType::OPLT,     std::string("<")                     );
    rules.emplace_back(TokenType::OPGE,     std::string(">=")                    );
    rules.emplace_back(TokenType::OPLE,     std::string("<=")                    );
    rules.emplace_back(TokenType::OPEQ,     std::string("=")                     );
    rules.emplace_back(TokenType::OPNE,     std::string(":=")                    );
    rules.emplace_back(TokenType::OPAS,     std::string("<>")                    );
    rules.emplace_back(TokenType::SRLP,     std::string("\\(")                   );
    rules.emplace_back(TokenType::SRRP,     std::string("\\)")                   );
    rules.emplace_back(TokenType::SRLB,     std::string("\\[")                   );
    rules.emplace_back(TokenType::SRRB,     std::string("\\]")                   );
    rules.emplace_back(TokenType::SRLB,     std::string("{")                     );
    rules.emplace_back(TokenType::SRRB,     std::string("}")                     );
    rules.emplace_back(TokenType::SRSM,     std::string(";")                     );
    rules.emplace_back(TokenType::SRCN,     std::string(":")                     );
    rules.emplace_back(TokenType::SRSP,     std::string(".")                     );
    rules.emplace_back(TokenType::SRCA,     std::string(",")                     );
    rules.emplace_back(TokenType::CI,       std::string("[1-9][0-9]*")           );
    rules.emplace_back(TokenType::ID,       std::string("[a-zA-Z][a-zA-Z0-9]*"), std::make_shared<IDHandler>());

    Lexer::Lexer<TokenType> lexer( std::move(rules));
    lexer.openInput("/home/alex/Projects/university_compiler/program.pas");
    std::unique_ptr<Lexer::Token<TokenType>> token;
    while ((token = lexer.getToken())->type() != TokenType::ENDOFFILE)
    {
        std::cout << "NEW TOKEN : " << boost::any_cast<std::string>(token->value()) << std::endl;

    }
    return 0;
}
