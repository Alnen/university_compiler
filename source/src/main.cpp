#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_MAP_SIZE 50

#include <memory>
#include <boost/bimap.hpp>

#include "Lexer/Lexer.h"
#include "Lexer/TokenHandler.h"

#include <iostream>
#include "Parser/Grammar.h"
#include "Parser/ControlTable.h"
#include "Parser/SyntaxAnalyzer.h"
#include "Parser/ActionFactory.h"
#include "Parser/StackItem.h"

#include <string>
#include <fstream>
#include <utility>
#include "Parser/set.h"

#include "PascalCompiler/Grammar/Nonterminals.h"
#include "PascalCompiler/Grammar/Terminals.h"
#include "PascalCompiler/Grammar/PascalRules.h"
#include "PascalCompiler/Actions/ActionContainer.hpp"
#include <utility>

#include <boost/mpl/map.hpp>

#include "PascalCompiler/IR/GlobalContext.h"
#include <boost/hana/map.hpp>

std::set<std::string> used_id;
std::set<std::string> used_ci;

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
        else
        {
            used_id.insert(boost::any_cast<std::string>(a->value()));
        }
        return a;
    }

protected:
    const boost::bimap<std::string, TokenType>& m_mapping;
};

class CIHandler : public Lexer::TokenHandler<TokenType>
{
public:
    CIHandler()
    {
    }

    virtual TokenPtr operator()(TokenPtr a) override
    {
        used_ci.insert(boost::any_cast<std::string>(a->value()));
        return a;
    }
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

    Parser::SyntaxAnalyzer<TokenType, NonterminalSymbols, decltype(ActionContainer)> syntax_analyzer(grammar);

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
    rules.emplace_back(TokenType::CI,       std::string("[0-9][0-9]*")         , std::make_shared<CIHandler>());
    rules.emplace_back(TokenType::ID,       std::string("[_a-zA-Z][_a-zA-Z0-9]*"), std::make_shared<IDHandler>(reserved_words()));

    std::ofstream lexer_out("lexer.log", std::ofstream::out&std::ofstream::trunc);
    Lexer::Lexer<TokenType> lexer( std::move(rules), &lexer_out, &(tokenTypeMapping()));
    lexer.openInput(argv[1]);


    PascalCompiler::initModule("nodule");
    // Creating Global Context
    llvm::FunctionType* mainFunctionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), false);
    auto* currentContext =  PascalCompiler::getGlobalModule()->registerContext("main", mainFunctionType, nullptr);
    PascalCompiler::getGlobalModule()->setContext(currentContext);

    PascalCompiler::getGlobalModule()->initializePrintf();

    std::shared_ptr<boost::container::flat_map<std::string, boost::any>> value;
    bool res;
    try
    {
        std::tie(value, res) = syntax_analyzer.parse(lexer);
    }
    catch(std::runtime_error& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit(1);
    }



    std::cout << "SyntaxAnalyser result: " << res << std::endl;

    auto tree = boost::any_cast<std::shared_ptr<PascalCompiler::TreeNode>>((*value)["tree"]);
    tree->print(std::cout, 0, true);

    std::ofstream diag_out("program_tree.txt", std::ofstream::out&std::ofstream::trunc);
    PascalCompiler::print_uml(tree.get()->children()[0].get(), diag_out);

    int symbol = 186;

    std::cout << "first :[ ";
    for (auto val : grammar.ruleFirst({EPSILON, 204}))
    {
        std::cout << val << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "follo :[ ";
    for (auto val : grammar.follow(symbol))
    {
        std::cout << val << ", ";
    }
    std::cout << "]" << std::endl;

    std::ofstream control_table_out("cto.txt", std::ofstream::out&std::ofstream::trunc);
    syntax_analyzer.print(control_table_out);

    std::ofstream lexer_output("lexer_table.log", std::ofstream::out&std::ofstream::trunc);
    lexer_output << std::endl << "ID table" << std::endl;
    for(const auto& id : used_id)
    {
        lexer_output << id << std::endl;
    }


    lexer_output << std::endl << "CI table" << std::endl;
    for(const auto& id : used_ci)
    {
        lexer_output << id << std::endl;
    }
    return 0;
}
