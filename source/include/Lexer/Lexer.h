#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <utility>
#include <iostream>
#include <fstream>

#include <boost/container/flat_map.hpp>
#ifdef BOOST_OS_MACOS
    #include <boost/iostreams/device/mapped_file.hpp>
#else
    #include <fstream>
#endif
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

template <class TokenType>
class LexerLogger
{
public:
    LexerLogger(std::ostream* outPtr, boost::container::flat_map<TokenType, std::string>* mapping):
        m_outPtr(outPtr),
        m_tokenTypeMapping(mapping)
    {
    }

    template <class T>
    LexerLogger& operator<<(T&& value)
    {
        if (m_outPtr != nullptr)
        {
            (*m_outPtr) << std::forward<T>(value);
        }
        return *this;
    }

    template <class T>
    LexerLogger& operator<<(Token<T>& value)
    {

        if (m_outPtr != nullptr)
        {
            if (m_tokenTypeMapping != nullptr)
            {
                (*m_outPtr) << (*m_tokenTypeMapping)[value.type()];
            }
            else
            {
                (*m_outPtr) << value.type();
            }
            (*m_outPtr) << "{V = \"" << boost::any_cast<std::string>(value.value()) << "\"}";
        }
        return *this;
    }

protected:
    std::ostream* m_outPtr;
    boost::container::flat_map<TokenType, std::string>* m_tokenTypeMapping;
};

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

    Lexer(std::vector<Rule> rules, std::ostream* logger = &std::cout, boost::container::flat_map<TokenType, std::string>* mappin = nullptr, ErrorHandlerPtr error_heandler = std::make_unique<DefaultErrorHandler<TokenType>>());
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
#ifdef BOOST_OS_MACOS
    boost::iostreams::mapped_file_source            m_sourceFile;
#else
	std::string                                     m_sourceFile;
#endif	
    size_t                                          m_offset;
    size_t                                          m_len;
    // Log
    LexerLogger<TokenType>                          m_logger;
};

template <class _TokenType, class State>
Lexer<_TokenType, State>::Lexer(std::vector<Rule> rules, std::ostream* logger, boost::container::flat_map<_TokenType, std::string>* mapping, ErrorHandlerPtr error_heandler):
    m_logger(logger, mapping)
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
    std::ofstream out("lexer_dfa.txt", std::ofstream::out&std::ofstream::trunc);
    std::cout << m_DFA << std::endl;
    m_DFA.print_plantuml(out);
}

template <class TokenType, class State>
typename Lexer<TokenType, State>::TokenPtr
Lexer<TokenType, State>::getToken()
{
    std::vector<char> ignore = {' ', '\t', '\r'};
    Walker<State, std::shared_ptr<IInputChecker>> walker(&m_DFA);

    while (m_offset < m_sourceFile.size() && std::find(ignore.begin(), ignore.end(), m_sourceFile.data()[m_offset]) != ignore.end())
    {
        m_logger <<  m_sourceFile.data()[m_offset];
        ++m_offset;
    }

    std::unique_ptr<Token<TokenType>> token;

    if (m_offset == m_sourceFile.size())
    {
        token = std::make_unique<Token<TokenType>>(TokenType::ENDOFFILE, "EOF");
    }
    else
    {
        m_len = 0;

        while (m_offset + m_len < m_sourceFile.size() && walker.goNextState(m_sourceFile.data()[m_offset + m_len]))
        {
            ++m_len;
        }


        if (walker.isCurrentStateFinal())
        {
            const auto& rule_info= m_finalStatesHandlers[walker.getCurrentState()];
            token = std::make_unique<Token<TokenType>>(rule_info.first, std::string(&m_sourceFile.data()[m_offset], m_len));
            token = (*rule_info.second)(std::move(token));
        }
        else
        {
            std::cout << "NOT FOUND" << std::endl;
            token = std::make_unique<Token<TokenType>>(TokenType::ERROR, std::string(&m_sourceFile.data()[m_offset], ++m_len));
        }
        m_offset += m_len;
    }

    m_logger << *token << " ";

    return token;
}

template <class _TokenType, class _State>
void Lexer<_TokenType, _State>::openInput(const std::string& path)
{
#ifdef BOOST_OS_MACOS
    if (m_sourceFile.is_open())
    {
        m_sourceFile.close();
    }
    m_sourceFile.open(path);
#else
	std::ifstream sourceFile(path);
    m_sourceFile = std::string((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
	std::cout << "[LEXER] Path: " << path << "\n [LEXER] Sources: " << m_sourceFile << std::endl;
#endif
    m_offset = 0;
    m_len = 0;

    m_logger << "[Lexer] Reading programm code from : \"" << path << "\"\n";
}

template <class _TokenType, class _State>
Lexer<_TokenType, _State>::~Lexer()
{
#ifdef BOOST_OS_MACOS
    if (m_sourceFile.is_open())
    {
        m_sourceFile.close();
    }
#endif
}

}

#endif // LEXER_H

