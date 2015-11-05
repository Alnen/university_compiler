#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "Grammar.h"
#include "Lexer/Lexer.h"
#include "ControlTable.h"
#include <vector>
#include <iomanip>

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
    ControlTable<_TerminalType, _NonterminalType> m_control_table;
    Grammar<_TerminalType, _NonterminalType> m_grammar;
    TokenList m_stack;
    //
    std::vector<int> output_tree;
};

template<class TerminalType, class NonterminalType>
bool SyntaxAnalyzer<TerminalType, NonterminalType>::parse(Lexer::Lexer<TeminalType>& lexer)
{
    std::unique_ptr<Lexer::Token<TerminalType>> token;
    while ((token = lexer.getToken())->type() != TerminalType::ENDOFFILE)
    {
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
            std::cout << "1" << std::endl;
            m_stack.pop_back();
            return true;
        }
        else if (m_grammar.isTerminal(m_stack.back()))
        {
            std::cout << "2 " << m_stack.back() << std::endl;
            return false;
        }
        else
        {
            if (m_stack.back() == 126)
            {
                if (!flag)
                {
                    flag = true;
                }
            }
            const Rule<TokenType>& ct_rule = m_control_table.getRule(m_stack.back(), new_token);
            if (ct_rule.left == Rule<TokenType>::EMPTY_RULE) {
                std::cout << "3" << std::endl;
                return false;
            }
            else
            {
                //output_tree.push_back(getRuleId(ct_rule));
                m_stack.pop_back();
                //std::cout << ct_rule.right.size() << "|" << ct_rule.right[0] << std::endl;
                if (!(ct_rule.right.size() == 1 && ct_rule.right[0] == NonterminalType::EPSILON))
                {
                    std::copy(ct_rule.right.rbegin(), ct_rule.right.rend(), std::back_inserter(m_stack));
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
