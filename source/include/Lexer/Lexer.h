#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <utility>

#include <boost/container/flat_map.hpp>

#include "Token.h"
#include "RuleImpl.h"
#include "TokenHandler.h"
#include "DefaultErrorHandler.h"

#include "FiniteAutomata/DFA.h"
#include "FiniteAutomata/NFA.h"

#include "RegexAST/RegexASTParser.h"
#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "RegexAST/RegexASTToNFA.h"
#include "Utility.h"

namespace Lexer
{

template <class _TokenType, class _State = int>
class Lexer
{
public:
    using TokenType = _TokenType;
    using TokenPtr = std::unique_ptr<Token<TokenType>>;
    using HandlerPtr = std::unique_ptr<TokenHandler<TokenType>>;
    using State = _State;
    using FinalStates = std::vector<_State>;
    using TokenInfo = std::pair<TokenType, HandlerPtr>;
    using Rule = RuleImpl<TokenType, HandlerPtr>;

    Lexer(std::vector<Rule> rules, std::string inputFile, HandlerPtr&& error_heandler = std::make_unique<DefaultErrorHandler<TokenType>>()/*, std::string serialize_path*/);
    TokenPtr getToken();

protected:
    bool regexASTToDFA(RegexAST::BasicNode* ast, HandlerPtr handler);

    DFA<State, std::shared_ptr<IInputChecker>>  m_DFA;
    boost::container::flat_map<State, TokenInfo>                 m_finalStatesHandlers;
    HandlerPtr                                  m_errorHandler;
    // file
    // offset
    // len
};

template <class _TokenType, class State>
Lexer<_TokenType, State>::Lexer(std::vector<Rule> rules, std::string inputFile, HandlerPtr&& error_heandler/*, std::string serialize_path*/)
{
    m_errorHandler = std::move(error_heandler);

    RegexAST::RegexToASTParser              regex_parser;
    RegexAST::RegexASTAnnotationEvaluator   ast_evaluator;
    RegexAST::BasicLeaf::LeafId             firstLeafId = 1;

    NFA<int, std::shared_ptr<IInputChecker>> a;
    a.addState(0);
    a.setStartState(0);
    RegexASTtoNFA<int, std::shared_ptr<IInputChecker>> b(&a, 0);
    for (auto&  rule : rules)
    {
        // regex to ast
        std::cout << "regex : " << rule.m_regex << std::endl;
        auto parse_answer = regex_parser.parse(rule.m_regex);
        if (!parse_answer.m_success)
        {
            throw std::runtime_error("Malformed regex");
        }

        auto ast = std::move(parse_answer.m_ast);
        // evaluate ast
        auto follow_pos_table = std::move(ast_evaluator.evaluate(ast.get(), firstLeafId));

        // add sub dfa to dfa and get states

        b.convertASTtoNFA(ast.get(), firstLeafId, follow_pos_table.first, follow_pos_table.second);
        // Remember states -> type, handler
        // TODO
        firstLeafId += follow_pos_table.second.size();
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << a;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    m_DFA = std::move(a.convert());
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << m_DFA << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    m_DFA.print_plantuml(std::cout);
}

template <class _TokenType, class _State>
typename Lexer<_TokenType, _State>::TokenPtr
Lexer<_TokenType, _State>::getToken()
{

}

}

#endif // LEXER_H

