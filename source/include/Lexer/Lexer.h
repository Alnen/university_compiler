#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <utility>

#include <boost/container/flat_map.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "Token.h"
#include "RuleImpl.h"
#include "TokenHandler.h"
#include "DefaultErrorHandler.h"

#include "FiniteAutomata/DFA.h"
#include "FiniteAutomata/NFA.h"
#include "FiniteAutomata/DFAWalker.h"

#include "RegexAST/RegexASTParser.h"
#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "RegexAST/RegexASTToNFA.h"
#include "Utility.h"
#include "StdFix.h"

namespace Lexer
{

template <class _TokenType, class _State = int>
class Lexer
{
public:
    using TokenType = _TokenType;
    using TokenPtr = std::unique_ptr<Token<TokenType>>;
    using HandlerPtr = std::shared_ptr<TokenHandler<TokenType>>;
    using ErrorHandlerPtr = std::unique_ptr<TokenHandler<TokenType>>;
    using State = _State;
    using FinalStates = std::vector<_State>;
    using TokenInfo = std::pair<TokenType, HandlerPtr>;
    using Rule = RuleImpl<TokenType>;

    Lexer(std::vector<Rule> rules, ErrorHandlerPtr error_heandler = std::make_unique<DefaultErrorHandler<TokenType>>()/*, std::string serialize_path*/);
    void openInput(const std::string& path);
    TokenPtr getToken();

    ~Lexer();

protected:
    bool regexASTToDFA(RegexAST::BasicNode* ast, HandlerPtr handler);

    // DFA
    DFA<State, std::shared_ptr<IInputChecker>>      m_DFA;
    boost::container::flat_map<State, TokenInfo>    m_finalStatesHandlers;
    ErrorHandlerPtr                                 m_errorHandler;
    // File
    boost::iostreams::mapped_file_source            m_sourceFile;
    size_t                                          m_offset;
    size_t                                          m_len;
};

template <class _TokenType, class State>
Lexer<_TokenType, State>::Lexer(std::vector<Rule> rules, ErrorHandlerPtr error_heandler/*, std::string serialize_path*/)
{
    m_errorHandler = std::move(error_heandler);


    RegexAST::RegexToASTParser              regex_parser;
    RegexAST::RegexASTAnnotationEvaluator   ast_evaluator;
    RegexAST::BasicLeaf::LeafId             firstLeafId = 1;

    NFA<int, std::shared_ptr<IInputChecker>> nfa;
    nfa.addState(0);
    nfa.setStartState(0);
    RegexASTtoNFA<int, std::shared_ptr<IInputChecker>> regex_to_dfa(&nfa, 0);
    boost::container::flat_map<State, size_t> nfa_handler_mapping;
    for (size_t i = 0; i < rules.size(); ++i)
    {
        auto&  rule  = rules[i];
        // regex to ast
        std::cout << "regex : " << rule.m_regex << std::endl;
        auto parse_answer = regex_parser.parse(rule.m_regex + "#");
        if (!parse_answer.m_success)
        {
            throw std::runtime_error("Malformed regex");
        }
        auto ast = std::move(parse_answer.m_ast);
        // evaluate ast
        auto follow_pos_table = std::move(ast_evaluator.evaluate(ast.get(), firstLeafId));
        // add sub dfa to dfa and get states
        auto final_states = regex_to_dfa.convertASTtoNFA(ast.get(), firstLeafId, follow_pos_table.first, follow_pos_table.second);
        // Remember states -> type, handler
        for (State state : final_states)
        {
            nfa_handler_mapping[state] = i;
        }
        // TODO
        firstLeafId += follow_pos_table.second.size();
    }
    std::cout << nfa << std::endl;
    boost::container::flat_map<State, std::vector<State>> nfa_to_dfa_final_state_map;
    std::tie(m_DFA, nfa_to_dfa_final_state_map) = std::move(nfa.convert());
    for (const auto& mapping : nfa_to_dfa_final_state_map)
    {
        std::cout << "BOOM" << std::endl;
        size_t rule_number = -1;
        for (State intermidiate_state : mapping.second)
        {
            if (rule_number > nfa_handler_mapping[intermidiate_state])
            {
                rule_number = nfa_handler_mapping[intermidiate_state];
            }
        }
        m_finalStatesHandlers[mapping.first] = std::make_pair(rules[rule_number].m_type, rules[rule_number].m_handlerPtr);
    }
    std::cout << m_DFA << std::endl;
    m_DFA.print_plantuml(std::cout);
}

template <class TokenType, class State>
typename Lexer<TokenType, State>::TokenPtr
Lexer<TokenType, State>::getToken()
{
    std::vector<char> ignore = {' ', '\t', '\n'};
    Walker<State, std::shared_ptr<IInputChecker>> walker(&m_DFA);
    if (m_offset == m_sourceFile.size())
    {
        return std::make_unique<Token<TokenType>>(TokenType::ENDOFFILE, "EOF");
    }

    m_len = 0;

    while (m_offset < m_sourceFile.size() && std::find(ignore.begin(), ignore.end(), m_sourceFile.data()[m_offset]) != ignore.end()) ++m_offset;

    //std::cout << "GET TOKEN" << std::endl;
    //std::cout << "current_state :" << walker.getCurrentState() << std::endl;
    //std::cout << "current_input :" << m_sourceFile.data()[m_offset + m_len] << std::endl;
    while (m_offset + m_len < m_sourceFile.size() && walker.goNextState(m_sourceFile.data()[m_offset + m_len]))
    {
        //std::cout << "current_state :" << walker.getCurrentState() << std::endl;
        //std::cout << "current_input :" << m_sourceFile.data()[m_offset + m_len] << std::endl;
        ++m_len;
    }

    std::unique_ptr<Token<TokenType>> token;
    if (walker.isCurrentStateFinal())
    {
        const auto& rule_info= m_finalStatesHandlers[walker.getCurrentState()];
        token = std::make_unique<Token<TokenType>>(rule_info.first, std::string(&m_sourceFile.data()[m_offset], m_len));
        token = (*rule_info.second)(std::move(token));
    }
    else
    {
        std::cout << "NOT FOUND" << std::endl;
        token = std::make_unique<Token<TokenType>>(TokenType::ERROR, "error");
    }
    m_offset += m_len;

    return token;
}

template <class _TokenType, class _State>
void Lexer<_TokenType, _State>::openInput(const std::string& path)
{
    if (m_sourceFile.is_open())
    {
        m_sourceFile.close();
    }
    m_sourceFile.open(path);
    m_offset = 0;
    m_len = 0;
}

template <class _TokenType, class _State>
Lexer<_TokenType, _State>::~Lexer()
{
    if (m_sourceFile.is_open())
    {
        m_sourceFile.close();
    }
}

}

#endif // LEXER_H

