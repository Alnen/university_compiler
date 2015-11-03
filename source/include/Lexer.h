#ifndef LEXER_H
#define LEXER_H

#include "DFA.h"
#include "NFA.h"

#include "RegexASTParser.h"
#include "RegexASTAnnotationEvaluator.h"
#include "RegexASTToNFA.h"
#include "NFAtoDFA.h"
#include "Utility.h"

#include <memory>
#include <utility>

#include <boost/container/flat_map.hpp>

namespace Lexer
{

class Token;

class TokenHandler
{
public:
    using TokenPtr = std::unique_ptr<Token>;
    virtual TokenPtr operator()(TokenPtr token) = 0;
};

class IDHandler : public TokenHandler
{
public:
    virtual TokenPtr operator()(TokenPtr a) override
    {
        return a;
    }
};

class DefaultErrorHandler : public TokenHandler
{
public:
    virtual TokenPtr operator()(TokenPtr token) override
    {
        return token;
    }
};

template <class _TokenType, class HandlerPtr = std::unique_ptr<TokenHandler>>
struct RuleImpl
{
    RuleImpl() = default;
    RuleImpl(_TokenType type, std::string regex, HandlerPtr&& handlerPtr):
        m_type(type),
        m_regex(regex),
        m_handlerPtr(std::move(handlerPtr))
    {

    }


    RuleImpl(RuleImpl&& rhs):
        m_type(rhs.m_type),
        m_regex(rhs.m_regex),
        m_handlerPtr(std::move(rhs.m_handlerPtr))
    {

    }

    RuleImpl& operator=(RuleImpl&& rhs)
    {
        m_type = rhs.m_type;
        m_regex = rhs.m_regex;
        m_handlerPtr = std::move(rhs.m_handlerPtr);
    }

    RuleImpl(const RuleImpl&) = delete;
    RuleImpl& operator=(const RuleImpl&) = delete;


    _TokenType m_type;
    std::string m_regex;
    HandlerPtr m_handlerPtr;
};


template <class _TokenType, class _State = int>
class Lexer
{
public:
    using TokenPtr = std::unique_ptr<Token>;
    using TokenType = _TokenType;
    using HandlerPtr = std::unique_ptr<TokenHandler>;
    using State = _State;
    using FinalStates = std::vector<_State>;
    using TokenInfo = std::pair<TokenType, HandlerPtr>;
    using Rule = RuleImpl<TokenType, HandlerPtr>;

    Lexer(std::vector<Rule> rules, std::string inputFile, HandlerPtr&& error_heandler = std::make_unique<DefaultErrorHandler>()/*, std::string serialize_path*/);
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
    // TODO: Optimize
}

/*template <class _TokenType, class _State>
bool
Lexer<_TokenType, _State>::regexASTToDFA(RegexAST::BasicNode* ast, Map<std::unique_ptr<IInputChecker>, std::vector<BasicLeaf::LeafId>>)
{

}*/

template <class _TokenType, class _State>
typename Lexer<_TokenType, _State>::TokenPtr
Lexer<_TokenType, _State>::getToken()
{

}

}

#endif // LEXER_H

