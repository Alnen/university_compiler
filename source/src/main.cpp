#include <memory>
#include <boost/bimap.hpp>

#include "Lexer/Lexer.h"
#include "Lexer/TokenHandler.h"

// experimental
#include <iostream>
#include "Grammar.h"
#include "ControlTable.h"
#include "SyntaxAnalyzer.h"

#include <string>
#include <fstream>
#include "set.h"

#include "Nonterminals.h"
#include "Terminals.h"
#include "PascalRules.h"

class IDHandler : public Lexer::TokenHandler<TokenType>
{
public:
    IDHandler(const boost::bimap<std::string, TokenType>& mapping): 
        m_mapping(mapping)
    {
    }

    virtual TokenPtr operator()(TokenPtr a) override
    {
        std::string value = boost::any_cast<std::string>(a->value());
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        boost::bimap<std::string, TokenType>::left_const_iterator pos;
        if ((pos = m_mapping.left.find(value)) != m_mapping.left.end())
        {
            a->set_type(pos->second);
        }
        return a;
    }

protected:
    const boost::bimap<std::string, TokenType>& m_mapping;
};

int main (int argc, char** argv)
{
    Parser::Set<TokenType> terminals;
    for (size_t i = TERMINAL_START_INDEX + 1;
    i < TERMINAL_FINISH_INDEX; ++i)
    {
        terminals.add(TokenType(i));
    }
    Parser::Set<NonterminalSymbols> nonterminals;
    for (size_t i = NONTERMINAL_START_INDEX + 1;
        i < NONTERMINAL_FINISH_INDEX; ++i)
    {
        nonterminals.add(NonterminalSymbols(i));
    }

    Parser::Grammar<TokenType, NonterminalSymbols> grammar(grammar_rules, Program);
    std::cout<< "grammar.isLL1() "  << grammar.isLL1() << std::endl;
    Parser::SyntaxAnalyzer<TokenType, NonterminalSymbols> syntax_analyzer(grammar);

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
        { "numerator",  TokenType::RWN   },
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
    rules.emplace_back(TokenType::OPAS,     std::string(":=")                    );
    rules.emplace_back(TokenType::OPNE,     std::string("<>")                    );
    rules.emplace_back(TokenType::SRLP,     std::string("\\(")                   );
    rules.emplace_back(TokenType::SRRP,     std::string("\\)")                   );
    rules.emplace_back(TokenType::SRLB,     std::string("\\[")                   );
    rules.emplace_back(TokenType::SRRB,     std::string("\\]")                   );
    rules.emplace_back(TokenType::SRLCB,    std::string("{")                     );
    rules.emplace_back(TokenType::SRRCB,    std::string("}")                     );
    rules.emplace_back(TokenType::SRSM,     std::string(";")                     );
    rules.emplace_back(TokenType::SRCN,     std::string(":")                     );
    rules.emplace_back(TokenType::SRSP,     std::string(".")                     );
    rules.emplace_back(TokenType::SRCA,     std::string(",")                     );
    rules.emplace_back(TokenType::CI,       std::string("[1-9][0-9]*")           );
    rules.emplace_back(TokenType::ID,       std::string("[a-zA-Z][a-zA-Z0-9]*"), std::make_shared<IDHandler>(reserved_words));

    Lexer::Lexer<TokenType> lexer( std::move(rules));
    lexer.openInput("/home/alex/Projects/university_compiler/program.pas");

    bool res = syntax_analyzer.parse(lexer);
    std::cout << "SyntaxAnalyser result: " << res << std::endl;

    return 0;
}
