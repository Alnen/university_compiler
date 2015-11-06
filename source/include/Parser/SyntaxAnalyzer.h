#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "Grammar.h"
#include "Lexer/Lexer.h"
#include "ControlTable.h"
#include <vector>
#include <iomanip>

#include "boost/any.hpp"

namespace Parser
{

template<class _TerminalType, class _NonterminalType>
class SyntaxAnalyzer
{
public:
    using TeminalType    = typename Grammar<_TerminalType, _NonterminalType>::TerminalType;
    using NonteminalType = typename Grammar<_TerminalType, _NonterminalType>::NonterminalType;
    using TokenType      = typename Grammar<_TerminalType, _NonterminalType>::TokenType;
    using TokenList      = typename Grammar<_TerminalType, _NonterminalType>::TokenList;

    SyntaxAnalyzer(const Grammar<_TerminalType, _NonterminalType>& grammar);
    bool readNextToken(TokenType new_token);
    bool parse(Lexer::Lexer<TeminalType>& lexer);

private:
    int getRuleId(const Rule<TokenType>& rule);
	
private:
    using StackElement = std::pair<TokenType, boost::any>;
    ControlTable<_TerminalType, _NonterminalType>   m_control_table;
    Grammar<_TerminalType, _NonterminalType>        m_grammar;
    std::vector<TokenType>                       m_stack;
    std::vector<boost::any>                         m_valueStack;
    std::vector<boost::any>                         m_backValueStack;
};

template<class TerminalType, class NonterminalType>
bool SyntaxAnalyzer<TerminalType, NonterminalType>::parse(Lexer::Lexer<TeminalType>& lexer)
{
    std::unique_ptr<Lexer::Token<TerminalType>> token;
    while (!m_stack.empty())
    {
        token = lexer.getToken();
        std::cout << "NEW TOKEN : " << std::setw(3) << token->type() << "|" << boost::any_cast<std::string>(token->value()) << std::endl;
        if (!readNextToken(token->type()))
        {
            return false;
        }
    }
    return true;
}

template<class TerminalType, class NonterminalType>
SyntaxAnalyzer<TerminalType, NonterminalType>::SyntaxAnalyzer(const Grammar<TerminalType, NonterminalType>& grammar) : m_grammar(grammar), m_control_table(grammar)
{
    m_stack.push_back(TerminalType::ENDOFFILE);
    m_stack.push_back(grammar.start_symbol);
}

template<class TerminalType, class NonterminalType>
bool SyntaxAnalyzer<TerminalType, NonterminalType>::readNextToken(TokenType new_token)
{
    static bool flag = false;
    while (true)
    {
        std::cout << "stack : [";
        std::copy(m_stack.begin(), m_stack.end(), std::ostream_iterator<TokenType>(std::cout, " "));
        std::cout << "]" << std::endl;
        if (m_stack.back() == new_token)
        {
            m_stack.pop_back();
            return true;
        }
        else if (m_grammar.isTerminal(m_stack.back()))
        {
            return false;
        }
        else
        {
            const Rule<TokenType>& ct_rule = m_control_table.getRule(m_stack.back(), new_token);
            if (ct_rule.left() == Rule<TokenType>::EMPTY_RULE) {
                return false;
            }
            else
            {
                m_stack.pop_back();
                if (!(ct_rule.right().size() == 1 && ct_rule.right()[0] == NonterminalType::EPSILON))
                {
                    std::copy(ct_rule.right().rbegin(), ct_rule.right().rend(), std::back_inserter(m_stack));
                }
            }
        }
    }
}

template<class TerminalType, class NonterminalType>
int SyntaxAnalyzer<TerminalType, NonterminalType>::getRuleId(const Rule<TokenType>& rule)
{
    for (int i = 0; i < m_grammar.rules.size(); ++i) {
        if (m_grammar.rules[i] == rule) return i;
    }
    return -1;
}

}

#endif // SYNTAX_ANALYZER_H
